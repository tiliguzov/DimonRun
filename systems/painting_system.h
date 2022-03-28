#pragma once

#include "engine/system.h"
#include "engine/coordinator.h"

namespace systems {

class PaintingSystem : public engine::System {
 public:
  explicit PaintingSystem(engine::Coordinator* coordinator);
  void Update() override;

 private:
  engine::Coordinator* coordinator_;
};

}  // namespace systems

