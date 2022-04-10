#include "painting_system.h"

#include <iostream>

#include "core/components.h"

namespace  systems {

PaintingSystem::PaintingSystem(engine::Coordinator* coordinator)
    : coordinator_(coordinator) {}

void PaintingSystem::Update() {
  for (engine::Entity entity : entities_) {
    auto& pos_comp
        = coordinator_->GetComponent<core::MovementComponent>(entity);
    auto& graphics_item_comp
        = coordinator_->GetComponent<core::GraphicsItemComponent>(entity);
    if (graphics_item_comp.item == nullptr) {
      continue;
    }
    graphics_item_comp.item
        ->setPos(pos_comp.position.x(), pos_comp.position.y());
    if (entity == 440) {
        std::cout << "Painting system is updated) ";
        std::cout << "pos of entity " << entity << " is " << pos_comp.position.x()
                  << " " << pos_comp.position.y() << " now" << std::endl;
    }
  }
}

}  // namespace systems
