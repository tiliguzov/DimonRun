#pragma once

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QListWidget>
#include <QMouseEvent>

class MapCreator : public QWidget {
  Q_OBJECT

 public:
  MapCreator(QWidget* parent);
  void LoadTextures();
  void CreateGrid();
  void AddTexture();
  QPointF MousePositionOnSceneView();

  void mousePressEvent(QMouseEvent* event) override;

 private:
  QGraphicsScene* scene_;
  QGraphicsView* scene_view_;
  QListWidget* list_of_textures_;
  bool new_texture_on{false};
  QPixmap* new_texture_;
  QPixmap* ground_;
};