#pragma once

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QPixmap>
#include <QVector2D>

#include "core/constants.h"

namespace core {

struct PositionComponent {
  QVector2D position;
};

struct GraphicsItemComponent {
  QGraphicsPixmapItem* item;
  std::string source_name;
};

struct MovementComponent {
  QVector2D direction;
  float current_speed = 1;
};

}  // namespace core
