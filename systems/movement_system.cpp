#include "movement_system.h"

#include "core/components.h"

namespace systems {

MovementSystem::MovementSystem(engine::Coordinator* coordinator)
    : coordinator_(coordinator) {}

void MovementSystem::Update() {
  for (engine::Entity entity : entities_) {
    auto& pos_comp = coordinator_->
            GetComponent<core::PositionComponent>(entity);
    auto& move_comp =
      coordinator_->GetComponent<core::MovementComponent>(entity);

    pos_comp.position += move_comp.direction * move_comp.current_speed;
  }
}

}  // namespace systems
