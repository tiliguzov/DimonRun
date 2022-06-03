#pragma once

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimerEvent>
#include <QWidget>

#include <memory>

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
  void SetHeroEntity(engine::Entity entity);
  void SetBackgroundImage(QGraphicsPixmapItem* item);

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
  engine::Entity hero_entity_;
  QGraphicsScene* scene_;
  QGraphicsView* scene_view_;
  QGraphicsPixmapItem* background_image_;
};

}  // namespace core
