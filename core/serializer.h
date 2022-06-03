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

  void DeleteEntity(engine::Entity entity);

 private:
  struct Dungeon {
    Dungeon() = default;
    int offset_x{0};
    int offset_y{0};
    int entities_count{0};
    QGraphicsPixmapItem* background_image;
    std::vector<engine::Entity> entities;
  };

 private:
  void DownloadDungeonFromJson(DungeonName dungeon_name);
  template<typename ComponentType>
  void DownloadCompFromJson(
      engine::Entity entity,
      const std::unique_ptr<Dungeon>& dungeon,
      const QJsonObject& entity_object);


  template<typename ComponentType>
  void DownloadCompIfNecessary(
      engine::Entity entity,
      const std::unique_ptr<Dungeon>& dungeon,
      std::ifstream& stream);

  template<typename ComponentType>
  void UploadCompIfNecessary(
      engine::Entity entity,
      const std::unique_ptr<Dungeon>& dungeon,
      std::ofstream& stream);

  template<typename ComponentType>
  ComponentType DownloadComponent(
      std::ifstream& stream,
      const std::unique_ptr<Dungeon>& dungeon);

  template<typename ComponentType>
  void UploadComponent(
      std::ofstream& stream,
      const std::unique_ptr<Dungeon>& dungeon,
      const ComponentType& component);

  void RemoveEntityFromScene(engine::Entity entity);

 private:
  engine::Coordinator* coordinator_;
  core::Scene* scene_;

  std::unordered_map<DungeonName, std::unique_ptr<Dungeon>> dungeons_;
};

//----------- Position Component Specialization --------------------------------
template<>
void Serializer::DownloadCompFromJson<PositionComponent>(
    engine::Entity entity,
    const std::unique_ptr<Dungeon>& dungeon,
    const QJsonObject& entity_object);

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
void Serializer::DownloadCompFromJson<GraphicsItemComponent>(
    engine::Entity entity,
    const std::unique_ptr<Dungeon>& dungeon,
    const QJsonObject& entity_object);

template<>
GraphicsItemComponent Serializer::DownloadComponent<GraphicsItemComponent>(
    std::ifstream& stream,
    const std::unique_ptr<Dungeon>&);

template<>
void Serializer::UploadComponent<GraphicsItemComponent>(
    std::ofstream& stream,
    const std::unique_ptr<Dungeon>&,
    const GraphicsItemComponent& component);

//----------- Collision Component Specialization --------------------------
template<>
void Serializer::DownloadCompFromJson<CollisionComponent>(
    engine::Entity entity,
    const std::unique_ptr<Dungeon>& dungeon,
    const QJsonObject& entity_object);

template<>
CollisionComponent Serializer::DownloadComponent<CollisionComponent>(
    std::ifstream& stream,
    const std::unique_ptr<Dungeon>&);

template<>
void Serializer::UploadComponent<CollisionComponent>(
    std::ofstream& stream,
    const std::unique_ptr<Dungeon>&,
    const CollisionComponent& component);

//----------- Illness Component Specialization --------------------------
template<>
void Serializer::DownloadCompFromJson<IllnessComponent>(
    engine::Entity entity,
    const std::unique_ptr<Dungeon>& dungeon,
    const QJsonObject& entity_object);

template<>
IllnessComponent Serializer::DownloadComponent<IllnessComponent>(
    std::ifstream& stream,
    const std::unique_ptr<Dungeon>&);

template<>
void Serializer::UploadComponent<IllnessComponent>(
    std::ofstream& stream,
    const std::unique_ptr<Dungeon>&,
    const IllnessComponent& component);


//----------- Movement Component Specialization ---------------------------
template<>
void Serializer::DownloadCompFromJson<MovementComponent>(
    engine::Entity entity,
    const std::unique_ptr<Dungeon>& dungeon,
    const QJsonObject& entity_object);

template<>
MovementComponent Serializer::DownloadComponent<MovementComponent>(
    std::ifstream& stream,
    const std::unique_ptr<Dungeon>&);

template<>
void Serializer::UploadComponent<MovementComponent>(
    std::ofstream& stream,
    const std::unique_ptr<Dungeon>&,
    const MovementComponent& component);

//----------- Animation Component Specialization ---------------------------
template<>
void Serializer::DownloadCompFromJson<AnimationComponent>(
    engine::Entity entity,
    const std::unique_ptr<Dungeon>& dungeon,
    const QJsonObject& entity_object);

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
