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
    auto& comp = coordinator_->GetComponent<core::MovementComponent>(entity);

    auto& transform =
      coordinator_->GetComponent<core::TransformationComponent>(entity);

    comp.position += transform.direction * transform.current_speed;
    if (entity == 440) {
        std::cout << "Movement system is updated)" << " ";
        std::cout << "pos of entity " << entity << " is " << comp.position.x()
                  << " " << comp.position.y() << " now" << std::endl;
    }
  }
}

}  // namespace systems
