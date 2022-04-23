#pragma once

#include <QVector2D>
#include <QPixmap>
#include <QGraphicsItem>
#include <QGraphicsView>

#include "animation_pack.h"

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

//  the direction hero face to, left or right
enum class LookingSide {
  kLeft,
  kRight
};

struct AnimationComponent {
  LookingSide direction = LookingSide::kRight;
  MovementType movement_type = MovementType::kStaticInAir;

  const AnimationPack* animations{nullptr};
};

}  // namespace core
