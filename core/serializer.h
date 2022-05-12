#pragma once

#include "engine/coordinator.h"
#include "components.h"
#include "scene.h"
#include "dungeon_name.h"

namespace core {

class Serializer {
 public:
  Serializer(engine::Coordinator* coordinator, Scene* scene);

  void DownloadDungeon(DungeonName dungeon_name);
  void UploadDungeon(DungeonName dungeon_name);
  void RemoveDungeon(DungeonName dungeon_name);

  void DownloadDungeonFromJson(DungeonName dungeon_name);

 private:
  struct Dungeon {
    Dungeon() = default;
    int offset_x{0};
    int offset_y{0};
    int entities_count{0};
    std::vector<engine::Entity> entities;
  };

 private:
  template<typename ComponentType>
  ComponentType DownloadComponent(
      std::ifstream& stream,
      const std::unique_ptr<Dungeon>& dungeon);

  template<typename ComponentType>
  void UploadComponent(
      std::ofstream& stream,
      const std::unique_ptr<Dungeon>& dungeon,
      const ComponentType& component);

 private:
  engine::Coordinator* coordinator_;
  core::Scene* scene_;

  std::unordered_map<DungeonName, std::unique_ptr<Dungeon>> dungeons_;
  std::unordered_map<DungeonName, std::string> source_by_name_{
      {DungeonName::kDefaultHub, "default_hub.txt"},
      {DungeonName::kEditedHub, "edited_hub.txt"}
  };

  std::unordered_map<DungeonName, QString> json_source_by_name_{
      // {DungeonName::kDefaultHub, ":dungeons/default_hub_example.json"}
      {DungeonName::kDefaultHub, ":dungeons/default_hub.json"}
  };
};

template<>
PositionComponent Serializer::DownloadComponent<PositionComponent>(
    std::ifstream& stream,
    const std::unique_ptr<Dungeon>& dungeon);

template<>
GraphicsItemComponent Serializer::DownloadComponent<GraphicsItemComponent>(
    std::ifstream& stream,
    const std::unique_ptr<Dungeon>&);

template<>
void Serializer::UploadComponent<PositionComponent>(
    std::ofstream& stream,
    const std::unique_ptr<Dungeon>& dungeon,
    const PositionComponent& component);

template<>
void Serializer::UploadComponent<GraphicsItemComponent>(
    std::ofstream& stream,
    const std::unique_ptr<Dungeon>&,
    const GraphicsItemComponent& component);


} // core
