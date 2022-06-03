#include "connector.h"

#include <memory>

#include "constants.h"
#include "components.h"
#include "engine/coordinator.h"
#include "systems/joystick_system.h"
#include "systems/movement_system.h"
#include "systems/painting_system.h"
#include "systems/animation_system.h"
#include "systems/collision_system.h"
#include "systems/illness_system.h"
#include "core/location_manager/location_manager.h"

namespace core {

Connector::Connector() : coordinator_(std::make_unique<engine::Coordinator>()),
                         keyboard_(std::make_unique<Keyboard>()),
                         current_dungeon_(DungeonName::kHub),
                         location_manager_(
                             std::make_unique<LocationManager>()) {
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
  // TODO(someone): show use event
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
}

// example of interacting with engine
void Connector::StartGame(Scene* scene) {
  serializer_ =
      std::make_unique<Serializer>(coordinator_.get(), scene);

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
  // TODO(someone) : check if its a coin then add
}

std::shared_ptr<engine::Coordinator> Connector::GetCoordinator() {
  return coordinator_;
}

DungeonName Connector::GetCurrentDungeon() {
  return current_dungeon_;
}

void Connector::SetCurrentDungeon(DungeonName dungeon) {
  current_dungeon_ = dungeon;
}

Serializer* Connector::GetSerializer() {
  return serializer_.get();
}

}  // namespace core
