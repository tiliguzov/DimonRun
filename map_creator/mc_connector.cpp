#include "mc_connector.h"

#include <memory>
#include <iostream>

#include "core/constants.h"
#include "core/components.h"
#include "engine/coordinator.h"
#include "systems/painting_system.h"
#include "systems/animation_system.h"

MCConnector::MCConnector() :
    coordinator_(std::make_unique<engine::Coordinator>()) {
  RegisterComponents();
  RegisterSystems();
}

void MCConnector::OnTick() {
  for (const auto& system : systems_) {
    system->Update();
  }
}

void MCConnector::RegisterSystems() {
  {
    auto painting_system = coordinator_
        ->RegisterSystem<systems::PaintingSystem>(coordinator_.get());
    coordinator_->SetSystemSignature<systems::PaintingSystem>
        ({coordinator_->GetComponentID<core::PositionComponent>(),
          coordinator_->GetComponentID<core::GraphicsItemComponent>()});
    systems_.push_back(painting_system);
  }
  {
    auto animation_system = coordinator_
        ->RegisterSystem<systems::AnimationSystem>(coordinator_.get());
    coordinator_->SetSystemSignature<systems::AnimationSystem>
        ({coordinator_->GetComponentID<core::AnimationComponent>(),
          coordinator_->GetComponentID<core::GraphicsItemComponent>()});
    systems_.push_back(animation_system);
  }
}

void MCConnector::RegisterComponents() {
  coordinator_->RegisterComponent<core::PositionComponent>();
  coordinator_->RegisterComponent<core::GraphicsItemComponent>();
  coordinator_->RegisterComponent<core::AnimationComponent>();
}

std::shared_ptr<engine::Coordinator> MCConnector::GetCoordinator() {
  return coordinator_;
}
