#include "joystick_system.h"
#include <iostream>

namespace systems {

JoystickSystem::JoystickSystem(
        engine::Coordinator *coordinator, core::Keyboard *keyboard) :
        coordinator_(coordinator), keyboard_(keyboard) {}

void JoystickSystem::Update() {
  for (const auto& entity : entities_) {
    auto& motion_comp = coordinator_->
            GetComponent<core::MovementComponent>(entity);

    auto& position_comp = coordinator_->
            GetComponent<core::PositionComponent>(entity);

    QVector2D direction;
    if (keyboard_->IsKeyPressed(core::KeyAction::kMoveDown)) {
      direction = {0.0, 1.0};
    }
    if (keyboard_->IsKeyPressed(core::KeyAction::kMoveUp)) {
      direction = {0.0, -1.0};
    }
    if (keyboard_->IsKeyPressed(core::KeyAction::kMoveLeft)) {
      direction = {-1.0, 0.0};
    }
    if (keyboard_->IsKeyPressed(core::KeyAction::kMoveRight)) {
      direction = {1.0, 0.0};
    }
    if (static_cast<int>(position_comp.position.x()) %
            core::kTextureSize == 0 &&
        static_cast<int>(position_comp.position.y()) %
            core::kTextureSize == 0) {
        motion_comp.direction = direction;
    }
  }
}

}  // namespace systems
