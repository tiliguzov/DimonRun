#include "serializer.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <fstream>
#include <iostream>

#include "components.h"
#include "constants.h"

namespace core {

namespace {

template<typename T>
void Read(std::ifstream& is, T* data, int size) {
  is.read(reinterpret_cast<char*>(data), size);
}

template <typename T>
void Write(std::ofstream& os, T* data, int size) {
  os.write(reinterpret_cast<const char*>(data), size);
}

}  // namespace

Serializer::Serializer(engine::Coordinator* coordinator, Scene* scene) :
    coordinator_(coordinator), scene_(scene) {}

void Serializer::DownloadDungeon(
    DungeonName dungeon_name,
    DungeonType dungeon_type) {
  std::ifstream in_stream;
  std::string file_name;
  switch (dungeon_type) {
    case DungeonType::kDefault: {
      file_name = source_by_name_default_.at(dungeon_name);
      break;
    }
    case DungeonType::kEdited: {
      file_name = source_by_name_edited_.at(dungeon_name);
      break;
    }
    case DungeonType::kHandCreated: {
      DownloadDungeonFromJson(dungeon_name);
      return;
    }
  }
  in_stream.open(file_name,
                 std::ios::binary | std::ios::in);
  if (!in_stream) {
    std::cerr << "Cannot open dungeon download file!";
    exit(1);
  }

  dungeons_.insert({dungeon_name, std::make_unique<Dungeon>()});
  auto& new_dungeon = dungeons_.at(dungeon_name);
  Read(in_stream, &new_dungeon->offset_x, sizeof(new_dungeon->offset_x));
  Read(in_stream, &new_dungeon->offset_y, sizeof(new_dungeon->offset_y));
  Read(in_stream, &new_dungeon->entities_count,
       sizeof(new_dungeon->entities_count));

  for (int i = 0; i < new_dungeon->entities_count; i++) {
    engine::Entity new_entity = coordinator_->CreateEntity();
    new_dungeon->entities.push_back(new_entity);

    engine::ComponentSignature component_signature;
    Read(in_stream, &component_signature, sizeof(engine::ComponentSignature));
    coordinator_->SetComponentSignature(new_entity, component_signature);

    if (coordinator_->HasComponent<PositionComponent>(new_entity)) {
      coordinator_->AddComponent(
          new_entity,
          DownloadComponent<PositionComponent>(in_stream, new_dungeon));
    }
    if (coordinator_->HasComponent<GraphicsItemComponent>(new_entity)) {
      coordinator_->AddComponent(
          new_entity,
          DownloadComponent<GraphicsItemComponent>(in_stream, new_dungeon));
    }
    if (coordinator_->HasComponent<MovementComponent>(new_entity)) {
      coordinator_->AddComponent(
          new_entity,
          DownloadComponent<MovementComponent>(in_stream, new_dungeon));
    }
    if (coordinator_->HasComponent<AnimationComponent>(new_entity)) {
      coordinator_->AddComponent(
          new_entity,
          DownloadComponent<AnimationComponent>(in_stream, new_dungeon));
    }
  }

  in_stream.close();
  if (!in_stream.good()) {
    std::cerr << "Error occurred at dungeon reading time!";
    exit(1);
  }
}

void Serializer::UploadDungeon(
    DungeonName dungeon_name,
    DungeonType dungeon_type) {
  std::ofstream out_stream;
  std::string file_name;
  switch (dungeon_type) {
    case DungeonType::kDefault:
    case DungeonType::kEdited: {
      file_name = source_by_name_edited_.at(dungeon_name);
      break;
    }
    case DungeonType::kHandCreated: {
      file_name = source_by_name_default_.at(dungeon_name);
      break;
    }
  }
  out_stream.open(file_name,
                  std::ios::binary | std::ios::out);
  if (!out_stream) {
    std::cerr << "Cannot open dungeon upload file!";
    exit(1);
  }

  auto& dungeon = dungeons_.at(dungeon_name);
  Write(out_stream, &dungeon->offset_x, sizeof(dungeon->offset_x));
  Write(out_stream, &dungeon->offset_y, sizeof(dungeon->offset_y));
  Write(out_stream, &dungeon->entities_count, sizeof(dungeon->entities_count));

  for (auto entity : dungeon->entities) {
    engine::ComponentSignature component_signature{
        coordinator_->GetComponentSignature(entity)};
    Write(out_stream, &component_signature, sizeof(engine::ComponentSignature));

    if (coordinator_->HasComponent<PositionComponent>(entity)) {
      UploadComponent(out_stream, dungeon,
          coordinator_->GetComponent<PositionComponent>(entity));
    }
    if (coordinator_->HasComponent<GraphicsItemComponent>(entity)) {
      UploadComponent(out_stream, dungeon,
          coordinator_->GetComponent<GraphicsItemComponent>(entity));
    }
    if (coordinator_->HasComponent<MovementComponent>(entity)) {
      UploadComponent(out_stream, dungeon,
          coordinator_->GetComponent<MovementComponent>(entity));
    }
    if (coordinator_->HasComponent<AnimationComponent>(entity)) {
      UploadComponent(out_stream, dungeon,
          coordinator_->GetComponent<AnimationComponent>(entity));
    }
  }

  out_stream.close();
  if (!out_stream.good()) {
    std::cerr << "Error occurred at dungeon writing time!";
    exit(1);
  }
}

void Serializer::RemoveDungeon(DungeonName dungeon_name) {
  auto& removing_dungeon = dungeons_.at(dungeon_name);
  for (engine::Entity entity : removing_dungeon->entities) {
    coordinator_->DestroyEntity(entity);
  }
  dungeons_.erase(dungeon_name);
}

void Serializer::DownloadDungeonFromJson(DungeonName dungeon_name) {
  QFile input_file(source_by_name_hand_created_.at(dungeon_name));
  input_file.open(QFile::ReadOnly | QIODevice::Text);
  QByteArray bytes = input_file.readAll();
  QJsonDocument document = QJsonDocument::fromJson(bytes);

  dungeons_.insert({dungeon_name, std::make_unique<Dungeon>()});
  auto& new_dungeon = dungeons_.at(dungeon_name);

  new_dungeon->offset_x = document["offset_x"].toInt();
  new_dungeon->offset_y = document["offset_y"].toInt();
  QJsonArray entities = document["entities"].toArray();
  new_dungeon->entities_count = entities.size();

  for (auto entity : entities) {
    QJsonObject entity_object{entity.toObject()};
    engine::Entity new_entity = coordinator_->CreateEntity();
    new_dungeon->entities.push_back(new_entity);

    if (entity_object.contains("position_comp")) {  // position component
      QJsonObject position_comp_object{
          entity_object["position_comp"].toObject()};
      PositionComponent position_component{
          {static_cast<float>(position_comp_object["column"].toInt()
          * kTextureSize), static_cast<float>(
          position_comp_object["row"].toInt() * kTextureSize)}};
      coordinator_->AddComponent(new_entity, position_component);
    }

    if (entity_object.contains("graphics_comp")) {  // graphics item component
      QJsonObject graphics_comp_object{
          entity_object["graphics_comp"].toObject()};
      QString source_name{graphics_comp_object["source"].toString()};
      auto item = scene_->GetScene()->addPixmap(QPixmap(source_name));
      item->setZValue(graphics_comp_object["z_value"].toInt());
      GraphicsItemComponent graphics_item_component{
          item, source_name.toStdString()};
      coordinator_->AddComponent(new_entity, graphics_item_component);
    }

    if (entity_object.contains("movement_comp")) {  // movement component
      QJsonObject movement_comp_object{
          entity_object["movement_comp"].toObject()};
      MovementComponent movement_component{
          {static_cast<float>(movement_comp_object["direction_x"].toInt()),
           static_cast<float>(movement_comp_object["direction_y"].toInt())},
          static_cast<float>(movement_comp_object["current_speed"].toInt())};
      coordinator_->AddComponent(new_entity, movement_component);
    }

    if (entity_object.contains("animation_comp")) {  // animation component
      QJsonObject animation_comp_object{
          entity_object["animation_comp"].toObject()};
      QString source_name{animation_comp_object["source"].toString()};
      HorizontalDirection direction{static_cast<HorizontalDirection>(
          animation_comp_object["direction"].toInt())};
      MovementType move_type{static_cast<MovementType>(
          animation_comp_object["move_type"].toInt())};
      coordinator_->AddComponent(new_entity,
          AnimationComponent{AnimationPack(source_name.toStdString()),
                             source_name.toStdString(),
                             direction,
                             move_type});
    }
  }
}

//----------- Default Component Download/Upload --------------------------------

template<typename ComponentType>
ComponentType Serializer::DownloadComponent(
    std::ifstream& stream,
    const std::unique_ptr<Dungeon>& dungeon) {
  ComponentType component;
  Read(stream, &component, sizeof(ComponentType));
  return component;
}

template<typename ComponentType>
void Serializer::UploadComponent(std::ofstream& stream,
                                 const std::unique_ptr<Dungeon>& dungeon,
                                 const ComponentType& component) {
  Write(stream, &component, sizeof(ComponentType));
}

//----------- Position Component Specialization --------------------------------

template<>
PositionComponent Serializer::DownloadComponent<PositionComponent>(
    std::ifstream& stream,
    const std::unique_ptr<Dungeon>& dungeon) {
  float x;
  float y;
  Read(stream, &x, sizeof(float));
  Read(stream, &y, sizeof(float));
  return PositionComponent{{x + dungeon->offset_x, y + dungeon->offset_y}};
}

template<>
void Serializer::UploadComponent<PositionComponent>(
    std::ofstream& stream,
    const std::unique_ptr<Dungeon>& dungeon,
    const PositionComponent& component) {
  float x{component.position.x() - dungeon->offset_x};
  float y{component.position.y() - dungeon->offset_y};
  Write(stream, &x, sizeof(float));
  Write(stream, &y, sizeof(float));
}


//----------- Graphics Item Component Specialization ---------------------------

template<>
GraphicsItemComponent Serializer::DownloadComponent<GraphicsItemComponent>(
    std::ifstream& stream,
    const std::unique_ptr<Dungeon>&) {
  char source_name[kMaxPathLength];
  int z_value;
  Read(stream, &source_name, kMaxPathLength);
  Read(stream, &z_value, sizeof(int));

  auto item = scene_->GetScene()->addPixmap(QPixmap(source_name));
  item->setZValue(z_value);
  GraphicsItemComponent component{item, source_name};

  return component;
}

template<>
void Serializer::UploadComponent<GraphicsItemComponent>(
    std::ofstream& stream,
    const std::unique_ptr<Dungeon>&,
    const GraphicsItemComponent& component) {
  int z_value{static_cast<int>(component.item->zValue())};
  Write(stream, component.source_name.data(), kMaxPathLength);
  Write(stream, &z_value, sizeof(int));
}


//----------- Animation Component Specialization ---------------------------

template<>
AnimationComponent Serializer::DownloadComponent<AnimationComponent>(
    std::ifstream& stream,
    const std::unique_ptr<Dungeon>&) {
  char source_name[kMaxPathLength];
  int direction;
  int move_type;
  Read(stream, &source_name, kMaxPathLength);
  Read(stream, &direction, sizeof(int));
  Read(stream, &move_type, sizeof(int));
  return AnimationComponent{AnimationPack(source_name), source_name,
      static_cast<HorizontalDirection>(direction),
      static_cast<MovementType>(move_type)};
}

template<>
void Serializer::UploadComponent<AnimationComponent>(
    std::ofstream& stream,
    const std::unique_ptr<Dungeon>&,
    const AnimationComponent& component) {
  int direction{static_cast<int>(component.direction)};
  int move_type{static_cast<int>(component.move_type)};
  Write(stream, component.source_name.data(), kMaxPathLength);
  Write(stream, &direction, sizeof(int));
  Write(stream, &move_type, sizeof(int));
}

}  // namespace core
