#include <iostream>
#include "animation_system.h"
#include "core/components.h"
#include "core/constants.h"

systems::AnimationSystem::AnimationSystem(engine::Coordinator* coordinator)
      : coordinator_(coordinator), time_(0) {}

void systems::AnimationSystem::Update() {
  time_ += core::kTickTime;

  for (engine::Entity entity : entities_) {
    auto& anim_comp =
        coordinator_->GetComponent<core::AnimationComponent>(entity);
    const auto& graphics_comp =
        coordinator_->GetComponent<core::GraphicsItemComponent>(entity);

    // check if we do not need to change frame
    if (!CurrentFrameIsOk(anim_comp.frames->GetFrameDuration(), time_)) {
      anim_comp.need_change_frame = true;
    }
    if (anim_comp.need_change_frame) {
      QPixmap* image = anim_comp.frames->GetFrame(anim_comp.move_type, time_);
      if (anim_comp.direction == core::HorizontalDirection::kRight) {
        graphics_comp.item->setPixmap(*image);
      } else {
        graphics_comp.item->setPixmap(
            image->transformed(QTransform().scale(-1, 1)));
      }
      anim_comp.need_change_frame = false;
    }
  }
}

bool systems::AnimationSystem::CurrentFrameIsOk(int32_t frame_duration,
                                                int32_t time) {
  return (time % frame_duration) >= core::kTickTime;
}
