#pragma once

#include <unordered_set>

#include "types_and_constants.h"

class System {
 public:
  System();
  virtual void Update() = 0;
  std::unordered_set<Entity> entities_;
};
