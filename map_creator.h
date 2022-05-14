#pragma once

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QListWidget>

class MapCreator : public QWidget {
  Q_OBJECT

 public:
  MapCreator(QWidget* parent);
  void LoadTextures();

 private:
  QGraphicsScene* scene_;
  QGraphicsView* scene_view_;
  QListWidget* list_of_textures_;
};