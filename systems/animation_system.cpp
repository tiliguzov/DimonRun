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

    if (NeedChangeFrame(&anim_comp, time_)) {
      QPixmap& image = anim_comp.frames.GetFrame(anim_comp.move_type, time_);
      if (anim_comp.direction == core::HorizontalDirection::kRight) {
        graphics_comp.item->setPixmap(image);
      } else {
        graphics_comp.item->setPixmap(
            image.transformed(QTransform().scale(-1, 1)));
      }
      anim_comp.need_change_frame = false;
    }
  }
}
bool systems::AnimationSystem::NeedChangeFrame(
          core::AnimationComponent* anim_comp,
          int32_t time) {
  if (anim_comp->need_change_frame) {
    anim_comp->need_change_frame = false;
    return true;
  }
  return (time % anim_comp->frames.GetFrameDuration()) >= core::kTickTime;
}
