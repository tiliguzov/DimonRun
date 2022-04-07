#include "joystick_system.h"

namespace systems {

JoystickSystem::JoystickSystem(
        engine::Coordinator *coordinator,
        core::Keyboard *keyboard) : coordinator_(coordinator), keyboard_(keyboard) {}

void JoystickSystem::Update() {
  if (keyboard_->IsBlocked()) {
    return;
  }
  for (const auto& entity : entities_) {
    auto& motion = coordinator_->GetComponent<core::TransformationComponent>(entity);
    QVector2D direction;
    if (keyboard_->IsKeyPressed(core::KeyAction::kMoveDown)) {
      direction += {0.0, -1.0};
    }
    if (keyboard_->IsKeyPressed(core::KeyAction::kMoveUp)) {
      direction += {0.0, 1.0};
    }
    if (keyboard_->IsKeyPressed(core::KeyAction::kMoveLeft)) {
      direction += {-1.0, 0.0};
    }
    if (keyboard_->IsKeyPressed(core::KeyAction::kMoveRight)) {
      direction += {1.0, 0.0};
    }
    motion.direction = direction.normalized();
  }
}

}  // namespace systems
