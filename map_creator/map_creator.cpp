#include "map_creator.h"
#include "core/components.h"

#include <QListWidgetItem>
#include <QIcon>
#include <QPixmap>
#include <QCursor>
#include <QGraphicsLineItem>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFont>
#include <string>
#include <QFileDialog>
#include <QVector2D>
#include <iostream>

MapCreator::MapCreator(QWidget* parent, core::Connector* connector) :
      QWidget(parent),
      connector_(connector),
      timer_id_(startTimer(5)),
      scene_(new QGraphicsScene(this)),
      scene_view_(new QGraphicsView(this)),
      layer_button_(new QPushButton("0", this)),
      save_button_(new QPushButton("Save", this)),
      download_button_(new QPushButton("Download", this)),
      last_item_(nullptr) {
  CreateDefaultScene();
  LoadTextures();
  CreateGrid();

  connect(list_of_textures_[0], &QListWidget::itemClicked, this, [&] {
    ChooseItem();
  });

  connect(list_of_textures_[1], &QListWidget::itemClicked, this, [&] {
    ChooseItem();
  });

  connect(list_of_textures_[2], &QListWidget::itemClicked, this, [&] {
    ChooseItem();
  });

  connect(layer_button_, &QPushButton::clicked, this, [&] {
    ChangeLayer();
  });

  connect(save_button_, &QPushButton::clicked, this, [&] {
    auto file_name = QFileDialog::getSaveFileName(this);
    if (!file_name.endsWith(".json", Qt::CaseSensitive)) {
      return;
    }
    QFile file(file_name);
    file.open(QIODevice::WriteOnly);
    file.write(AllEntities().toJson());
    file.close();
  });

  connect(download_button_, &QPushButton::clicked, this, [&] {
    auto file_name = QFileDialog::getOpenFileName(this);
    if (!file_name.endsWith(".json", Qt::CaseSensitive)) {
      return;
    }
    QJsonObject file = GetJsonObject(file_name.toStdString());
    ReadFromJson(file);
  });
}

void MapCreator::CreateDefaultScene() {
  for (auto & list_of_texture : list_of_textures_) {
    list_of_texture = new QListWidget(this);
    list_of_texture->setGeometry(800, 0, 200, 600);
    list_of_texture->setIconSize(QSize(50, 50));
    list_of_texture->setVisible(false);
  }
  list_of_textures_[0]->setVisible(true);
  QFont font;
  font.setPixelSize(20);

  layer_button_->setFont(font);
  layer_button_->setGeometry(745, 0, 40, 40);

  save_button_->setFont(font);
  save_button_->setGeometry(690, 500, 95, 40);

  download_button_->setFont(font);
  download_button_->setGeometry(690, 540, 95, 40);

  scene_view_->setScene(scene_);
  scene_view_->setGeometry(0, 0, 800, 600);
  setGeometry(0, 0, 1000, 600);
  scene_view_->centerOn(0, 0);
  show();
}

void MapCreator::LoadTextures() {
  QJsonObject textures = GetJsonObject(":list_of_textures.json");
  for (const auto& key1 : textures.keys()) {
    int layer = key1.toInt();
    auto layer_textures = textures[key1].toObject();
    for (const auto& key2 : layer_textures.keys()) {
      auto texture_info = layer_textures[key2].toObject();
      auto pixmap = new QPixmap(texture_info["source"].toString());
      QString name = texture_info["name"].toString();
      auto item = new QListWidgetItem(QIcon(*pixmap),
                                      name, list_of_textures_[layer]);
      source_[item] = texture_info["source"].toString().toStdString();
      if (texture_info.contains("animation_source")) {
        animation_source_[source_[item]] =
            texture_info["animation_source"].toString().toStdString();
        direction_[source_[item]] =
            static_cast<core::HorizontalDirection>
            (texture_info["direction"].toInt());
        movement_type_[source_[item]] =
            str_to_type[texture_info["move_type"].toString().toStdString()];
        start_time_[source_[item]] =
            texture_info["start_time"].toInt();
      }
      if (texture_info.contains("is_movable")) {
        is_movable_[source_[item]] =
            texture_info["is_movable"].toBool();
        gravity_[source_[item]] =
            texture_info["gravity"].toBool();
        can_use_[source_[item]] =
            texture_info["can_use"].toBool();
        is_usable_[source_[item]] =
            texture_info["is_usable"].toBool();
        is_breakable_[source_[item]] =
            texture_info["is_breakable"].toBool();
      }

      if (texture_info.contains("kill_time")) {
        kill_time_[source_[item]] =
            texture_info["kill_time"].toInt();
        is_ill_[source_[item]] =
            texture_info["is_ill"].toBool();
      }

      if (texture_info.contains("current_speed")) {
        current_speed_[source_[item]] =
            texture_info["current_speed"].toDouble();
        direction_x_[source_[item]] =
            texture_info["direction_x"].toDouble();
        direction_y_[source_[item]] =
            texture_info["direction_y"].toDouble();
      }
    }
  }
  AddTexture(pos_of_hero_, 1, ":Hero_static_in_air_00.png");
}

void MapCreator::CreateGrid() {
  QPen pen;
  pen.setWidth(3);
  for (int i = -2000; i <= 2000; i += 500) {
    auto ox = new QGraphicsLineItem(-2000, i, 2000, i);
    auto oy = new QGraphicsLineItem(i, -2000, i, 2000);
    ox->setPen(pen);
    ox->setVisible(true);
    oy->setPen(pen);
    oy->setVisible(true);
    scene_->addItem(ox);
    scene_->addItem(oy);
  }
  pen.setCosmetic(true);
  pen.setWidth(1);
  for (int i = -2000; i <= 2000; i += 50) {
    auto ox = new QGraphicsLineItem(-2000, i, 2000, i);
    auto oy = new QGraphicsLineItem(i, -2000, i, 2000);
    ox->setPen(pen);
    ox->setVisible(true);
    oy->setPen(pen);
    oy->setVisible(true);
    scene_->addItem(ox);
    scene_->addItem(oy);
  }
  pen.setCosmetic(false);
  pen.setWidth(5);
  auto red_dot = new QGraphicsEllipseItem(10, 10, 10, 10);
  red_dot->setVisible(true);
  red_dot->setPen(pen);
  scene_->addItem(red_dot);
  scene_view_->centerOn(0.0, 0.0);
}

void MapCreator::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::RightButton && scene_view_->underMouse()) {
    DeleteTexture(MousePositionOnSceneView(), layer_);
  }
  if (event->button() == Qt::LeftButton &&
          new_texture_on && scene_view_->underMouse()) {
    AddTexture(MousePositionOnSceneView(), layer_,
               source_[list_of_textures_[layer_]->currentItem()]);
  }
}

void MapCreator::timerEvent(QTimerEvent* event) {
  if (event->timerId() != timer_id_) {
    return;
  }
  connector_->OnTick();
}

void MapCreator::AddTexture(QPointF point,
                            int layer, const std::string &source) {
  DeleteTexture(point, layer);
  if (source == ":Hero_static_in_air_00.png") {
    DeleteTexture(pos_of_hero_, 1);
    pos_of_hero_ = point;
  }
  auto coordinator = connector_->GetCoordinator();

  engine::Entity texture_entity = coordinator->CreateEntity();
  coordinator->AddComponent(texture_entity,
            core::PositionComponent{{static_cast<float>(point.x()),
                                     static_cast<float>(point.y())}});
  auto pixmap_item = scene_->addPixmap(new_texture_);
  pixmap_item->setPos(point);
  pixmap_item->setZValue(layer);
  coordinator->AddComponent(texture_entity
                            , core::GraphicsItemComponent{pixmap_item, source,
                            new_texture_scale_.first,
                            new_texture_scale_.second,
                            new_texture_rotate_});
  items_.push_back(texture_entity);

  if (animation_source_.count(source)) {
    coordinator->AddComponent(texture_entity, core::AnimationComponent{
        core::AnimationPack(animation_source_[source]),
        animation_source_[source],
        direction_[source],
        movement_type_[source],
        start_time_[source]
        });
  }
  if (is_movable_.count(source)) {
    coordinator->AddComponent(texture_entity, core::CollisionComponent{
      is_movable_[source],
      gravity_[source],
      can_use_[source],
      is_usable_[source],
      is_breakable_[source]
    });
  }
  if (kill_time_.count(source)) {
    coordinator->AddComponent(texture_entity, core::IllnessComponent{
      kill_time_[source],
      is_ill_[source]
    });
  }
  if (current_speed_.count(source)) {
    coordinator->AddComponent(texture_entity, core::MovementComponent{
      {static_cast<float>(direction_x_[source]),
      static_cast<float>(direction_y_[source])},
      static_cast<float>(current_speed_[source])
    });
  }
}

void MapCreator::DeleteTexture(QPointF point, int layer) {
  auto coordinator = connector_->GetCoordinator();
  for (int i = 0; i < items_.size(); ++i) {
    auto entity = items_[i];
    auto item = coordinator->
        GetComponent<core::GraphicsItemComponent>(entity).item;
    if (item->pos() == point && item->zValue() == layer) {
      coordinator->DestroyEntity(entity);
      items_.erase(items_.begin() + i);
      scene_->removeItem(item);
    }
  }
}

void MapCreator::DeleteAllTextures() {
  auto coordinator = connector_->GetCoordinator();
  for (int i = 0; i < items_.size(); ++i) {
    auto entity = items_[i];
    auto item = coordinator->
        GetComponent<core::GraphicsItemComponent>(entity).item;
    coordinator->DestroyEntity(entity);
    items_.erase(items_.begin() + i);
    scene_->removeItem(item);
  }
}


QPointF MapCreator::MousePositionOnSceneView() {
  QPointF res = (scene_view_->mapToScene(QCursor::pos())) -
      mapToGlobal(scene_view_->pos());
  if (res.x() < 0) {
    res.setX(res.x() - 50);
  }
  if (res.y() < 0) {
    res.setY(res.y() - 50);
  }
  res.setX(static_cast<int>(res.x()) / 50 * 50);
  res.setY(static_cast<int>(res.y()) / 50 * 50);
  return res;
}

QJsonObject MapCreator::GetJsonObject(
    const std::string& path_to_json) {
  QFile file(QString::fromStdString(path_to_json));
  file.open(QIODevice::ReadOnly);
  QJsonObject input_object = QJsonDocument::fromJson(file.readAll()).object();
  file.close();
  return input_object;
}

void MapCreator::keyPressEvent(QKeyEvent* event) {
  if (event->key() == Qt::Key_X) {
    ChangeLayer();
    return;
  }
  if (list_of_textures_[layer_]->currentItem() == nullptr ||
      !new_texture_on) {
    return;
  }
  auto texture = list_of_textures_[layer_]->currentItem()
      ->icon().pixmap(50, 50, QIcon::Normal, QIcon::Off);
  switch (event->key()) {
    case Qt::Key_S:
    case Qt::Key_W: {
      texture = texture.transformed(QTransform().scale(1, -1));
      new_texture_scale_.second *= -1;
      break;
    }
    case Qt::Key_A:
    case Qt::Key_D: {
      texture = texture.transformed(QTransform().scale(-1, 1));
      new_texture_scale_.first *= -1;
      break;
    }
    case Qt::Key_Q: {
      texture = texture.transformed(QTransform().rotate(-90));
      new_texture_rotate_ -= 90;
      break;
    }
    case Qt::Key_E: {
      texture = texture.transformed(QTransform().rotate(90));
      new_texture_rotate_ += 90;
      break;
    }
  }
  list_of_textures_[layer_]->currentItem()->setIcon(QIcon(texture));
  new_texture_ = texture;
}

void MapCreator::ChangeLayer() {
  SetDefaultScaleAndRotate();
  list_of_textures_[layer_]->setVisible(false);
  layer_++;
  layer_ %= 3;
  list_of_textures_[layer_]->setVisible(true);
  list_of_textures_[layer_]->clearSelection();
  if (layer_ == 0) {
    layer_button_->setText("0");
  } else if (layer_ == 1) {
    layer_button_->setText("1");
  } else if (layer_ == 2) {
    layer_button_->setText("2");
  }
  auto coordinator = connector_->GetCoordinator();
  for (auto item : items_) {
    auto pixmap_item =
        coordinator->GetComponent<core::GraphicsItemComponent>(item).item;
    if (pixmap_item->zValue() > layer_) {
      pixmap_item->setVisible(false);
    } else {
      pixmap_item->setVisible(true);
    }
  }
  new_texture_on = false;
  new_texture_scale_ = {1, 1};
  new_texture_rotate_ = 0;
}

void MapCreator::ChooseItem() {
  SetDefaultScaleAndRotate();
  last_item_ = list_of_textures_[layer_]->currentItem();
  new_texture_scale_ = {1, 1};
  new_texture_rotate_ = 0;
  new_texture_ = list_of_textures_[layer_]->currentItem()->icon().pixmap
      (50, 50, QIcon::Normal, QIcon::Off);
  new_texture_on = true;
  scene_view_->setFocus();
}

void MapCreator::SetDefaultScaleAndRotate() {
  if (last_item_ != nullptr) {
    new_texture_ = new_texture_.transformed(QTransform()
        .scale(new_texture_scale_.first, new_texture_scale_.second));
    new_texture_ = new_texture_.transformed(QTransform()
        .rotate(-new_texture_rotate_));
    last_item_->setIcon(QIcon(new_texture_));
  }
}

QJsonDocument MapCreator::AllEntities() {
  QJsonObject result;
  result["offset_x"] = 0;
  result["offset_y"] = 0;
  result["background_image"] = ":background_image.jpg";
  QJsonArray entities;
  auto coordinator = connector_->GetCoordinator();
  for (auto entity : items_) {
    QJsonObject entity_info;
    QJsonObject pos_comp_info;
    auto pos_comp = coordinator->GetComponent<core::PositionComponent>(entity);
    pos_comp_info["row"] = pos_comp.position.y() / 50;
    pos_comp_info["column"] = pos_comp.position.x() / 50;
    entity_info["position_comp"] = pos_comp_info;

    QJsonObject graphics_comp_info;
    auto graphics_comp = coordinator->
        GetComponent<core::GraphicsItemComponent>(entity);
    graphics_comp_info["source"] = graphics_comp.source_name.c_str();
    graphics_comp_info["z_value"] = graphics_comp.item->zValue();
    graphics_comp_info["scale_x"] = graphics_comp.scale_x;
    graphics_comp_info["scale_y"] = graphics_comp.scale_y;
    graphics_comp_info["rotate"] = graphics_comp.rotate;
    entity_info["graphics_comp"] = graphics_comp_info;

    if (coordinator->HasComponent<core::AnimationComponent>(entity)) {
      QJsonObject anim_comp_info;
      auto anim_comp = coordinator->
          GetComponent<core::AnimationComponent>(entity);
      anim_comp_info["source"] = anim_comp.source_name.c_str();
      anim_comp_info["direction"] = static_cast<int>(anim_comp.direction);
      anim_comp_info["move_type"] = static_cast<int>(anim_comp.move_type);
      anim_comp_info["start_time"] = anim_comp.start_time;
      entity_info["animation_comp"] = anim_comp_info;

      if (anim_comp_info["move_type"] == static_cast<int>
              (core::MovementType::kStaticInAir)) {
        entity_info["joystick_comp"] = QJsonObject();
      }
    }

    if (coordinator->HasComponent<core::CollisionComponent>(entity)) {
      QJsonObject coll_comp_info;
      auto coll_comp = coordinator->
          GetComponent<core::CollisionComponent>(entity);
      coll_comp_info["is_movable"] = coll_comp.is_movable;
      coll_comp_info["gravity"] = coll_comp.gravity;
      coll_comp_info["can_use"] = coll_comp.can_use;
      coll_comp_info["is_usable"] = coll_comp.is_usable;
      coll_comp_info["is_breakable"] = coll_comp.is_breakable;

      entity_info["collision_comp"] = coll_comp_info;
    }

    if (coordinator->HasComponent<core::IllnessComponent>(entity)) {
      QJsonObject illness_comp_info;
      auto illness_comp = coordinator->
          GetComponent<core::IllnessComponent>(entity);
      illness_comp_info["kill_time"] = illness_comp.kill_time;
      illness_comp_info["is_ill"] = illness_comp.is_ill;

      entity_info["illness_comp"] = illness_comp_info;
    }

    if (coordinator->HasComponent<core::MovementComponent>(entity)) {
      QJsonObject move_comp_info;
      auto move_comp = coordinator->
          GetComponent<core::MovementComponent>(entity);
      move_comp_info["current_speed"] = move_comp.current_speed;
      move_comp_info["direction_x"] = move_comp.direction.x();
      move_comp_info["direction_y"] = move_comp.direction.y();

      entity_info["movement_comp"] = move_comp_info;
    }
    entities.push_back(entity_info);
  }
  result["entities"] = entities;
  QJsonDocument res(result);
  return res;
}

void MapCreator::ReadFromJson(const QJsonObject& file) {
  DeleteAllTextures();
  pos_of_hero_ = {0, 0};
  AddTexture(pos_of_hero_, 1, ":Hero_static_in_air_00.png");

  auto entities = file["entities"].toArray();
  for (auto entity : entities) {
    QPoint pos;
    auto position_comp = entity.toObject()["position_comp"].toObject();
    auto graphics_comp = entity.toObject()["graphics_comp"].toObject();
    pos.setX(position_comp["column"].toInt() * 50);
    pos.setY(position_comp["row"].toInt() * 50);
    int layer = graphics_comp["z_value"].toInt();
    new_texture_scale_ = {graphics_comp["scale_x"].toInt(),
                          graphics_comp["scale_y"].toInt()};
    new_texture_rotate_ = {graphics_comp["rotate"].toInt()};
    auto source = graphics_comp["source"].toString();
    new_texture_ = QPixmap(source);
    new_texture_ = new_texture_.transformed(QTransform().scale(
        new_texture_scale_.first, new_texture_scale_.second));
    new_texture_ = new_texture_.transformed(QTransform()
        .rotate(new_texture_rotate_));
    AddTexture(pos, layer, source.toStdString());
  }
}

void MapCreator::resizeEvent(QResizeEvent* event) {
  int width = event->size().width();
  int height = event->size().height();

  scene_view_->setGeometry(0, 0, width - 200, height);
  for (auto list_of_textures : list_of_textures_) {
    list_of_textures->setGeometry(width - 200, 0, 200, height);
  }

  layer_button_->setGeometry(width - 255, 0, 40, 40);
  save_button_->setGeometry(width - 310, height - 100, 95, 40);
  download_button_->setGeometry(width - 310, height - 60, 95, 40);
}

