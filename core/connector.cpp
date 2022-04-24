#include "connector.h"

#include <QFile>
#include <memory>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include "constants.h"
#include "components.h"
#include "engine/coordinator.h"
#include "systems/joystick_system.h"
#include "systems/movement_system.h"
#include "systems/painting_system.h"
#include "systems/animation_system.h"

namespace core {

Connector::Connector() : coordinator_(std::make_unique<engine::Coordinator>()),
                             keyboard_(std::make_unique<core::Keyboard>()) {
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
  coordinator_->AddComponent(hero, PositionComponent{{0, 0}});
  coordinator_->AddComponent(hero, MovementComponent{{0, 0}, 1});
  auto item = scene->GetScene()->addPixmap(
      QPixmap(":textures/hero/Hero_static_in_air_00.png"));
  // z value for hero
  item->setZValue(kPlayerZIndex);
  coordinator_->AddComponent(hero, GraphicsItemComponent{item});
  coordinator_->AddComponent(hero, AnimationComponent());
  coordinator_->GetComponent<AnimationComponent>(hero).animations =
      new AnimationPack(":animations/hero.json");
  return item;
}

// example of interacting with engine
void Connector::StartGame(Scene* scene) {
  scene->GetSceneView()->scale(2.5, 2.5);

  QFile file(QString::fromStdString(":level0.json"));
  file.open(QIODevice::ReadOnly);
  QJsonObject input_object = QJsonDocument::fromJson(file.readAll()).object();
  file.close();

  auto textures_types = input_object["textures"].toObject();

  for (const auto &key : textures_types.keys()) {
    auto texture_path = textures_types[key].toArray()[0].toString();
    auto z_value = textures_types[key].toArray()[1].toInt();
    QPixmap image(texture_path);
    for (int i = 2; i < textures_types[key].toArray().size(); ++i) {
      int x = textures_types[key].toArray()[i].toArray()[0].toInt();
      int y = textures_types[key].toArray()[i].toArray()[1].toInt();
      int scale_x = textures_types[key].toArray()[i].toArray()[2].toInt();
      int scale_y = textures_types[key].toArray()[i].toArray()[3].toInt();
      x *= core::kTextureSize;
      y *= core::kTextureSize;
      engine::Entity entity = coordinator_->CreateEntity();
      auto item = scene->GetScene()->addPixmap(image);
      item->setZValue(z_value);
      item->setPixmap(image.transformed(QTransform().scale(scale_x, scale_y)));
      coordinator_->AddComponent(entity, GraphicsItemComponent{item});
      coordinator_->AddComponent(entity,
                                 PositionComponent{{1.0f * x, 1.0f * y}});
    }
  }

  QFile file_torch(QString::fromStdString(":animations/back_wall_torch.json"));
  file_torch.open(QIODevice::ReadOnly);
  QJsonObject input_torch = QJsonDocument::fromJson(
      file_torch.readAll()).object();
  file_torch.close();

  for (const auto& position : input_torch["positions"].toArray()) {
    int x = position[0].toInt();
    int y = position[1].toInt();
    x *= kTextureSize;
    y *= kTextureSize;
    engine::Entity entity = coordinator_->CreateEntity();
    coordinator_->AddComponent(entity, PositionComponent{{1.0f * x, 1.0f * y}});
    auto item = scene->GetScene()->addPixmap(
        QPixmap(":textures/background/back_wall_and_torch_00.png"));
    item->setZValue(kBackgroundZIndex);
    coordinator_->AddComponent(entity, GraphicsItemComponent{item});
    coordinator_->AddComponent(entity, AnimationComponent());
    coordinator_->GetComponent<AnimationComponent>(entity).animations =
        new AnimationPack(":animations/back_wall_torch.json");
    coordinator_->GetComponent<AnimationComponent>(entity).movement_type =
        MovementType::kTorchBurning;
  }
}

}  // namespace core
