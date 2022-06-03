#pragma once

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QPixmap>
#include <QVector2D>

#include <string>

#include "animation_pack.h"

namespace core {

struct PositionComponent {
  QVector2D position;
};

struct CollisionComponent {
    bool is_movable;
    bool gravity;
    bool can_use;
    bool is_usable;
    bool is_breakable;
};

struct JoysticComponent {};

struct IllnessComponent {
  int kill_time;
  bool is_ill;
};

struct GraphicsItemComponent {
  QGraphicsPixmapItem* item;
  std::string source_name;
  double scale_x, scale_y;
  int rotate;
};

struct MovementComponent {
  QVector2D direction;
  float current_speed = 1;
};

struct AnimationComponent {
  AnimationPack frames;
  std::string source_name;
  HorizontalDirection direction;
  MovementType move_type;
  int start_time{0};
  // the flag that shows, if we have to change frame before its time pass
  bool need_change_frame{false};
};

}  // namespace core
