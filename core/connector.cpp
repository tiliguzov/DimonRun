#include "connector.h"

#include <memory>
#include <iostream>

#include "constants.h"
#include "components.h"
#include "engine/coordinator.h"
#include "systems/joystick_system.h"
#include "systems/movement_system.h"
#include "systems/painting_system.h"

namespace core {

Connector::Connector() : coordinator_(
                             std::make_unique<engine::Coordinator>()),
                             keyboard_(std::make_unique<core::Keyboard>()) {
  RegisterComponents();
  RegisterSystems();
                               
  // example of interaction with engine
  // engine::Entity entity = coordinator_->CreateEntity();
  // coordinator_->AddComponent(entity, MovementComponent{{0, 0}});
  // coordinator_->AddComponent(entity, TransformationComponent{1, {0, 0}, 1});

}

void Connector::OnTick() {
  for (const auto& system : systems_) {
    system->Update();
  }
}

void Connector::OnKeyPress(Qt::Key key) {
    keyboard_->OnPress(key);
}

void Connector::OnKeyRelease(Qt::Key key) {
    keyboard_->OnRelease(key);
}

void Connector::RegisterSystems() {
    {
        auto joystick_system_ = coordinator_
            ->RegisterSystem<systems::JoystickSystem>(
                    coordinator_.get(), keyboard_.get());

        coordinator_->SetSystemSignature<systems::JoystickSystem>
          ({coordinator_->GetComponentID<TransformationComponent>()});

        systems_.push_back(joystick_system_);
    }
    {
        auto movement_system = coordinator_
            ->RegisterSystem<systems::MovementSystem>(coordinator_.get());
            
        coordinator_->SetSystemSignature<systems::MovementSystem>
          ({coordinator_->GetComponentID<MovementComponent>()});

        systems_.push_back(movement_system);
    }
    {
        auto painting_system = coordinator_
            ->RegisterSystem<systems::PaintingSystem>(coordinator_.get());
      
        coordinator_->SetSystemSignature<systems::PaintingSystem>
            ({coordinator_->GetComponentID<MovementComponent>(),
                  coordinator_->GetComponentID<GraphicsItemComponent>()});
        
        systems_.push_back(painting_system);
    }
}

void Connector::RegisterComponents() {
  coordinator_->RegisterComponent<MovementComponent>();
  coordinator_->RegisterComponent<TransformationComponent>();
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
      coordinator_->AddComponent(entity, TransformationComponent{{0, 0}, 1});
      auto* item = scene->GetScene()->addPixmap(QPixmap(":ground.jpg"));
      coordinator_->AddComponent(entity, GraphicsItemComponent{item});
    }
  }
  engine::Entity player = coordinator_->CreateEntity();
  coordinator_->AddComponent(player, MovementComponent{{0, 0}, false});
  coordinator_->AddComponent(player, TransformationComponent{{0, 0}, 0});
  coordinator_->AddComponent(player, GraphicsItemComponent
       {scene->GetScene()->addPixmap(QPixmap(":fox.png"))});
  /*coordinator_->GetComponent<GraphicsItemComponent>(player).item
        ->setPos(0, 150);*/
}

}  // namespace core
