#pragma once

#include <QKeyEvent>

#include <memory>
#include <vector>

#include "core/keyboard.h"
#include "core/scene.h"
#include "core/serializer.h"
#include "engine/coordinator.h"
#include "core/dungeon_name.h"
#include "core/location_manager/location_manager.h"
#include "core/events_constants.h"

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
  void OpenNewDungeon(DungeonName dungeon_name);


  std::shared_ptr<engine::Coordinator> GetCoordinator();

 private:
  void RegisterSystems();
  void RegisterComponents();

  Scene* scene_{nullptr};

  std::shared_ptr<engine::Coordinator> coordinator_;
  std::unique_ptr<core::Keyboard> keyboard_;
  std::unique_ptr<Serializer> serializer_;
  std::unique_ptr<LocationManager> location_manager_;

  std::vector<std::shared_ptr<engine::System>> systems_;

  std::unordered_set<DungeonName> current_dungeons_;
};

}  // namespace core
