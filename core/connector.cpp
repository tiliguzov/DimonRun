#include "connector.h"

#include <memory>

#include "constants.h"
#include "components.h"
#include "engine/coordinator.h"
#include "systems/joystick_system.h"
#include "systems/movement_system.h"
#include "systems/painting_system.h"
#include "systems/animation_system.h"

namespace core {

Connector::Connector() : coordinator_(std::make_unique<engine::Coordinator>()),
                         keyboard_(std::make_unique<Keyboard>()) {
  RegisterComponents();
  RegisterSystems();
}

void Connector::OnTick() {
  for (const auto& system : systems_) {
    system->Update();
  }
}

void Connector::OnKeyPress(Qt::Key key) {
    keyboard_->OnPress(key);
}

void Connector::OnKeyRelease(Qt::Key key) {
    keyboard_->OnRelease(key);
}

void Connector::RegisterSystems() {
    {
        auto joystick_system_ = coordinator_
            ->RegisterSystem<systems::JoystickSystem>(
                    coordinator_.get(), keyboard_.get());

        coordinator_->SetSystemSignature<systems::JoystickSystem>
            ({coordinator_->GetComponentID<MovementComponent>(),
              coordinator_->GetComponentID<AnimationComponent>()});

        systems_.push_back(joystick_system_);
    }
    {
        auto movement_system = coordinator_
            ->RegisterSystem<systems::MovementSystem>(coordinator_.get());
        coordinator_->SetSystemSignature<systems::MovementSystem>
            ({coordinator_->GetComponentID<PositionComponent>(),
              coordinator_->GetComponentID<MovementComponent>()});

        systems_.push_back(movement_system);
    }
    {
        auto painting_system = coordinator_
            ->RegisterSystem<systems::PaintingSystem>(coordinator_.get());
        coordinator_->SetSystemSignature<systems::PaintingSystem>
            ({coordinator_->GetComponentID<PositionComponent>(),
              coordinator_->GetComponentID<GraphicsItemComponent>()});
        systems_.push_back(painting_system);
    }
    {
        auto animation_system = coordinator_
            ->RegisterSystem<systems::AnimationSystem>(coordinator_.get());
        coordinator_->SetSystemSignature<systems::AnimationSystem>
            ({coordinator_->GetComponentID<AnimationComponent>(),
              coordinator_->GetComponentID<GraphicsItemComponent>()});
        systems_.push_back(animation_system);
    }
}

void Connector::RegisterComponents() {
  coordinator_->RegisterComponent<PositionComponent>();
  coordinator_->RegisterComponent<MovementComponent>();
  coordinator_->RegisterComponent<GraphicsItemComponent>();
  coordinator_->RegisterComponent<AnimationComponent>();
}

QGraphicsItem* Connector::CreateHero(Scene* scene) {
  engine::Entity hero = coordinator_->CreateEntity();
  coordinator_->AddComponent(hero, PositionComponent{{50, 50}});
  coordinator_->AddComponent(hero, MovementComponent{{0, 0}, 1});
  auto item = scene->GetScene()->addPixmap(
      QPixmap(":Hero_static_in_air_00.png"));
  item->setZValue(kPlayerZIndex);
  coordinator_->AddComponent(hero, GraphicsItemComponent{item,
                  ":Hero_static_in_air_00.png", 1, 1, 0});
  coordinator_->AddComponent(
      hero,
      AnimationComponent{AnimationPack(":hero.json"),
                         ":hero.json",
                         HorizontalDirection::kRight,
                         MovementType::kStaticInAir});
  return item;
}

// example of interacting with engine
void Connector::StartGame(Scene* scene) {
  serializer_ = std::make_unique<Serializer>(coordinator_.get(), scene);

  // [before game release] Download from json file to game
  serializer_->DownloadDungeon(DungeonName::kHub, DungeonType::kHandCreated);

  // [before game release] Upload dungeon from game to default binary file
  // serializer_->UploadDungeon(DungeonName::kHub, DungeonType::kHandCreated);

  // Download default dungeon from binary file to game
  // serializer_->DownloadDungeon(DungeonName::kHub, DungeonType::kDefault);

  // Download edited dungeon from binary file to game
  // serializer_->DownloadDungeon(DungeonName::kHub, DungeonType::kEdited);

  // Upload dungeon from game to binary file of edited dungeon
  // serializer_->UploadDungeon(DungeonName::kHub, DungeonType::kDefault);
  // serializer_->UploadDungeon(DungeonName::kHub, DungeonType::kEdited);

  // Removes dungeon from game
  // serializer_->RemoveDungeon(DungeonName::kHub);

  scene->GetSceneView()->scale(2.5, 2.5);
}

void Connector::DeleteEntity(engine::Entity entity) {
  serializer_->DeleteEntity(entity);
}

}  // namespace core
