#include "connector.h"

#include <memory>

#include "components.h"
#include "engine/coordinator.h"
#include "systems/movement_system.h"
#include "systems/painting_system.h"
#include "constants.h"

namespace core {

Connector::Connector() : coordinator_(std::make_unique<engine::Coordinator>()) {
  RegisterComponents();
  RegisterSystems();
}

void Connector::OnTick() {
  for (const auto& system : systems_) {
    system->Update();
  }
}

void Connector::RegisterSystems() {
  auto movement_system = coordinator_
      ->RegisterSystem<systems::MovementSystem>(coordinator_.get());

  auto painting_system = coordinator_
      ->RegisterSystem<systems::PaintingSystem>(coordinator_.get());

  coordinator_->SetSystemSignature<systems::MovementSystem>
      ({coordinator_->GetComponentID<MovementComponent>()});

  coordinator_->SetSystemSignature<systems::PaintingSystem>
      ({coordinator_->GetComponentID<MovementComponent>(),
          coordinator_->GetComponentID<GraphicsItemComponent>()});

  systems_.push_back(movement_system);
  systems_.push_back(painting_system);
}

void Connector::RegisterComponents() {
  coordinator_->RegisterComponent<MovementComponent>();
  coordinator_->RegisterComponent<GraphicsItemComponent>();
}

// example of interacting with engine
void Connector::Example(Scene* scene) {
  for (int i = -10; i <= 10; ++i) {
    for (int j = -10; j <= 10; ++j) {
      engine::Entity entity = coordinator_->CreateEntity();
      float x = i * core::kTextureSize;
      float y = j * core::kTextureSize;
      coordinator_->AddComponent(entity, MovementComponent{{x, y}, true});
      auto* item = scene->GetScene()->addPixmap(QPixmap(":ground.jpg"));
      coordinator_->AddComponent(entity, GraphicsItemComponent{item});
    }
  }
  engine::Entity player = coordinator_->CreateEntity();
  coordinator_->AddComponent(player, MovementComponent{{0, 150}, false});
  coordinator_->AddComponent(player, GraphicsItemComponent
       {scene->GetScene()->addPixmap(QPixmap(":fox.png"))});
  coordinator_->GetComponent<GraphicsItemComponent>(player).item->setPos(0, 150);
}

}  // namespace core
