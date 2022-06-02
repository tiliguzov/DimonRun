#pragma once

#include <QKeyEvent>

#include <memory>
#include <vector>

#include "core/keyboard.h"
#include "core/scene.h"
#include "core/serializer.h"
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

  void DeleteEntity(engine::Entity entity);

 private:
  void RegisterSystems();
  void RegisterComponents();

  std::unique_ptr<engine::Coordinator> coordinator_;
  std::unique_ptr<core::Keyboard> keyboard_;
  std::unique_ptr<Serializer> serializer_;

  std::vector<std::shared_ptr<engine::System>> systems_;
};

}  // namespace core
