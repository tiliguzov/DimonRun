#include "scene.h"

#include <QKeyEvent>
#include <QPainter>
#include <QTimerEvent>
#include <QGraphicsPixmapItem>
#include <QWidget>

#include "connector.h"
#include "constants.h"

namespace core {

Scene::Scene(QWidget* parent, Connector* connector)
    : QWidget(parent),
      connector_(connector),
      timer_id_(startTimer(kTickTime)),
      scene_(new QGraphicsScene(this)),
      scene_view_(new QGraphicsView(this)) {
  scene_view_->setScene(scene_);
  scene_view_->setGeometry(0, 0, 1000, 600);
  scene_view_->setAutoFillBackground(true);
  scene_view_->setBackgroundBrush(Qt::darkGreen);
  show();
  setFocus();
  // // example of interacting with engine
  connector->Example(this);
}

void Scene::timerEvent(QTimerEvent* event) {
  if (event->timerId() != timer_id_) {
    return;
  }
  connector_->OnTick();
}

void Scene::paintEvent(QPaintEvent*) {
  QPainter painter(this);
}

QGraphicsScene* Scene::GetScene() {
  return scene_;
}

QGraphicsView* Scene::GetSceneView() {
  return scene_view_;
}

engine::Entity* Scene::GetPlayer() {
  return player_;
}

}  // namespace core
