#pragma once

#include <memory>
#include <vector>
#include "core/scene.h"

#include "engine/coordinator.h"

namespace core {

class Connector {
 public:
  Connector();

  void OnTick();
  void Example(Scene* scene);

 private:
  void RegisterSystems();
  void RegisterComponents();

  std::unique_ptr<engine::Coordinator> coordinator_;
  std::vector<std::shared_ptr<engine::System>> systems_;
};

}  // namespace core
