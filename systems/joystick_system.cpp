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

    auto& animation_comp = coordinator_->
            GetComponent<core::AnimationComponent>(entity);

    QVector2D direction;
    if (keyboard_->IsKeyPressed(core::KeyAction::kMoveDown)) {
      direction = {0.0, 1.0};

      animation_comp.movement_type = core::MovementType::kFliesDown;
    }
    if (keyboard_->IsKeyPressed(core::KeyAction::kMoveUp)) {
      direction = {0.0, -1.0};

      animation_comp.movement_type = core::MovementType::kFliesUp;
    }
    if (keyboard_->IsKeyPressed(core::KeyAction::kMoveLeft)) {
      direction = {-1.0, 0.0};

      animation_comp.movement_type = core::MovementType::kFliesHorizontal;
      animation_comp.direction = core::HorizontalDirection::kLeft;
    }
    if (keyboard_->IsKeyPressed(core::KeyAction::kMoveRight)) {
      direction = {1.0, 0.0};

      animation_comp.movement_type = core::MovementType::kFliesHorizontal;
      animation_comp.direction = core::HorizontalDirection::kRight;
    }
    if (static_cast<int>(position_comp.position.x()) %
            core::kTextureSize == 0 &&
        static_cast<int>(position_comp.position.y()) %
            core::kTextureSize == 0) {
        motion_comp.direction = direction;

        animation_comp.movement_type = core::MovementType::kStaticInAir;
    }
  }
}

}  // namespace systems
