#pragma once

#include "engine/coordinator.h"
#include "components.h"
#include "scene.h"
#include "dungeon_name.h"

namespace core {

class Serializer {
 public:
  Serializer(engine::Coordinator* coordinator, Scene* scene);

 private:
  struct Dungeon {
    Dungeon() = default;
    int offset_x{0};
    int offset_y{0};
    int entities_count{0};
    std::vector<engine::Entity> entities;
  };
  // template<class CompType>
  // void DownloadComponent(std::ifstream& stream, CompType& component, const Dungeon);

  void DownloadDungeon(DungeonName dungeon_name);
  PositionComponent DownloadPositionComponent(
      std::ifstream& stream, const std::unique_ptr<Dungeon>&);
  GraphicsItemComponent DownloadGraphicsItemComponent(
      std::ifstream& stream, const std::unique_ptr<Dungeon>&);
  MovementComponent DownloadMovementComponent(
      std::ifstream& stream, const std::unique_ptr<Dungeon>&);



  engine::Coordinator* coordinator_;
  core::Scene* scene_;

  std::unordered_map<DungeonName, std::unique_ptr<Dungeon>> dungeons_;
  std::unordered_map<DungeonName, std::string> source_by_name_;
};

} // core
