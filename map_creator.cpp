#include "map_creator.h"
#include "core/components.h"
#include "core/serializer.h"

#include <QListWidgetItem>
#include <QIcon>
#include <QPixmap>
#include <QCursor>
#include <QGraphicsLineItem>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFont>
#include <iostream>
#include <string>
#include <QFileDialog>

MapCreator::MapCreator(QWidget* parent, MCConnector* connector) :
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
      auto pixmap = new QPixmap(layer_textures[key2].toArray()[0].toString());
      QString name = layer_textures[key2].toArray()[1].toString();
      auto item = new QListWidgetItem(QIcon(*pixmap), name, list_of_textures_[layer]);
      source_[item] = layer_textures[key2].toArray()[0].toString().toStdString();
    }
  }
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
  scene_view_->centerOn(0, 0);
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

void MapCreator::AddTexture(QPointF point, int layer, const std::string &source) {
  DeleteTexture(point, layer);

  auto coordinator = connector_->GetCoordinator();

  engine::Entity texture_entity = coordinator->CreateEntity();
  coordinator->AddComponent(texture_entity,
            core::PositionComponent{{static_cast<float>(point.x()),
                                     static_cast<float>(point.y())}});
  auto pixmap_item = scene_->addPixmap(new_texture_);
  pixmap_item->setPos(point);
  pixmap_item->setZValue(layer);
  coordinator->AddComponent(texture_entity
                            , core::GraphicsItemComponent{pixmap_item, source});
  items_.push_back(texture_entity);
  scale_of_items_[texture_entity] = new_texture_scale_;
  rotate_of_items_[texture_entity] = new_texture_rotate_;
}

void MapCreator::DeleteTexture(QPointF point, int layer) {
  auto coordinator = connector_->GetCoordinator();
  for (int i = 0; i < items_.size(); ++i) {
    auto entity = items_[i];
    auto item = coordinator->GetComponent<core::GraphicsItemComponent>(entity).item;
    if (item->pos() == point && item->zValue() == layer) {
      coordinator->DestroyEntity(entity);
      items_.erase(items_.begin() + i);
      scene_->removeItem(item);
    }
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
  res.setX((int)res.x() / 50 * 50);
  res.setY((int)res.y() / 50 * 50);
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
  if (list_of_textures_[layer_]->currentItem() == nullptr) {
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
    graphics_comp_info["scale_x"] = scale_of_items_[entity].first;
    graphics_comp_info["scale_y"] = scale_of_items_[entity].second;
    graphics_comp_info["rotate"] = rotate_of_items_[entity];
    entity_info["graphics_comp"] = graphics_comp_info;

    entities.push_back(entity_info);
  }
  result["entities"] = entities;
  QJsonDocument res(result);
  return res;
}

void MapCreator::ReadFromJson(const QJsonObject& file) {
  auto entities = file["entities"].toArray();
  for (auto entity : entities) {
    QPoint pos;
    auto position_comp = entity.toObject()["position_comp"].toObject();
    auto graphics_comp = entity.toObject()["graphics_comp"].toObject();
    pos.setX(position_comp["column"].toInt() * 50);
    pos.setY(position_comp["row"].toInt() * 50);
    int layer = graphics_comp["z_value"].toInt();
    new_texture_scale_ = {graphics_comp["scale_x"].toInt(), graphics_comp["scale_y"].toInt()};
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
