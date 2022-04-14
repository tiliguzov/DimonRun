#include "movement_system.h"

#include "core/components.h"

namespace systems {

MovementSystem::MovementSystem(engine::Coordinator* coordinator)
    : coordinator_(coordinator) {}

void MovementSystem::Update() {
  for (engine::Entity entity : entities_) {
    if (!coordinator_->HasComponent<core::MovementComponent>(entity)) {
      continue;
    }
    auto& comp = coordinator_->GetComponent<core::PositionComponent>(entity);

    auto& movement =
      coordinator_->GetComponent<core::MovementComponent>(entity);

    comp.position += movement.direction * movement.current_speed;
  }
}

}  // namespace systems
