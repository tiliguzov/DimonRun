#pragma once

#include <memory>

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimerEvent>
#include <QWidget>

#include "engine/coordinator.h"

namespace core {

class Connector;

enum ZValues {
    kBackgroundZIndex,
    kPlayerZIndex
};

class Scene : public QWidget {
  Q_OBJECT

 public:
  Scene(QWidget* parent, Connector* connector);

  QGraphicsScene* GetScene();
  QGraphicsView* GetSceneView();

 private:
  void paintEvent(QPaintEvent*) override;
  void timerEvent(QTimerEvent*) override;
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;

  void SetDefaultSceneSettings();

  bool eventFilter(QObject *object, QEvent *event) override;

  int32_t timer_id_;
  QWidget* parent_;
  Connector* connector_;
  QGraphicsItem* hero_item_;
  QGraphicsScene* scene_;
  QGraphicsView* scene_view_;
};

}  // namespace core
