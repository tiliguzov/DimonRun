#pragma once

#include <QVector2D>
#include <QPixmap>
#include <QGraphicsItem>

namespace core {

struct MovementComponent {
  QVector2D position;
  bool can_move;
};

struct GraphicsItemComponent {
  QGraphicsPixmapItem* item;
};

struct TransformationComponent {
  double default_speed = 1;
  QVector2D direction;
  double current_speed = 1;
};

}  // namespace core
