#pragma once

#include "components.h"
#include "dungeon_name.h"
#include "engine/coordinator.h"
#include "scene.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace core {

class Serializer {
 public:
  Serializer(engine::Coordinator* coordinator, Scene* scene);

  void DownloadDungeon(DungeonName dungeon_name, DungeonType dungeon_type);
  void UploadDungeon(DungeonName dungeon_name, DungeonType dungeon_type);
  void RemoveDungeon(DungeonName dungeon_name);

 private:
  struct Dungeon {
    Dungeon() = default;
    int offset_x{0};
    int offset_y{0};
    int entities_count{0};
    std::vector<engine::Entity> entities;
  };

 private:
  void DownloadDungeonFromJson(DungeonName dungeon_name);

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

  // Paths to dungeon files
  std::unordered_map<DungeonName, std::string> source_by_name_default_{
      {DungeonName::kHub, "default_hub"}
  };

  std::unordered_map<DungeonName, std::string> source_by_name_edited_{
      {DungeonName::kHub, "edited_hub"}
  };

  std::unordered_map<DungeonName, QString> source_by_name_hand_created_{
      {DungeonName::kHub, ":hub.json"}
  };
};

//----------- Position Component Specialization --------------------------------

template<>
PositionComponent Serializer::DownloadComponent<PositionComponent>(
    std::ifstream& stream,
    const std::unique_ptr<Dungeon>& dungeon);

template<>
void Serializer::UploadComponent<PositionComponent>(
    std::ofstream& stream,
    const std::unique_ptr<Dungeon>& dungeon,
    const PositionComponent& component);

//----------- Graphics Item Component Specialization ---------------------------

template<>
GraphicsItemComponent Serializer::DownloadComponent<GraphicsItemComponent>(
    std::ifstream& stream,
    const std::unique_ptr<Dungeon>&);

template<>
void Serializer::UploadComponent<GraphicsItemComponent>(
    std::ofstream& stream,
    const std::unique_ptr<Dungeon>&,
    const GraphicsItemComponent& component);

//----------- Animation Component Specialization ---------------------------

template<>
AnimationComponent Serializer::DownloadComponent<AnimationComponent>(
    std::ifstream& stream,
    const std::unique_ptr<Dungeon>&);

template<>
void Serializer::UploadComponent<AnimationComponent>(
    std::ofstream& stream,
    const std::unique_ptr<Dungeon>&,
    const AnimationComponent& component);

}  // namespace core
