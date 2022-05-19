#pragma once

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QPixmap>
#include <QVector2D>

#include "animation_pack.h"

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

//  the direction hero face to, left or right
enum class HorizontalDirection {
  kLeft,
  kRight
};

struct AnimationComponent {
  AnimationPack frames;
  std::string source_name;

  HorizontalDirection direction{HorizontalDirection::kRight};
  MovementType move_type{MovementType::kStaticInAir};

  // the flag that shows, if we have to change frame before its time pass
  bool need_change_frame{false};
};

}  // namespace core
