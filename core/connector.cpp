#include "connector.h"

#include <memory>
#include <iostream>

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
  // qDebug() << "create hero entity №" << hero;
  coordinator_->AddComponent(hero, PositionComponent{{150, 150}});
  coordinator_->AddComponent(hero, MovementComponent{{0, 0}, 1});
  auto item = scene->GetScene()->addPixmap(
      QPixmap(":textures/hero/Hero_static_in_air_00.png"));
  // z value for hero
  item->setZValue(kPlayerZIndex);
  coordinator_->AddComponent(hero, GraphicsItemComponent{item});
  coordinator_->AddComponent(hero,
          AnimationComponent{AnimationPack(":animations/hero.json"),
                             ":animations/hero.json"});
  return item;
}

// example of interacting with engine
void Connector::StartGame(Scene* scene) {
  serializer_ = std::make_unique<Serializer>(coordinator_.get(), scene);
  // qDebug() << "entities before hub download:" << coordinator_->GetEntityAliveCount();
  serializer_->DownloadDungeon(DungeonName::kHub, DungeonType::kHandCreated);
  // serializer_->UploadDungeon(DungeonName::kHub, DungeonType::kHandCreated);
  // serializer_->DownloadDungeon(DungeonName::kHub, DungeonType::kDefault);
  // qDebug() << "entities after hub downloaded:" << coordinator_->GetEntityAliveCount();
  // serializer_->RemoveDungeon(DungeonName::kHub);
  // qDebug() << "entities after hub removed:" << coordinator_->GetEntityAliveCount();
  // scene->GetSceneView()->scale(2.5, 2.5);
}

}  // namespace core
