#include "movement_system.h"

#include "core/components.h"

namespace systems {

MovementSystem::MovementSystem(engine::Coordinator* coordinator)
    : coordinator_(coordinator) {}

void MovementSystem::Update() {
  for (engine::Entity entity : entities_) {
    auto& position_comp = coordinator_->GetComponent<core::PositionComponent>(entity);

    auto& movement_comp =
      coordinator_->GetComponent<core::MovementComponent>(entity);

    position_comp.position += movement_comp.direction * movement_comp.current_speed;
  }
}

}  // namespace systems
