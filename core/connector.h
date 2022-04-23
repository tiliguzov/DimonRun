#pragma once

#include <memory>
#include <vector>

#include <QKeyEvent>

#include "core/keyboard.h"
#include "core/scene.h"
#include "engine/coordinator.h"

namespace core {

class Connector {
 public:
  Connector();

  void OnTick();
  void StartGame(Scene* scene);

  QGraphicsItem* CreateHero(Scene* scene);

  void OnKeyPress(Qt::Key key);
  void OnKeyRelease(Qt::Key key);

 private:
  void RegisterSystems();
  void RegisterComponents();

  std::unique_ptr<engine::Coordinator> coordinator_;
  std::vector<std::shared_ptr<engine::System>> systems_;
  std::unique_ptr<core::Keyboard> keyboard_;
};

}  // namespace core
