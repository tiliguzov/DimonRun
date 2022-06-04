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

template<typename T>
void Write(std::ofstream& os, T* data, int size) {
  os.write(reinterpret_cast<const char*>(data), size);
}

}  // namespace

Serializer::Serializer(engine::Coordinator* coordinator, Scene* scene) :
    coordinator_(coordinator),
    graphics_scene_(scene->GetScene()),
    scene_(scene) {}

void Serializer::RemoveEntityFromScene(engine::Entity entity) {
  if (coordinator_->HasComponent<GraphicsItemComponent>(entity)) {
    graphics_scene_->removeItem(
        coordinator_->GetComponent<GraphicsItemComponent>(entity).item);
  }
}

void Serializer::DeleteEntity(engine::Entity entity) {
  for (auto&[dungeon_name, dungeon] : dungeons_) {
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
  Read(stream, &dungeon->background_image, kMaxPathLength);
  Read(stream, &dungeon->entities_count, sizeof(dungeon->entities_count));

  auto* item = new QGraphicsPixmapItem(QPixmap(QString(
      dungeon->background_image)));

  graphics_scene_->addItem(item);
  scene_->SetBackgroundImage(item);

  for (int i = 0; i < dungeon->entities_count; i++) {
    engine::Entity entity = coordinator_->CreateEntity();
    dungeon->entities.push_back(entity);
    engine::ComponentSignature component_signature;
    Read(stream, &component_signature, sizeof(engine::ComponentSignature));
    coordinator_->SetComponentSignature(entity, component_signature);
    if (coordinator_->HasComponent<JoysticComponent>(entity)) {
      scene_->SetHeroEntity(entity);
    }
    // Download component from file and add to entity, if entity should
    // have this component according to it's signature
    DownloadCompIfNecessary<PositionComponent>(entity, dungeon, stream);
    DownloadCompIfNecessary<GraphicsItemComponent>(entity, dungeon, stream);
    DownloadCompIfNecessary<MovementComponent>(entity, dungeon, stream);
    DownloadCompIfNecessary<AnimationComponent>(entity, dungeon, stream);
    DownloadCompIfNecessary<CollisionComponent>(entity, dungeon, stream);
    DownloadCompIfNecessary<IllnessComponent>(entity, dungeon, stream);
    DownloadCompIfNecessary<JoysticComponent>(entity, dungeon, stream);
    DownloadCompIfNecessary<CoinComponent>(entity, dungeon, stream);
    DownloadCompIfNecessary<EventComponent>(entity, dungeon, stream);
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
  Write(stream, &dungeon->background_image, kMaxPathLength);
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
    UploadCompIfNecessary<CollisionComponent>(entity, dungeon, stream);
    UploadCompIfNecessary<IllnessComponent>(entity, dungeon, stream);
    UploadCompIfNecessary<JoysticComponent>(entity, dungeon, stream);
    UploadCompIfNecessary<CoinComponent>(entity, dungeon, stream);
    UploadCompIfNecessary<EventComponent>(entity, dungeon, stream);
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
  std::string backgroung_str =
      document["background_image"].toString().toStdString();
  std::strcpy(dungeon->background_image, backgroung_str.c_str());
  auto* item = new QGraphicsPixmapItem(QPixmap(QString(
      dungeon->background_image)));


  graphics_scene_->addItem(item);
  scene_->SetBackgroundImage(item);

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
    DownloadCompFromJson<CollisionComponent>(
        entity, dungeon, entity_object);
    DownloadCompFromJson<IllnessComponent>(
        entity, dungeon, entity_object);
    DownloadCompFromJson<JoysticComponent>(
        entity, dungeon, entity_object);
    DownloadCompFromJson<CoinComponent>(
        entity, dungeon, entity_object);
    DownloadCompFromJson<EventComponent>(
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
    QJsonObject position_comp_object{
        entity_object["position_comp"].toObject()};
    PositionComponent position_component{{
      static_cast<float>(position_comp_object["column"].toInt()
      * kTextureSize + dungeon->offset_x),
      static_cast<float>(position_comp_object["row"].toInt()
      * kTextureSize + dungeon->offset_y)}
    };
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
    auto item = graphics_scene_->addPixmap(QPixmap(source_name));

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

  auto item = graphics_scene_->addPixmap(QPixmap(source_name));
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

//----------- Collision Component Specialization -------------------------------
template<>
void Serializer::DownloadCompFromJson<CollisionComponent>(
    engine::Entity entity,
    const std::unique_ptr<Dungeon>& dungeon,
    const QJsonObject& entity_object) {
  if (entity_object.contains("collision_comp")) {
    QJsonObject
        collision_comp_object{entity_object["collision_comp"].toObject()};
    bool is_movable
        {collision_comp_object["is_movable"].toBool()};
    bool gravity{collision_comp_object["gravity"].toBool()};
    bool can_use{collision_comp_object["can_use"].toBool()};
    bool is_usable
        {collision_comp_object["is_usable"].toBool()};
    bool is_breakable
        {collision_comp_object["is_breakable"].toBool()};
    CollisionComponent collision_component
        {is_movable, gravity, can_use, is_usable, is_breakable};
    coordinator_->AddComponent(entity, collision_component);
  }
}

template<>
CollisionComponent Serializer::DownloadComponent<CollisionComponent>(
    std::ifstream& stream,
    const std::unique_ptr<Dungeon>&) {
  bool is_movable;
  bool gravity;
  bool can_use;
  bool is_usable;
  bool is_breakable;
  Read(stream, &is_movable, sizeof(bool));
  Read(stream, &gravity, sizeof(bool));
  Read(stream, &can_use, sizeof(bool));
  Read(stream, &is_usable, sizeof(bool));
  Read(stream, &is_breakable, sizeof(bool));
  CollisionComponent
      component{is_movable, gravity, can_use, is_usable, is_breakable};
  return component;
}

template<>
void Serializer::UploadComponent<CollisionComponent>(
    std::ofstream& stream,
    const std::unique_ptr<Dungeon>&,
    const CollisionComponent& component) {
  bool is_movable{component.is_movable};
  bool gravity{component.gravity};
  bool can_use{component.can_use};
  bool is_usable{component.is_usable};
  bool is_breakable{component.is_breakable};
  Write(stream, &is_movable, sizeof(bool));
  Write(stream, &gravity, sizeof(bool));
  Write(stream, &can_use, sizeof(bool));
  Write(stream, &is_usable, sizeof(bool));
  Write(stream, &is_breakable, sizeof(bool));
}

//----------- Illness Component Specialization ---------------------------------
template<>
void Serializer::DownloadCompFromJson<IllnessComponent>(
    engine::Entity entity,
    const std::unique_ptr<Dungeon>& dungeon,
    const QJsonObject& entity_object) {
  if (entity_object.contains("illness_comp")) {
    QJsonObject
        collision_comp_object{entity_object["illness_comp"].toObject()};
    int kill_time{collision_comp_object["kill_time"].toInt()};
    bool is_ill{collision_comp_object["is_ill"].toBool()};
    assert(is_ill == 0 && "kill_time != 0");
    IllnessComponent illness_component{kill_time, is_ill};
    coordinator_->AddComponent(entity, illness_component);
  }
}

template<>
IllnessComponent Serializer::DownloadComponent<IllnessComponent>(
    std::ifstream& stream,
    const std::unique_ptr<Dungeon>&) {
  int kill_time;
  bool is_ill;
  Read(stream, &kill_time, sizeof(int));
  Read(stream, &is_ill, sizeof(bool));
  assert(is_ill == 0 && "is_ill != 0");
  IllnessComponent component{kill_time, is_ill};
  return component;
}

template<>
void Serializer::UploadComponent<IllnessComponent>(
    std::ofstream& stream,
    const std::unique_ptr<Dungeon>&,
    const IllnessComponent& component) {
  int kill_time{0};
  bool is_ill{0};
  Write(stream, &kill_time, sizeof(int));
  Write(stream, &is_ill, sizeof(bool));
}

//----------- Joystick Component Specialization --------------------------------
template<>
void Serializer::DownloadCompFromJson<JoysticComponent>(
    engine::Entity entity,
    const std::unique_ptr<Dungeon>& dungeon,
    const QJsonObject& entity_object) {
  if (entity_object.contains("joystick_comp")) {
    scene_->SetHeroEntity(entity);
    JoysticComponent joystick_component;
    coordinator_->AddComponent(entity, joystick_component);
  }
}

template<>
JoysticComponent Serializer::DownloadComponent<JoysticComponent>(
    std::ifstream& stream,
    const std::unique_ptr<Dungeon>&) {
  JoysticComponent component;
  return component;
}

template<>
void Serializer::UploadComponent<JoysticComponent>(
    std::ofstream& stream,
    const std::unique_ptr<Dungeon>&,
    const JoysticComponent& component) {
}

//----------- Movement Component Specialization --------------------------------
template<>
void Serializer::DownloadCompFromJson<MovementComponent>(
    engine::Entity entity,
    const std::unique_ptr<Dungeon>& dungeon,
    const QJsonObject& entity_object) {
  if (entity_object.contains("movement_comp")) {
    QJsonObject movement_comp_object{entity_object["movement_comp"].toObject()};
    MovementComponent movement_component{
        {static_cast<float>(movement_comp_object["direction_x"].toDouble()),
         static_cast<float>(movement_comp_object["direction_y"].toDouble())},
        static_cast<float>(movement_comp_object["current_speed"].toDouble())};
    coordinator_->AddComponent(entity, movement_component);
  }
}

template<>
MovementComponent Serializer::DownloadComponent<MovementComponent>(
    std::ifstream& stream,
    const std::unique_ptr<Dungeon>&) {
  float dir_x;
  float dir_y;
  float cur_speed;
  Read(stream, &dir_x, sizeof(float));
  Read(stream, &dir_y, sizeof(float));
  Read(stream, &cur_speed, sizeof(float));

  return MovementComponent{{static_cast<float>(dir_x),
                            static_cast<float>(dir_y)},
                           static_cast<float>(cur_speed)};
}

template<>
void Serializer::UploadComponent<MovementComponent>(
    std::ofstream& stream,
    const std::unique_ptr<Dungeon>&,
    const MovementComponent& component) {
  float dir_x{component.direction.x()};
  float dir_y{component.direction.x()};
  float cur_speed{component.current_speed};
  Write(stream, &dir_x, sizeof(float));
  Write(stream, &dir_y, sizeof(float));
  Write(stream, &cur_speed, sizeof(float));
}

//----------- Animation Component Specialization -------------------------------
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
    int start_time = animation_comp_object["start_time"].toInt();
    coordinator_->AddComponent(entity, AnimationComponent{
        AnimationPack(source_name.toStdString()),
        source_name.toStdString(),
        direction,
        move_type,
        start_time});
  }
}

template<>
AnimationComponent Serializer::DownloadComponent<AnimationComponent>(
    std::ifstream& stream,
    const std::unique_ptr<Dungeon>&) {
  char source_name[kMaxPathLength];
  int direction;
  int move_type;
  int start_time;
  Read(stream, &source_name, kMaxPathLength);
  Read(stream, &direction, sizeof(int));
  Read(stream, &move_type, sizeof(int));
  Read(stream, &start_time, sizeof(int));
  return AnimationComponent{AnimationPack(source_name), source_name,
                            static_cast<HorizontalDirection>(direction),
                            static_cast<MovementType>(move_type),
                            start_time};
}

template<>
void Serializer::UploadComponent<AnimationComponent>(
    std::ofstream& stream,
    const std::unique_ptr<Dungeon>&,
    const AnimationComponent& component) {
  int direction{static_cast<int>(component.direction)};
  int move_type{static_cast<int>(component.move_type)};
  int start_time{component.start_time};
  Write(stream, component.source_name.data(), kMaxPathLength);
  Write(stream, &direction, sizeof(int));
  Write(stream, &move_type, sizeof(int));
  Write(stream, &start_time, sizeof(int));
}

//----------- Coin Component Specialization ------------------------------------
template<>
void Serializer::DownloadCompFromJson<CoinComponent>(
    engine::Entity entity,
    const std::unique_ptr<Dungeon>&,
    const QJsonObject& entity_object) {
  if (entity_object.contains("coin_comp")) {
    QJsonObject coin_comp_object{
        entity_object["coin_comp"].toObject()};
    int value{coin_comp_object["value"].toInt()};

    coordinator_->AddComponent(entity, CoinComponent{value});
  }
}

template<>
CoinComponent Serializer::DownloadComponent<CoinComponent>(
    std::ifstream& stream,
    const std::unique_ptr<Dungeon>&) {
  int value;
  Read(stream, &value, sizeof(int));
  return CoinComponent{value};
}

template<>
void Serializer::UploadComponent<CoinComponent>(
    std::ofstream& stream,
    const std::unique_ptr<Dungeon>&,
    const CoinComponent& component) {
  Write(stream, &component.value, sizeof(int));
}

//----------- Event Component Specialization -----------------------------------
template<>
void Serializer::DownloadCompFromJson<EventComponent>(
    engine::Entity entity,
    const std::unique_ptr<Dungeon>&,
    const QJsonObject& entity_object) {
  if (entity_object.contains("event_comp")) {
    QJsonObject event_comp_object{
        entity_object["event_comp"].toObject()};
    EventType type{static_cast<EventType>(event_comp_object["type"].toInt())};
    int number{event_comp_object["number"].toInt()};

    coordinator_->AddComponent(entity, EventComponent{type, number});
  }
}

template<>
EventComponent Serializer::DownloadComponent<EventComponent>(
    std::ifstream& stream,
    const std::unique_ptr<Dungeon>&) {
  int type_index;
  int value;
  Read(stream, &type_index, sizeof(int));
  Read(stream, &value, sizeof(int));
  return EventComponent{static_cast<EventType>(type_index), value};
}

template<>
void Serializer::UploadComponent<EventComponent>(
    std::ofstream& stream,
    const std::unique_ptr<Dungeon>&,
    const EventComponent& component) {
  int type_index{static_cast<int>(component.type)};
  Write(stream, &type_index, sizeof(int));
  Write(stream, &component.number, sizeof(int));
}

}  // namespace core
