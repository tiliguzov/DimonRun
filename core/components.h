#pragma once

#include <QVector2D>

namespace core {

struct MovementComponent {
  QVector2D position;
};

struct TransformationComponent {
  QVector2D direction;
  double current_speed = 1;
};

}  // namespace core
