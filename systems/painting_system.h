#pragma once

#include "engine/coordinator.h"
#include "engine/system.h"

namespace systems {

class PaintingSystem : public engine::System {
 public:
  explicit PaintingSystem(engine::Coordinator* coordinator);
  void Update() override;

 private:
  engine::Coordinator* coordinator_;
};

}  // namespace systems

