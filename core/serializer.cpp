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

void Serializer::RemoveEntityFromScene(engine::Entity entity) {
  if (coordinator_->HasComponent<GraphicsItemComponent>(entity)) {
    scene_->GetScene()->removeItem(
        coordinator_->GetComponent<GraphicsItemComponent>(entity).item);
  }
}

void Serializer::DeleteEntity(engine::Entity entity) {
  for (auto& [dungeon_name, dungeon] : dungeons_) {
    auto it = std::find(
        dungeon->entities.begin(), dungeon->entities.end(), entity);
    if (it != dungeon->entities.end()) {
      dungeon->entities.erase(it);
    }
  }
  RemoveEntityFromScene(entity);
  coordinator_->DestroyEntity(entity);
}

void Serializer::DownloadDungeon(
    DungeonName dungeon_name,
    DungeonType dungeon_type) {
  std::string file_name;
  switch (dungeon_type) {
    case DungeonType::kDefault: {
      file_name = source_by_name_default.at(dungeon_name);
      break;
    }
    case DungeonType::kEdited: {
      file_name = source_by_name_edited.at(dungeon_name);
      break;
    }
    case DungeonType::kHandCreated: {
      DownloadDungeonFromJson(dungeon_name);
      return;
    }
  }

  std::ifstream stream;
  stream.open(file_name, std::ios::binary | std::ios::in);
  assert(stream && "Cannot open dungeon download file!");

  dungeons_.insert({dungeon_name, std::make_unique<Dungeon>()});
  auto& dungeon = dungeons_.at(dungeon_name);
  Read(stream, &dungeon->offset_x, sizeof(dungeon->offset_x));
  Read(stream, &dungeon->offset_y, sizeof(dungeon->offset_y));
  Read(stream, &dungeon->entities_count, sizeof(dungeon->entities_count));

  for (int i = 0; i < dungeon->entities_count; i++) {
    engine::Entity entity = coordinator_->CreateEntity();
    dungeon->entities.push_back(entity);

    engine::ComponentSignature component_signature;
    Read(stream, &component_signature, sizeof(engine::ComponentSignature));
    coordinator_->SetComponentSignature(entity, component_signature);

    // Download component from file and add to entity, if entity should
    // have this component according to it's signature
    DownloadCompIfNecessary<PositionComponent>(entity, dungeon, stream);
    DownloadCompIfNecessary<GraphicsItemComponent>(entity, dungeon, stream);
    DownloadCompIfNecessary<MovementComponent>(entity, dungeon, stream);
    DownloadCompIfNecessary<AnimationComponent>(entity, dungeon, stream);
  }

  stream.close();
  assert(stream.good() && "Error occurred at dungeon reading time!");
}

template<typename ComponentType>
void Serializer::DownloadCompIfNecessary(
    engine::Entity entity,
    const std::unique_ptr<Dungeon>& dungeon,
    std::ifstream& stream) {
  if (coordinator_->HasComponent<ComponentType>(entity)) {
    coordinator_->AddComponent(
        entity,
        DownloadComponent<ComponentType>(stream, dungeon));
  }
}

void Serializer::UploadDungeon(
    DungeonName dungeon_name,
    DungeonType dungeon_type) {
  std::string file_name;
  switch (dungeon_type) {
    case DungeonType::kDefault:
    case DungeonType::kEdited: {
      file_name = source_by_name_edited.at(dungeon_name);
      break;
    }
    case DungeonType::kHandCreated: {
      file_name = source_by_name_default.at(dungeon_name);
      break;
    }
  }

  std::ofstream stream;
  stream.open(file_name, std::ios::binary | std::ios::out);
  assert(stream && "Cannot open dungeon upload file!");

  auto& dungeon = dungeons_.at(dungeon_name);
  Write(stream, &dungeon->offset_x, sizeof(dungeon->offset_x));
  Write(stream, &dungeon->offset_y, sizeof(dungeon->offset_y));
  Write(stream, &dungeon->entities_count, sizeof(dungeon->entities_count));

  for (auto entity : dungeon->entities) {
    engine::ComponentSignature component_signature{
        coordinator_->GetComponentSignature(entity)};
    Write(stream, &component_signature, sizeof(engine::ComponentSignature));

    // Upload component of entity to file, if entity have this component
    // according to it's signature
    UploadCompIfNecessary<PositionComponent>(entity, dungeon, stream);
    UploadCompIfNecessary<GraphicsItemComponent>(entity, dungeon, stream);
    UploadCompIfNecessary<MovementComponent>(entity, dungeon, stream);
    UploadCompIfNecessary<AnimationComponent>(entity, dungeon, stream);
  }

  stream.close();
  assert(stream.good() && "Error occurred at dungeon writing time!");
}

template<typename ComponentType>
void Serializer::UploadCompIfNecessary(
    engine::Entity entity,
    const std::unique_ptr<Dungeon>& dungeon,
    std::ofstream& stream) {
  if (coordinator_->HasComponent<ComponentType>(entity)) {
    UploadComponent(stream, dungeon,
                    coordinator_->GetComponent<ComponentType>(entity));
  }
}

void Serializer::RemoveDungeon(DungeonName dungeon_name) {
  auto& removing_dungeon = dungeons_.at(dungeon_name);
  for (engine::Entity entity : removing_dungeon->entities) {
    RemoveEntityFromScene(entity);
    coordinator_->DestroyEntity(entity);
  }
  dungeons_.erase(dungeon_name);
}

void Serializer::DownloadDungeonFromJson(DungeonName dungeon_name) {
  QFile input_file(source_by_name_hand_created.at(dungeon_name));
  input_file.open(QFile::ReadOnly | QIODevice::Text);
  QByteArray bytes = input_file.readAll();
  QJsonDocument document = QJsonDocument::fromJson(bytes);

  dungeons_.insert({dungeon_name, std::make_unique<Dungeon>()});
  auto& dungeon = dungeons_.at(dungeon_name);

  dungeon->offset_x = document["offset_x"].toInt();
  dungeon->offset_y = document["offset_y"].toInt();
  QJsonArray entities_data = document["entities"].toArray();
  dungeon->entities_count = entities_data.size();

  for (auto entity_data : entities_data) {
    QJsonObject entity_object{entity_data.toObject()};
    engine::Entity entity = coordinator_->CreateEntity();
    dungeon->entities.push_back(entity);

    // Download component from json file if entity_object contains such
    // component data and add to entity
    DownloadCompFromJson<PositionComponent>(
        entity, dungeon, entity_object);
    DownloadCompFromJson<GraphicsItemComponent>(
        entity, dungeon, entity_object);
    DownloadCompFromJson<MovementComponent>(
        entity, dungeon, entity_object);
    DownloadCompFromJson<AnimationComponent>(
        entity, dungeon, entity_object);
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
void Serializer::DownloadCompFromJson<PositionComponent>(
    engine::Entity entity,
    const std::unique_ptr<Dungeon>& dungeon,
    const QJsonObject& entity_object) {
  if (entity_object.contains("position_comp")) {
    QJsonObject position_comp_object{entity_object["position_comp"].toObject()};
    PositionComponent position_component{{
        static_cast<float>(position_comp_object["column"].toInt()
        * kTextureSize + dungeon->offset_x),
        static_cast<float>(position_comp_object["row"].toInt()
        * kTextureSize + dungeon->offset_y)}};
    coordinator_->AddComponent(entity, position_component);
  }
}

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
void Serializer::DownloadCompFromJson<GraphicsItemComponent>(
    engine::Entity entity,
    const std::unique_ptr<Dungeon>& dungeon,
    const QJsonObject& entity_object) {
  if (entity_object.contains("graphics_comp")) {
    QJsonObject graphics_comp_object{entity_object["graphics_comp"].toObject()};

    QString source_name{graphics_comp_object["source"].toString()};
    auto item = scene_->GetScene()->addPixmap(QPixmap(source_name));

    double scale_x{graphics_comp_object["scale_x"].toDouble()};
    double scale_y{graphics_comp_object["scale_y"].toDouble()};
    int rotate{graphics_comp_object["rotate"].toInt()};

    item->setZValue(graphics_comp_object["z_value"].toInt());
    item->setPixmap(item->pixmap().transformed(
        QTransform().scale(scale_x, scale_y)));
    item->setPixmap(item->pixmap().transformed(
        QTransform().rotate(rotate)));

    GraphicsItemComponent graphics_item_component{
        item, source_name.toStdString(), scale_x, scale_y, rotate};
    coordinator_->AddComponent(entity, graphics_item_component);
  }
}

template<>
GraphicsItemComponent Serializer::DownloadComponent<GraphicsItemComponent>(
    std::ifstream& stream,
    const std::unique_ptr<Dungeon>&) {
  char source_name[kMaxPathLength];
  int z_value;
  double scale_x, scale_y;
  int rotate;
  Read(stream, &source_name, kMaxPathLength);
  Read(stream, &z_value, sizeof(int));
  Read(stream, &scale_x, sizeof(double));
  Read(stream, &scale_y, sizeof(double));
  Read(stream, &rotate, sizeof(int));

  auto item = scene_->GetScene()->addPixmap(QPixmap(source_name));
  item->setZValue(z_value);
  item->setPixmap(item->pixmap().transformed(
      QTransform().scale(scale_x, scale_y)));
  item->setPixmap(item->pixmap().transformed(
      QTransform().rotate(rotate)));
  GraphicsItemComponent component{item, source_name, scale_x,
                                  scale_y, rotate};

  return component;
}

template<>
void Serializer::UploadComponent<GraphicsItemComponent>(
    std::ofstream& stream,
    const std::unique_ptr<Dungeon>&,
    const GraphicsItemComponent& component) {
  int z_value{static_cast<int>(component.item->zValue())};
  double scale_x{component.scale_x};
  double scale_y{component.scale_y};
  int rotate{component.rotate};
  Write(stream, component.source_name.data(), kMaxPathLength);
  Write(stream, &z_value, sizeof(int));
  Write(stream, &scale_x, sizeof(double));
  Write(stream, &scale_y, sizeof(double));
  Write(stream, &rotate, sizeof(int));
}

//----------- Movement Component Specialization ---------------------------
template<>
void Serializer::DownloadCompFromJson<MovementComponent>(
    engine::Entity entity,
    const std::unique_ptr<Dungeon>& dungeon,
    const QJsonObject& entity_object) {
  if (entity_object.contains("movement_comp")) {
    QJsonObject movement_comp_object{entity_object["movement_comp"].toObject()};
    MovementComponent movement_component{
        {static_cast<float>(movement_comp_object["direction_x"].toInt()),
         static_cast<float>(movement_comp_object["direction_y"].toInt())},
        static_cast<float>(movement_comp_object["current_speed"].toInt())};
    coordinator_->AddComponent(entity, movement_component);
  }
}

// Binary downloading and uploading of movement component is default

//----------- Animation Component Specialization ---------------------------
template<>
void Serializer::DownloadCompFromJson<AnimationComponent>(
    engine::Entity entity,
    const std::unique_ptr<Dungeon>& dungeon,
    const QJsonObject& entity_object) {
  if (entity_object.contains("animation_comp")) {
    QJsonObject animation_comp_object{
        entity_object["animation_comp"].toObject()};
    QString source_name{animation_comp_object["source"].toString()};
    HorizontalDirection direction{
        static_cast<HorizontalDirection>(
        animation_comp_object["direction"].toInt())};
    MovementType move_type{
        static_cast<MovementType>(animation_comp_object["move_type"].toInt())};
    coordinator_->AddComponent(entity, AnimationComponent{
        AnimationPack(source_name.toStdString()),
        source_name.toStdString(),
        direction,
        move_type});
  }
}

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
