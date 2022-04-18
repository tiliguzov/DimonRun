#pragma once

#include <QVector2D>
#include <QPixmap>
#include <QGraphicsItem>
#include <QGraphicsView>

namespace core {

struct PositionComponent {
  QVector2D position;
};

struct GraphicsItemComponent {
  QGraphicsPixmapItem* item;
};

struct MovementComponent {
  QVector2D direction;
  float current_speed = 1;
};

}  // namespace core
