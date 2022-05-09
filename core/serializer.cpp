//
// Created by marmotikon on 2.5.22.
//

#include <istream>
#include <fstream>
#include "serializer.h"
#include "components.h"

namespace core {


Serializer::Serializer(engine::Coordinator* coordinator, Scene* scene) :
    coordinator_(coordinator), scene_(scene) {}

PositionComponent Serializer::DownloadPositionComponent(
    std::ifstream& stream, const std::unique_ptr<Dungeon>& dungeon) {
  float x;
  float y;
  stream >> x >> y;
  return PositionComponent{{x + dungeon->offset_x, y + dungeon->offset_y}};
}

GraphicsItemComponent Serializer::DownloadGraphicsItemComponent(
    std::ifstream& stream, const std::unique_ptr<Dungeon>&) {
  char* source_name = nullptr;
  int z_value;
  stream >> source_name >> z_value;
  auto item = scene_->GetScene()->addPixmap(QPixmap(source_name));
  item->setZValue(z_value);
  return GraphicsItemComponent{item};
}

MovementComponent Serializer::DownloadMovementComponent(
    std::ifstream& stream, const std::unique_ptr<Dungeon>&) {
  float direction_x;
  float direction_y;
  float current_speed;
  stream >> direction_x >> direction_y >> current_speed;
  return MovementComponent{{direction_x, direction_y}, current_speed};
}

void Serializer::DownloadDungeon(DungeonName dungeon_name) {
  std::string filename(source_by_name_.at(dungeon_name));
  std::ifstream in_stream(filename, std::ios::binary);

  dungeons_.insert({dungeon_name, std::make_unique<Dungeon>()});
  auto& new_dungeon = dungeons_.at(dungeon_name);

  in_stream >> new_dungeon->offset_x >> new_dungeon->offset_y
      >> new_dungeon->entities_count;

  for (int i = 0; i < new_dungeon->entities_count; i++) {
    engine::Entity new_entity = coordinator_->CreateEntity();
    new_dungeon->entities.push_back(new_entity);

    engine::ComponentSignature component_signature;
    in_stream >> component_signature;
    if (component_signature[0]) {  // position component
      coordinator_->AddComponent(
          new_entity, DownloadPositionComponent(in_stream, new_dungeon));
    }
    if (component_signature[1]) {  // graphics item component
      coordinator_->AddComponent(
          new_entity, DownloadGraphicsItemComponent(in_stream, new_dungeon));
    }
    if (component_signature[2]) {  // movement component
      coordinator_->AddComponent(
          new_entity, DownloadMovementComponent(in_stream, new_dungeon));
    }
  }
}

} // core