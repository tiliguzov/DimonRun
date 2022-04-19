#include "movement_system.h"
#include <iostream>

#include "core/components.h"

namespace systems {

MovementSystem::MovementSystem(engine::Coordinator* coordinator)
    : coordinator_(coordinator) {}

void MovementSystem::Update() {
  for (engine::Entity entity : entities_) {
    auto& comp = coordinator_->GetComponent<core::PositionComponent>(entity);

    auto& movement =
      coordinator_->GetComponent<core::MovementComponent>(entity);

    comp.position += movement.direction * movement.current_speed;
    std::cout << "pos of entity # " << entity << " is " << comp.position.x() << " " << comp.position.y() << std::endl;
  }
}

}  // namespace systems
