#pragma once

#include <QVector2D>
#include <QPixmap>
#include <QGraphicsItem>

namespace core {

// example of component
struct MovementComponent {
  QVector2D position;
  bool can_move;
};

struct GraphicsItemComponent {
  QGraphicsPixmapItem* item;

  // explicit GraphicsItemComponent(QPixmap image) {
  //   item = new QGraphicsPixmapItem(image, nullptr);
  // }
};

}  // namespace core
