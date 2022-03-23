#pragma once

#include <unordered_set>

#include "types_and_constants.h"

namespace engine {

class System {
 public:
  System();
  virtual ~System() = default;
  virtual void Update() = 0;

  std::unordered_set<Entity> entities_;
};

}  // namespace engine
