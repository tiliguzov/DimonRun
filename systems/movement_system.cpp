#include "movement_system.h"

#include <iostream>

#include "core/components.h"

namespace systems {

MovementSystem::MovementSystem(engine::Coordinator* coordinator)
    : coordinator_(coordinator) {}

void MovementSystem::Update() {
  // example of interaction with engine
  for (engine::Entity entity : entities_) {
    if (!coordinator_->
          GetComponent<core::MovementComponent>(entity).can_move) {
      continue;
    }
    std::cout << "Movement system is updated)" << " ";
    auto& comp = coordinator_->GetComponent<core::MovementComponent>(entity);
    comp.position += {1, 1};
    std::cout << "pos of entity " << entity << " is " << comp.position.x()
              << " " << comp.position.y() << " now" << std::endl;
  }
}

}  // namespace systems
