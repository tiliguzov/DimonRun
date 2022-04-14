#pragma once

#include <memory>

#include <QTimerEvent>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "engine/coordinator.h"

namespace core {

class Connector;

class Scene : public QWidget {
  Q_OBJECT

 public:
  Scene(QWidget* parent, Connector* connector);

  QGraphicsScene* GetScene();
  QGraphicsView* GetSceneView();
  engine::Entity* GetPlayer();

 private:
  void paintEvent(QPaintEvent*) override;
  void timerEvent(QTimerEvent*) override;
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;


  int32_t timer_id_;
  Connector* connector_;
  engine::Entity* player_;
  QGraphicsScene* scene_;
  QGraphicsView* scene_view_;
};

}  // namespace core
