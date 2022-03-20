#include "scene.h"

#include <QKeyEvent>
#include <QPainter>

#include <algorithm>
#include <vector>
#include <iostream>

#include "connector.h"

Scene::Scene(QWidget* parent, Connector* connector)
    : QWidget(parent),
      connector_(connector),
      timer_id_(startTimer(5)) {  // constants
  show();
  setFocus();
}

void Scene::timerEvent(QTimerEvent* event) {
  if (event->timerId() != timer_id_) {
    return;
  }
  // std::cout << "TimerEvent)" << std::endl;
  connector_->OnTick();
}

void Scene::paintEvent(QPaintEvent*) {
  QPainter painter(this);
}
