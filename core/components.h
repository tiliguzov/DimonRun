#pragma once

#include <QVector2D>
#include <QPixmap>
#include <QGraphicsItem>
#include <QGraphicsView>

#include "core/constants.h"

namespace core {

struct PositionComponent {
  QVector2D position;
};

struct GraphicsItemComponent {
  QGraphicsPixmapItem* item;
  // char source_name[kMaxPathLength];
  std::string source_name;
};

struct MovementComponent {
  QVector2D direction;
  float current_speed = 1;
};

}  // namespace core
