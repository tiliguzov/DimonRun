#pragma once

#include <QVector2D>

namespace core {

struct MovementComponent {
  QVector2D position;
};

struct TransformationComponent {
  double default_speed = 1;
  QVector2D direction;
  double current_speed = 1;
};

}  // namespace core
