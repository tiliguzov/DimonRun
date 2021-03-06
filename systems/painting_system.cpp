#include "painting_system.h"

#include "core/components.h"

namespace  systems {

PaintingSystem::PaintingSystem(engine::Coordinator* coordinator)
    : coordinator_(coordinator) {}

void PaintingSystem::Update() {
  for (engine::Entity entity : entities_) {
    auto& pos_comp
        = coordinator_->GetComponent<core::PositionComponent>(entity);
    auto& graphics_comp
        = coordinator_->GetComponent<core::GraphicsItemComponent>(entity);

    graphics_comp.item->setPos(pos_comp.position.x(), pos_comp.position.y());
  }
}

}  // namespace systems
