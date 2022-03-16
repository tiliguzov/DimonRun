#pragma once

#include "types_and_constants.h"

class IComponentArray {
 public:  // todo sounds like a good place to protected
  virtual void EntityDestroyed(Entity) = 0;
  virtual ~IComponentArray() = 0;  // todo maybe = default
  // todo add other functions?
};
