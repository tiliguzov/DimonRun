#pragma once

#include <memory>
#include <vector>

#include <QKeyEvent>

#include "core/keyboard.h"
#include "engine/coordinator.h"

class MCConnector {
 public:
  MCConnector();

  void OnTick();

  std::shared_ptr<engine::Coordinator> GetCoordinator();

 private:
  void RegisterSystems();
  void RegisterComponents();

  std::shared_ptr<engine::Coordinator> coordinator_;
  std::vector<std::shared_ptr<engine::System>> systems_;
  std::unique_ptr<core::Keyboard> keyboard_;
};
