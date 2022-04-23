#include "animation_system.h"
#include "core/components.h"
#include "core/constants.h"

systems::AnimationSystem::AnimationSystem(engine::Coordinator* coordinator)
      : coordinator_(coordinator), time_(0) {}

void systems::AnimationSystem::Update() {
  time_ += core::kTickTime;

  for (engine::Entity entity : entities_) {
    const auto& animation =
        coordinator_->GetComponent<core::AnimationComponent>(entity);
    const auto& graphics_item_component =
        coordinator_->GetComponent<core::GraphicsItemComponent>(entity);
    QPixmap* image = animation.animations->GetFrame(
        animation.movement_type, time_);
    graphics_item_component.item->setPixmap(*image);
    if (animation.direction == core::HorizontalDirection::kLeft) {
      graphics_item_component.item->setPixmap(
          image->transformed(QTransform().scale(-1, 1)));
    }
  }
}
