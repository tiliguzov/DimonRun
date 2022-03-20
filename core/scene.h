#pragma once

#include <memory>

#include <QWidget>
#include <QTimerEvent>

#include "engine/coordinator.h"

class Connector;

class Scene : public QWidget {
  Q_OBJECT

 public:
  Scene(QWidget* parent,
        Connector* connector);

 private:
  void paintEvent(QPaintEvent*) override;
  void timerEvent(QTimerEvent*) override;

  int32_t timer_id_;
  Connector* connector_;
  Entity* player_;
};
