#pragma once

#include "engine/coordinator.h"
#include "engine/system.h"

#include "core/connector.h"
#include "core/components.h"

namespace systems {

class CollisionSystem : public engine::System {
 public:
  explicit CollisionSystem(engine::Coordinator* coordinator, core::Connector*);
  void Update() override;

 private:
  engine::Coordinator* coordinator_;
  core::Connector* connector_;
};

}  // namespace systems
