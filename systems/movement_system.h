#pragma once

#include "engine/coordinator.h"

namespace systems {

class MovementSystem : public engine::System {
 public:
  explicit MovementSystem(engine::Coordinator* coordinator);
  void Update() override;

 private:
  engine::Coordinator* coordinator_;
};

}  // namespace systems
