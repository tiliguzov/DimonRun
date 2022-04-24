#include <iostream>
#include "animation_system.h"
#include "core/components.h"
#include "core/constants.h"

systems::AnimationSystem::AnimationSystem(engine::Coordinator* coordinator)
      : coordinator_(coordinator), time_(0) {}

void systems::AnimationSystem::Update() {
  time_ += core::kTickTime;

  for (engine::Entity entity : entities_) {
    auto& animation_comp =
        coordinator_->GetComponent<core::AnimationComponent>(entity);
    const auto& graphics_item_comp =
        coordinator_->GetComponent<core::GraphicsItemComponent>(entity);

    // check if we do not need to change frame
    if (!animation_comp.type_changed_last_tick &&
             CurrentFrameIsOk(animation_comp.animations
             ->GetFrameDuration(), time_)) {
      continue;
    }
    animation_comp.type_changed_last_tick = false;

    QPixmap* image = animation_comp.animations->GetFrame(
        animation_comp.movement_type, time_);
    if (animation_comp.direction == core::HorizontalDirection::kRight) {
      graphics_item_comp.item->setPixmap(*image);
    } else {
      graphics_item_comp.item->setPixmap(
          image->transformed(QTransform().scale(-1, 1)));
    }
  }
}

bool systems::AnimationSystem::CurrentFrameIsOk(int32_t frame_duration,
                                                int32_t time) {
  return (time % frame_duration) >= core::kTickTime;
}
