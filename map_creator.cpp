#include "map_creator.h"

#include <QListWidgetItem>
#include <QIcon>
#include <QPixmap>
#include <QCursor>
#include <QGraphicsLineItem>
#include <iostream>

MapCreator::MapCreator(QWidget* parent) :
      QWidget(parent),
      scene_(new QGraphicsScene(this)),
      scene_view_(new QGraphicsView(this)),
      list_of_textures_(new QListWidget(this)) {
  scene_view_->setScene(scene_);
  scene_view_->centerOn(0, 0);
  scene_view_->setGeometry(0, 0, 800, 600);
  list_of_textures_->setGeometry(800, 0, 200, 600);
  list_of_textures_->setIconSize(QSize(50, 50));
  LoadTextures();
  CreateGrid();
  setGeometry(0, 0, 1000, 600);
  show();

  connect(list_of_textures_, &QListWidget::itemClicked, this, [&] {
    // new_texture_ = list_of_textures_->currentItem()->icon().pixmap
    //     (50, 50, QIcon::Normal, QIcon::Off);
    new_texture_ = ground_;
    new_texture_on = true;
  });
}

void MapCreator::LoadTextures() {
  ground_ = new QPixmap(":textures/background/ground.jpg");
  auto item = new QListWidgetItem(*ground_, "ground", list_of_textures_);
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
  if (new_texture_on && scene_view_->underMouse()) {
    AddTexture();
  }
}

void MapCreator::AddTexture() {
  // std::cerr << scene_view_->rect << std::endl;
  auto pixmap_item = new QGraphicsPixmapItem(*new_texture_);
  pixmap_item->setPos(MousePositionOnSceneView());
  pixmap_item->setVisible(true);
  scene_->addItem(pixmap_item);
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
