#include "connector.h"

#include <memory>

#include "components.h"
#include "engine/coordinator.h"
#include "systems/movement_system.h"
#include "systems/joystick_system.h"

namespace core {

Connector::Connector() : coordinator_(
                             std::make_unique<engine::Coordinator>()),
                             keyboard_(std::make_unique<core::Keyboard>()) {
  RegisterComponents();
  RegisterSystems();

  // example of interaction with engine
  engine::Entity entity = coordinator_->CreateEntity();
  coordinator_->AddComponent(entity, MovementComponent{{0, 0}});
  coordinator_->AddComponent(entity, TransformationComponent{1, {0, 0}, 1});
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
}

void Connector::RegisterComponents() {
  coordinator_->RegisterComponent<MovementComponent>();
  coordinator_->RegisterComponent<TransformationComponent>();
}

}  // namespace core
