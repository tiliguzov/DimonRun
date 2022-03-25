#pragma once

#include <memory>

#include <QTimerEvent>
#include <QWidget>

#include "engine/coordinator.h"

namespace core {

class Connector;

class Scene : public QWidget {
  Q_OBJECT

 public:
  Scene(QWidget* parent, Connector* connector);

 private:
  void paintEvent(QPaintEvent*) override;
  void timerEvent(QTimerEvent*) override;
  void keyPressEvent(QKeyEvent* event);
  void keyReleaseEvent(QKeyEvent* event);


  int32_t timer_id_;
  Connector* connector_;
  engine::Entity* player_;
};

}  // namespace core
