#include "map_creator.h"

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

MapCreator::MapCreator(QWidget* parent) :
      QWidget(parent),
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
    new_texture_ = list_of_textures_[layer_]->currentItem()->icon().pixmap
        (50, 50, QIcon::Normal, QIcon::Off);
    new_texture_on = true;
  });

  connect(list_of_textures_[1], &QListWidget::itemClicked, this, [&] {
    new_texture_ = list_of_textures_[layer_]->currentItem()->icon().pixmap
        (50, 50, QIcon::Normal, QIcon::Off);
    new_texture_on = true;
  });

  connect(list_of_textures_[2], &QListWidget::itemClicked, this, [&] {
    new_texture_ = list_of_textures_[layer_]->currentItem()->icon().pixmap
        (50, 50, QIcon::Normal, QIcon::Off);
    new_texture_on = true;
  });

  connect(layer_button_, &QPushButton::clicked, this, [&] {
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
    for (auto item : items_) {
      if (item->zValue() > layer_) {
        item->setVisible(false);
      } else {
        item->setVisible(true);
      }
    }
    new_texture_on = false;
  });
}

void MapCreator::LoadTextures() {
  QJsonObject textures = GetJsonObject(":list_of_textures.json");
  for (const auto& key1 : textures.keys()) {
    int layer = key1.toInt();
    std::cerr << layer << std::endl;
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

void MapCreator::AddTexture() {
  DeleteTexture();
  auto pixmap_item = new QGraphicsPixmapItem(new_texture_);
  pixmap_item->setPos(MousePositionOnSceneView());
  pixmap_item->setVisible(true);
  pixmap_item->setZValue(layer_);
  scene_->addItem(pixmap_item);
  items_.push_back(pixmap_item);
}

void MapCreator::DeleteTexture() {
  auto pos = MousePositionOnSceneView();
  for (int i = 0; i < items_.size(); ++i) {
    auto item = items_[i];
    if (item->pos() == pos && item->zValue() == layer_) {
      items_.erase(items_.begin() + i);
      scene_->removeItem(item);
      break;
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