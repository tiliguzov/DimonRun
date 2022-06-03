#pragma once

#include <QKeyEvent>

#include <memory>
#include <vector>

#include "core/keyboard.h"
#include "core/scene.h"
#include "core/serializer.h"
#include "engine/coordinator.h"
#include "core/dungeon_name.h"

namespace core {

class Connector {
 public:
  Connector();

  void OnTick();
  void StartGame(Scene* scene);

  void OnKeyPress(Qt::Key key);
  void OnKeyRelease(Qt::Key key);

  void DeleteEntity(engine::Entity entity);
  void UseEvent(engine::Entity);
  void CheckAndAddCoin(engine::Entity);
  Serializer* GetSerializer();

  DungeonName GetCurrentDungeon();
  void SetCurrentDungeon(DungeonName);


  std::shared_ptr<engine::Coordinator> GetCoordinator();

 private:
  void RegisterSystems();
  void RegisterComponents();

  std::shared_ptr<engine::Coordinator> coordinator_;
  std::unique_ptr<core::Keyboard> keyboard_;
  std::unique_ptr<Serializer> serializer_;

  std::vector<std::shared_ptr<engine::System>> systems_;

  DungeonName current_dungeon_;
};

}  // namespace core
