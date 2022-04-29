#pragma once

#include "engine/coordinator.h"
#include "engine/system.h"

namespace systems {

class AnimationSystem : public engine::System {
 public:
  explicit AnimationSystem(engine::Coordinator* coordinator);
  void Update() override;

  static bool CurrentFrameIsOk(int32_t frame_duration, int32_t time);

 private:
  engine::Coordinator* coordinator_;
  uint64_t time_;
};

}  // namespace systems
