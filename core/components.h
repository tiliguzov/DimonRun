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
  QGraphicsView* camera;
  bool need_camera_following;
};

struct MovementComponent {
  QVector2D direction;
  double current_speed = 1;
};

}  // namespace core
