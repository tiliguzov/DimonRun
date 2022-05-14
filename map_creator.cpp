#include "map_creator.h"

#include <QListWidgetItem>
#include <QIcon>
#include <QPixmap>

MapCreator::MapCreator(QWidget* parent) :
      QWidget(parent),
      scene_(new QGraphicsScene(this)),
      scene_view_(new QGraphicsView(this)),
      list_of_textures_(new QListWidget(this)) {
  scene_view_->setScene(scene_);
  list_of_textures_->setGeometry(800, 0, 200, 600);
  list_of_textures_->setIconSize(QSize(100, 100));
  LoadTextures();
  // auto item = new QListWidgetItem(pixmap, "ground", list_of_textures_);
  setGeometry(0, 0, 1000, 600);
  show();
}

void MapCreator::LoadTextures() {

}
