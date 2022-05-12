#include "scene.h"

#include <QKeyEvent>
#include <QPainter>
#include <QTimerEvent>
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

  SetDefaultSceneSettings();

  hero_item_ = connector_->CreateHero(this);

  show();
  setFocus();
  // example of interacting with engine
  connector->StartGame(this); // todo
}

void Scene::timerEvent(QTimerEvent* event) {
  if (event->timerId() != timer_id_) {
    return;
  }
  connector_->OnTick();
  scene_view_->centerOn(hero_item_);
}

void Scene::paintEvent(QPaintEvent*) {
  QPainter painter(this);
}

void Scene::keyPressEvent(QKeyEvent *event) {
    connector_->OnKeyPress(static_cast<Qt::Key>(event->key()));
}

void Scene::keyReleaseEvent(QKeyEvent *event) {
    connector_->OnKeyRelease(static_cast<Qt::Key>(event->key()));
}

void Scene::SetDefaultSceneSettings() {
    scene_view_->setScene(scene_);
    scene_view_->setGeometry(0, 0, kDefaultWindowWidth, kDefaultWindowHeight);
    scene_view_->setAutoFillBackground(true);
    scene_view_->setBackgroundBrush(Qt::darkGreen);
    scene_view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scene_view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scene_view_->viewport()->installEventFilter(this);  // wheel
    // scrolling disabled
    scene_view_->setSceneRect(kLeftUpXCoordiante, kLeftUpYCoordiante,
                              kWidth, kHeight);
}

QGraphicsScene* Scene::GetScene() {
  return scene_;
}

QGraphicsView* Scene::GetSceneView() {
  return scene_view_;
}

bool Scene::eventFilter(QObject *object, QEvent *event) {
  if (object == scene_view_->viewport() && event->type() == QEvent::Wheel) {
    return true;
  }
  return false;
}

}  // namespace core
