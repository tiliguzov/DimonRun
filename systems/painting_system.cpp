#include "painting_system.h"

#include <iostream>

#include "core/components.h"

namespace  systems {

PaintingSystem::PaintingSystem(engine::Coordinator* coordinator)
    : coordinator_(coordinator) {}

void PaintingSystem::Update() {
  for (engine::Entity entity : entities_) {
    if (!coordinator_->HasComponent<core::GraphicsItemComponent>(entity)) {
      continue;
    }
    auto& pos_comp
        = coordinator_->GetComponent<core::PositionComponent>(entity);
    auto& graphics_item_comp
        = coordinator_->GetComponent<core::GraphicsItemComponent>(entity);
    graphics_item_comp.item
        ->setPos(pos_comp.position.x(), pos_comp.position.y());
    if (graphics_item_comp.need_camera_following) {
      graphics_item_comp.camera->centerOn(graphics_item_comp.item);
    }
  }
}

}  // namespace systems
