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
#include <iostream>
#include <string>

MapCreator::MapCreator(QWidget* parent, MCConnector* connector) :
      QWidget(parent),
      connector_(connector),
      timer_id_(startTimer(5)),
      scene_(new QGraphicsScene(this)),
      scene_view_(new QGraphicsView(this)),
      layer_button_(new QPushButton("0", this)) {
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

  scene_view_->setScene(scene_);
  scene_view_->centerOn(0, 0);
  scene_view_->setGeometry(0, 0, 800, 600);

  LoadTextures();
  CreateGrid();
  setGeometry(0, 0, 1000, 600);
  show();

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
    DeleteTexture();
  }
  if (event->button() == Qt::LeftButton &&
          new_texture_on && scene_view_->underMouse()) {
    AddTexture();
  }
}

void MapCreator::timerEvent(QTimerEvent* event) {
  if (event->timerId() != timer_id_) {
    return;
  }
  connector_->OnTick();
}

void MapCreator::AddTexture() {
  DeleteTexture();

  auto coordinator = connector_->GetCoordinator();

  engine::Entity texture_entity = coordinator->CreateEntity();
  auto point = MousePositionOnSceneView();
  coordinator->AddComponent(texture_entity,
            core::PositionComponent{{static_cast<float>(point.x()),
                                     static_cast<float>(point.y())}});
  auto pixmap_item = scene_->addPixmap(new_texture_);
  pixmap_item->setPos(point);
  pixmap_item->setZValue(layer_);
  coordinator->AddComponent(texture_entity
                            , core::GraphicsItemComponent{pixmap_item});

  items_.push_back(texture_entity);
}

void MapCreator::DeleteTexture() {
  auto coordinator = connector_->GetCoordinator();
  auto pos = MousePositionOnSceneView();
  for (int i = 0; i < items_.size(); ++i) {
    auto entity = items_[i];
    auto item = coordinator->GetComponent<core::GraphicsItemComponent>(entity).item;
    if (item->pos() == pos && item->zValue() == layer_) {
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
      break;
    }
    case Qt::Key_A:
    case Qt::Key_D: {
      texture = texture.transformed(QTransform().scale(-1, 1));
      break;
    }
    case Qt::Key_Q: {
      texture = texture.transformed(QTransform().rotate(-90));
      break;
    }
    case Qt::Key_E: {
      texture = texture.transformed(QTransform().rotate(90));
      break;
    }
  }
  list_of_textures_[layer_]->currentItem()->setIcon(QIcon(texture));
  new_texture_ = texture;
}

void MapCreator::ChangeLayer() {
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
}

void MapCreator::ChooseItem() {
  new_texture_ = list_of_textures_[layer_]->currentItem()->icon().pixmap
      (50, 50, QIcon::Normal, QIcon::Off);
  new_texture_on = true;
  scene_view_->setFocus();
}
