#pragma once

#include "engine/coordinator.h"
#include "engine/system.h"

namespace systems {

class AnimationSystem : public engine::System {
 public:
  explicit AnimationSystem(engine::Coordinator* coordinator);
  void Update() override;

 private:
  engine::Coordinator* coordinator_;
  uint64_t time_;
};

}  // namespace systems