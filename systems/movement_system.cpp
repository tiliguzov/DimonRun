#include "movement_system.h"

#include "core/components.h"

#include <iostream>

namespace systems {

MovementSystem::MovementSystem(engine::Coordinator* coordinator)
    : coordinator_(coordinator) {}

void MovementSystem::Update() {
    const long double eps = 1e-1;
  for (engine::Entity entity : entities_) {
    auto& pos_comp = coordinator_->
            GetComponent<core::PositionComponent>(entity);
    auto& move_comp =
      coordinator_->GetComponent<core::MovementComponent>(entity);
    pos_comp.position += move_comp.direction * move_comp.current_speed;
    if (std::abs(pos_comp.position.x() - std::round(pos_comp.position.x())) < eps) {
        pos_comp.position.setX(std::round(pos_comp.position.x()));
    }
    if (std::abs(pos_comp.position.y() - std::round(pos_comp.position.y())) < eps) {
        pos_comp.position.setY(std::round(pos_comp.position.y()));
    }
  }
}

}  // namespace systems
