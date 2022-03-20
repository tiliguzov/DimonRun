#include <iostream>

#include "movement_system.h"
#include "core/components.h"

MovementSystem::MovementSystem(Coordinator* coordinator)
    : coordinator_(coordinator) {}

void MovementSystem::Update() {
  std::cout << "Movement system is updated)" << " ";
  for (Entity entity : entities_) {
    auto& comp = coordinator_->GetComponent<MovementComponent>(entity);
    comp.position += {1, 1};
    std::cout << "pos of entity " << entity << " is " << comp.position.x()
              << " " << comp.position.y() << " now" << std::endl;
  }
}
