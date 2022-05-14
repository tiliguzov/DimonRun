#include "joystick_system.h"
#include <iostream>

namespace systems {

JoystickSystem::JoystickSystem(
        engine::Coordinator *coordinator, core::Keyboard *keyboard) :
        coordinator_(coordinator), keyboard_(keyboard) {}

void JoystickSystem::Update() {
  for (const auto& entity : entities_) {
    auto& move_comp = coordinator_->
            GetComponent<core::MovementComponent>(entity);
    auto& pos_comp = coordinator_->
            GetComponent<core::PositionComponent>(entity);
    auto& anim_comp = coordinator_->
            GetComponent<core::AnimationComponent>(entity);

    core::MovementType old_type = anim_comp.move_type;
    core::HorizontalDirection old_direction = anim_comp.direction;

    QVector2D direction;
    if (keyboard_->IsKeyPressed(core::KeyAction::kMoveDown)) {
      direction = {0.0, 1.0};

      anim_comp.move_type = core::MovementType::kFliesDown;
    }
    if (keyboard_->IsKeyPressed(core::KeyAction::kMoveUp)) {
      direction = {0.0, -1.0};

      anim_comp.move_type = core::MovementType::kFliesUp;
    }
    if (keyboard_->IsKeyPressed(core::KeyAction::kMoveLeft)) {
      direction = {-1.0, 0.0};

      anim_comp.move_type = core::MovementType::kFliesHorizontal;
      anim_comp.direction = core::HorizontalDirection::kLeft;
    }
    if (keyboard_->IsKeyPressed(core::KeyAction::kMoveRight)) {
      direction = {1.0, 0.0};

      anim_comp.move_type = core::MovementType::kFliesHorizontal;
      anim_comp.direction = core::HorizontalDirection::kRight;
    }
    if (static_cast<int>(pos_comp.position.x()) % core::kTextureSize == 0 &&
        static_cast<int>(pos_comp.position.y()) % core::kTextureSize == 0) {
      move_comp.direction = direction;

      if (direction.isNull()) {
        anim_comp.move_type = core::MovementType::kStaticInAir;
      }
    }

    if (anim_comp.move_type != old_type ||
          anim_comp.direction != old_direction) {
      anim_comp.need_change_frame = true;
    }
  }
}

}  // namespace systems
