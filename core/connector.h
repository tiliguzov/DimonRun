#pragma once

#include <unordered_set>
#include <memory>
#include <vector>

#include "engine/coordinator.h"

class Connector {
 public:
  Connector();

  void OnTick();

 private:
  void RegisterSystems();
  void RegisterComponents();

  std::unique_ptr<Coordinator> coordinator_;
  std::vector<std::shared_ptr<System>> systems_;
};
