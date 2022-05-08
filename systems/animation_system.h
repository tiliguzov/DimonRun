#pragma once

#include "engine/coordinator.h"
#include "engine/system.h"
#include "core/components.h"

namespace systems {

class AnimationSystem : public engine::System {
 public:
  explicit AnimationSystem(engine::Coordinator* coordinator);
  void Update() override;

  static bool NeedChangeFrame(core::AnimationComponent& anim_comp,
                              int32_t time);

 private:
  engine::Coordinator* coordinator_;
  uint64_t time_;
};

}  // namespace systems
