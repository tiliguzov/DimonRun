#pragma once

#include "engine/coordinator.h"
#include "engine/system.h"

#include "core/components.h"

namespace systems {

class CollisionSystem : public engine::System {
 public:
  explicit CollisionSystem(engine::Coordinator* coordinator);
  void Update() override;

 private:
  bool IsIntersectPositions(core::PositionComponent&, core::PositionComponent&);
  engine::Coordinator* coordinator_;
};

}  // namespace systems
