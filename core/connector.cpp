#include "connector.h"

#include <memory>
#include <iostream>

#include "constants.h"
#include "components.h"
#include "engine/coordinator.h"
#include "systems/joystick_system.h"
#include "systems/movement_system.h"
#include "systems/painting_system.h"
#include "systems/animation_system.h"
#include "systems/collision_system.h"
#include "systems/illness_system.h"

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

void Connector::UseEvent(engine::Entity entity) {
  auto event_comp{coordinator_->GetComponent<EventComponent>(entity)};
  switch (event_comp.type) {
    case EventType::kSecretRoom: {
      auto event_data{secret_rooms_data.at(event_comp.number)};
      if (current_dungeons_.count(event_data.name) != 0) {
        break;
      }
      current_dungeons_.insert(event_data.name);
      serializer_->DownloadDungeon(event_data.name, DungeonType::kDefault);
      break;
    }
    case EventType::kNextDungeon: {
      auto event_data{new_dungeon_chests_data.at(event_comp.number)};
      location_manager_->UnlockLocation(event_data.name);
      scene_->OpenScroll(event_data.scroll_massage);
      break;
    }
    case EventType::kProgressReset: {
      OpenNewDungeon(DungeonName::kHub);
      location_manager_->ResetProgress();
    }
    default: {
      std::cout << "strange event\n";
    }
  }
}

void Connector::RegisterSystems() {
  {
    auto joystick_system_ = coordinator_
        ->RegisterSystem<systems::JoystickSystem>(
            coordinator_.get(), keyboard_.get());

    coordinator_->SetSystemSignature<systems::JoystickSystem>
        ({coordinator_->GetComponentID<MovementComponent>(),
          coordinator_->GetComponentID<AnimationComponent>(),
          coordinator_->GetComponentID<JoysticComponent>()});

    systems_.push_back(joystick_system_);
  }
  {
    auto illness_system_ = coordinator_
        ->RegisterSystem<systems::IllnessSystem>(
            coordinator_.get(), this);

    coordinator_->SetSystemSignature<systems::IllnessSystem>
        ({coordinator_->GetComponentID<IllnessComponent>()});

    systems_.push_back(illness_system_);
  }
  {
    auto collision_system = coordinator_
        ->RegisterSystem<systems::CollisionSystem>(coordinator_.get(), this);
    coordinator_->SetSystemSignature<systems::CollisionSystem>
        ({coordinator_->GetComponentID<PositionComponent>(),
          coordinator_->GetComponentID<GraphicsItemComponent>(),
          coordinator_->GetComponentID<CollisionComponent>()});
    systems_.push_back(collision_system);
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
  coordinator_->RegisterComponent<CollisionComponent>();
  coordinator_->RegisterComponent<JoysticComponent>();
  coordinator_->RegisterComponent<IllnessComponent>();
  coordinator_->RegisterComponent<GraphicsItemComponent>();
  coordinator_->RegisterComponent<AnimationComponent>();
  coordinator_->RegisterComponent<EventComponent>();
  coordinator_->RegisterComponent<CoinComponent>();
}

// example of interacting with engine
void Connector::StartGame(Scene* scene) {
  scene_ = scene;
  serializer_ = std::make_unique<Serializer>(coordinator_.get(), scene);
  current_dungeons_.insert(DungeonName::kHub);


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
}

void Connector::DeleteEntity(engine::Entity entity) {
  serializer_->DeleteEntity(entity);
}

void Connector::CheckAndAddCoin(engine::Entity entity) {
  if (coordinator_->HasComponent<CoinComponent>(entity)) {
    location_manager_->AddCoins(
        coordinator_->GetComponent<CoinComponent>(entity).value);
  }
}

std::shared_ptr<engine::Coordinator> Connector::GetCoordinator() {
  return coordinator_;
}

void Connector::OpenNewDungeon(DungeonName dungeon_name) {
  // Remove all open dungeons
  for (auto current_dungeon : current_dungeons_) {
    // serializer_->UploadDungeon()  - if we want to save edited dungeons
    serializer_->RemoveDungeon(current_dungeon);
  }
  current_dungeons_.clear();

  // Add new dungeon
  current_dungeons_.insert(dungeon_name);
  serializer_->DownloadDungeon(dungeon_name, DungeonType::kDefault);
}

}  // namespace core
