#include "connector.h"

#include <memory>

#include "constants.h"
#include "components.h"
#include "engine/coordinator.h"
#include "systems/joystick_system.h"
#include "systems/movement_system.h"
#include "systems/painting_system.h"

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
            ({coordinator_->GetComponentID<MovementComponent>()});

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
}

void Connector::RegisterComponents() {
  coordinator_->RegisterComponent<PositionComponent>();
  coordinator_->RegisterComponent<MovementComponent>();
  coordinator_->RegisterComponent<GraphicsItemComponent>();
}

QGraphicsItem* Connector::CreateHero(Scene* scene) {
  engine::Entity hero = coordinator_->CreateEntity();
  coordinator_->AddComponent(hero, PositionComponent{{0, 0}});
  coordinator_->AddComponent(hero, MovementComponent{{0, 0}, 1});
  auto item = scene->GetScene()->addPixmap(QPixmap(":fox.png"));
  // z value for hero
  item->setZValue(kPlayerZIndex);
  coordinator_->AddComponent(hero, GraphicsItemComponent{item, ":fox.png"});
  return item;
}

// example of interacting with engine
void Connector::StartGame(Scene* scene) {
  serializer_ = std::make_unique<Serializer>(coordinator_.get(), scene);
  // serializer_->DownloadDungeonFromJson(DungeonName::kDefaultHub);
  // serializer_->UploadDungeon(DungeonName::kEditedHub);
  serializer_->DownloadDungeon(DungeonName::kEditedHub);
  // const int test_scene_size_ = 2;
  // for (int i = -test_scene_size_; i <= test_scene_size_; ++i) {
  //   for (int j = -test_scene_size_; j <= test_scene_size_; ++j) {
  //     engine::Entity entity = coordinator_->CreateEntity();
  //     float x = i * core::kTextureSize;
  //     float y = j * core::kTextureSize;
  //     coordinator_->AddComponent(entity, PositionComponent{{x, y}});
  //     auto item = scene->GetScene()->addPixmap(QPixmap(":ground1.jpg"));
  //     item->setZValue(kBackgroundZIndex);  // z value for background
  //     coordinator_->AddComponent(entity, GraphicsItemComponent{
  //         item, ":ground1.jpg"});
  //   }
  // }
}

}  // namespace core
