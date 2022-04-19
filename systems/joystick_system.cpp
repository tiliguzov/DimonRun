#include "joystick_system.h"
#include <iostream>

namespace systems {

JoystickSystem::JoystickSystem(
        engine::Coordinator *coordinator,
        core::Keyboard *keyboard) : coordinator_(coordinator),
        keyboard_(keyboard) {}

void JoystickSystem::Update() {
  if (keyboard_->IsBlocked()) {
    return;
  }
  for (const auto& entity : entities_) {
    auto& motion = coordinator_->GetComponent<core::MovementComponent>(entity);

    auto& position = coordinator_->GetComponent<core::PositionComponent>(entity);

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
    if (static_cast<int>(position.position.x()) % core::kTextureSize != 0 ||
        static_cast<int>(position.position.y()) % core::kTextureSize != 0) {
        motion.direction = motion.last_nonzero_direction;
        std::cout << direction.x() << " " << direction.y() << std::endl;
    } else {
        motion.direction  = motion.last_nonzero_direction = direction;
    }
  }
}

}  // namespace systems
