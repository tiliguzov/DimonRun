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
      timer_id_(startTimer(kTickTime)) {
  show();
  setFocus();
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

}  // namespace core
