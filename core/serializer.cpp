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

  in_stream.read((char*) &new_dungeon->offset_x, sizeof(new_dungeon->offset_x));
  in_stream.read((char*) &new_dungeon->offset_y, sizeof(new_dungeon->offset_y));
  in_stream.read((char*) &new_dungeon->entities_count,
                 sizeof(new_dungeon->entities_count));

  for (int i = 0; i < new_dungeon->entities_count; i++) {
    engine::Entity new_entity = coordinator_->CreateEntity();
    qDebug() << "create entity №" << new_entity;
    new_dungeon->entities.push_back(new_entity);

    engine::ComponentSignature component_signature;
    in_stream.read((char *) &component_signature,
                   sizeof(engine::ComponentSignature));
    coordinator_->SetComponentSignature(new_entity, component_signature);

    // std::cout << "down_cs " << component_signature.to_string() << '\n';


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
  }

  in_stream.close();
  if(!in_stream.good()) {
    std::cerr << "Error occurred at dungeon reading time!";
    exit(1);
  }

  // qDebug() << "Download finished";
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

  out_stream.write((char*) &dungeon->offset_x, sizeof(dungeon->offset_x));
  out_stream.write((char*) &dungeon->offset_y, sizeof(dungeon->offset_y));
  out_stream.write((char*) &dungeon->entities_count,
                   sizeof(dungeon->entities_count));
  // std::cout << "up_xycount " << dungeon->offset_x << ' ' << dungeon->offset_y << ' '
  // << dungeon->entities_count << '\n';

  for (auto entity : dungeon->entities) {
    engine::ComponentSignature component_signature{
        coordinator_->GetComponentSignature(entity)};
    out_stream.write((char*) &component_signature,
        sizeof(engine::ComponentSignature));

    // std::string temp = component_signature.to_string();
    // std::cout << "up_cs " << component_signature.to_string() << '\n';

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
  }

  out_stream.close();
  if(!out_stream.good()) {
    std::cerr << "Error occurred at dungeon writing time!";
    exit(1);
  }

  // qDebug() << "Upload finished";
}

void Serializer::RemoveDungeon(DungeonName dungeon_name) {
  auto& removing_dungeon = dungeons_.at(dungeon_name);
  for (engine::Entity entity : removing_dungeon->entities) {
    coordinator_->DestroyEntity(entity);
    qDebug() << "delete entity №" << entity;
  }
  dungeons_.erase(dungeon_name);
}

void Serializer::DownloadDungeonFromJson(DungeonName dungeon_name) {
  // qDebug() << "Download from json" << static_cast<int>(dungeon_name);

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
  }
}

//----------- Default Component Download/Upload --------------------------------

template<typename ComponentType>
ComponentType Serializer::DownloadComponent(
    std::ifstream& stream,
    const std::unique_ptr<Dungeon>& dungeon) {
  // std::cout << "doun_default_comp\n";
  ComponentType component;
  stream.read((char*) &component, sizeof(ComponentType));
  return component;
}

template<typename ComponentType>
void Serializer::UploadComponent(std::ofstream& stream,
                                 const std::unique_ptr<Dungeon>& dungeon,
                                 const ComponentType& component) {
  // std::cout << "up_default_comp\n";
  stream.write((char*) &component, sizeof(ComponentType));
}

//----------- Position Component Specialization --------------------------------

template<>
PositionComponent Serializer::DownloadComponent<PositionComponent>(
    std::ifstream& stream,
    const std::unique_ptr<Dungeon>& dungeon) {
  // PositionComponent position_component;
  // stream.read((char*) &position_component, sizeof(PositionComponent));
  // std::cout << "down_pos: " << position_component.position.x() << ' '
  // << position_component.position.y() << '\n';
  // position_component.position.setX(
  //     position_component.position.x() + dungeon->offset_x);
  // position_component.position.setY(
  //     position_component.position.y() + dungeon->offset_y);
  // return position_component;

  float x;
  float y;
  stream.read((char*) &x, sizeof(float ));
  stream.read((char*) &y, sizeof(float ));
  // std::cout << "down_pos: " << x << ' ' << y << '\n';
  return PositionComponent{{x + dungeon->offset_x, y + dungeon->offset_y}};
}

template<>
void Serializer::UploadComponent<PositionComponent>(
    std::ofstream& stream,
    const std::unique_ptr<Dungeon>& dungeon,
    const PositionComponent& component) {
  // PositionComponent position_component{
  //     {component.position.x() - dungeon->offset_x,
  //      component.position.y() - dungeon->offset_y}};
  // stream.write((char*) &position_component, sizeof(PositionComponent));
  // std::cout << "up_pos: " << position_component.position.x() << ' '
  // << position_component.position.y() << '\n';
  float x{component.position.x() - dungeon->offset_x};
  float y{component.position.y() - dungeon->offset_y};
  stream.write((char*) &x, sizeof(float ));
  stream.write((char*) &y, sizeof(float ));
  // std::cout << "up_pos: " << x << ' ' << y << '\n';
}


//----------- Graphics Item Component Specialization ---------------------------

template<>
GraphicsItemComponent Serializer::DownloadComponent<GraphicsItemComponent>(
    std::ifstream& stream,
    const std::unique_ptr<Dungeon>&) {
  char source_name[kMaxPathLength];
  int z_value;
  stream.read((char*) &source_name, kMaxPathLength);
  stream.read((char*) &z_value, sizeof(int));

  auto item = scene_->GetScene()->addPixmap(QPixmap(source_name));
  item->setZValue(z_value);
  GraphicsItemComponent component{item, source_name};

  // std::cout << "down_graph " << source_name << ' ' << z_value << '\n';

  return component;
}

template<>
void Serializer::UploadComponent<GraphicsItemComponent>(
    std::ofstream& stream,
    const std::unique_ptr<Dungeon>&,
    const GraphicsItemComponent& component) {
  // const char* file_name{component.source_name.data()};
  int z_value{static_cast<int>(component.item->zValue())};
  stream.write((char*) component.source_name.data(), kMaxPathLength);
  stream.write((char*) &z_value, sizeof(int));
  // std::cout << "up_graph " << component.source_name.data() << ' ' << z_value << '\n';
}

} // core
