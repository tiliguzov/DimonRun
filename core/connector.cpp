#include "connector.h"

#include <memory>

#include "components.h"
#include "engine/coordinator.h"
#include "systems/movement_system.h"

namespace core {

Connector::Connector() : coordinator_(std::make_unique<engine::Coordinator>()) {
  RegisterComponents();
  RegisterSystems();

  // example of interaction with engine
  engine::Entity entity = coordinator_->CreateEntity();
  coordinator_->AddComponent(entity, MovementComponent{{0, 0}});
}

void Connector::OnTick() {
  for (const auto& system : systems_) {
    system->Update();
  }
}

void Connector::RegisterSystems() {
  auto movement_system = coordinator_
      ->RegisterSystem<systems::MovementSystem>(coordinator_.get());

  coordinator_->SetSystemSignature<systems::MovementSystem>
      ({coordinator_->GetComponentID<MovementComponent>()});

  systems_.push_back(movement_system);
}

void Connector::RegisterComponents() {
  coordinator_->RegisterComponent<MovementComponent>();
}

}  // namespace core
