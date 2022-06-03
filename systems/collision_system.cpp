#include "collision_system.h"
#include "core/constants.h"
#include "animation_system.h"

#include <cmath>
#include <algorithm>
#include <iostream>
#include <assert.h>
#include <vector>
#include <utility>

namespace {

bool IsIntersectPositions(
    const core::PositionComponent& position1,
    const core::PositionComponent& position2) {
  double first_x = position1.position.x(), first_y = position1.position.y();
  double second_x = position2.position.x(), second_y = position2.position.y();
  if (first_x > second_x) {
    std::swap(first_x, second_x);
    std::swap(first_y, second_y);
  }
  return (first_x + core::kTextureSize > second_x
      && abs(first_y - second_y) < core::kTextureSize);
}

std::pair<double, double> IntersectPositions(
    const core::PositionComponent& position1,
    const core::PositionComponent& position2) {
  if (!IsIntersectPositions(position1, position2)) {
    return {0, 0};
  }
  double first_x = position1.position.x(), first_y = position1.position.y();
  double second_x = position2.position.x(), second_y = position2.position.y();
  if (first_x > second_x) {
    std::swap(first_x, second_x);
    std::swap(first_y, second_y);
  }
  return {std::min(first_x, second_x) - std::max(first_x, second_x)
              + core::kTextureSize,
          std::abs(std::min(first_y, second_y) - std::max(first_y, second_y)
                       + core::kTextureSize)};
}

}  // namespace

systems::CollisionSystem::CollisionSystem(engine::Coordinator* coordinator,
                                          core::Connector* connector)
    : coordinator_(coordinator), connector_(connector), time_(0) {}

void systems::CollisionSystem::Update() {
  time_ += core::kTickTime;
  std::vector<engine::Entity> movable_entities;
  for (engine::Entity entity : entities_) {
    auto& collision_comp =
        coordinator_->GetComponent<core::CollisionComponent>(entity);
    if (collision_comp.is_movable) {
      movable_entities.push_back(entity);
    }
  }

  for (engine::Entity entity1 : movable_entities) {
    for (engine::Entity entity2 : entities_) {
      if (entity1 == entity2) {
        continue;
      }
      auto& graphics_comp1 = coordinator_->
          GetComponent<core::GraphicsItemComponent>(entity1);
      auto& graphics_comp2 = coordinator_->
          GetComponent<core::GraphicsItemComponent>(entity2);

      if (graphics_comp1.item->zValue() != graphics_comp2.item->zValue()) {
        continue;
      }
      auto& position_comp1 = coordinator_->
          GetComponent<core::PositionComponent>(entity1);
      auto& position_comp2 = coordinator_->
          GetComponent<core::PositionComponent>(entity2);

      auto& collision_comp1 =
          coordinator_->GetComponent<core::CollisionComponent>(entity1);
      auto& collision_comp2 =
          coordinator_->GetComponent<core::CollisionComponent>(entity2);

      auto& movement_comp1 =
          coordinator_->GetComponent<core::MovementComponent>(entity1);

      auto new_position1 = position_comp1;
      new_position1.position +=
          movement_comp1.direction * movement_comp1.current_speed;
      const long double eps = 1e-1;
      if (collision_comp1.can_use && collision_comp2.is_usable &&
          (IntersectPositions(new_position1, position_comp2).first > eps ||
              IntersectPositions(new_position1, position_comp2).second > eps)) {
        connector_->UseEvent(entity2);
        std::cout
            << "THIS IS USSSSSSSSSSSSSSSSSSSSSSSSSEEEEEEEEEEEEEEEEEEEEEEE"
            << std::endl;
        continue;
      }
      if (collision_comp1.can_use && collision_comp2.is_breakable &&
          (IntersectPositions(new_position1, position_comp2).first > eps ||
              IntersectPositions(new_position1, position_comp2).second > eps)) {
        auto& illness_comp2 =
            coordinator_->GetComponent<core::IllnessComponent>(entity2);
        if (!illness_comp2.is_ill) {
          illness_comp2.is_ill = true;
          if (coordinator_->
          HasComponent<core::AnimationComponent>(entity2) &&
              coordinator_->
              GetComponent<core::AnimationComponent>(entity2).move_type ==
              core::MovementType::kCoinMoving) {
            // connector_->AddCoin(entity2);
          } else if (coordinator_->
              HasComponent<core::AnimationComponent>(entity2) &&
              coordinator_->
              GetComponent<core::AnimationComponent>(entity2).move_type ==
              core::MovementType::kRubbishDestroy &&
              coordinator_->GetComponent<core::AnimationComponent>
                  (entity2).start_time > time_) {
            coordinator_->GetComponent<core::AnimationComponent>(entity2)
                .start_time = time_;
          }
          std::cout << "THIS IS ILNEEEEEEEEEEEEEEEEEEEEEEEESSSSSSSSSSSSSSSSSSS"
                    << std::endl;
        }
        continue;
      }
      if (collision_comp2.is_movable == false) {
        if (IntersectPositions(new_position1, position_comp2).second > eps) {
          movement_comp1.direction = {0, 0};
        }
      } else {
        auto& movement_comp2 =
            coordinator_->GetComponent<core::MovementComponent>(entity2);
        auto new_position2 = position_comp2;
        new_position2.position +=
            movement_comp2.direction * movement_comp2.current_speed;
        std::pair<double, double>
            intersection = IntersectPositions(new_position1, new_position2);
        if (intersection.first < eps) {
          intersection.first = 0;
        }
        if (intersection.second < eps) {
          intersection.second = 0;
        }
        if (intersection.first == 0 || intersection.second == 0) {
          continue;
        }

        if (intersection.first == core::kTextureSize) {
          if (new_position1.position.y() < new_position2.position.y()) {
            if (collision_comp1.gravity && collision_comp2.gravity) {
              if (movement_comp1.direction == QVector2D{0, 0}) {
                position_comp1.position -= {0, movement_comp1.current_speed};
              }
              movement_comp1.direction = {0, 0};
            } else if (collision_comp1.gravity && !collision_comp2.gravity) {
              double diff = abs(std::round(position_comp1.position.y())
                                    - position_comp1.position.y());
              if (diff > eps ||
                  (diff < eps &&
                      static_cast<int>(std::round(position_comp1.position.y()))
                          % core::kTextureSize != 0)) {
                position_comp2.position += {0, movement_comp2.current_speed};
                movement_comp2.direction = {0.0, 0.0};
              } else {
                movement_comp1.direction = {0.0, 0.0};
                movement_comp2.direction = {0.0, 0.0};
              }
            } else {
              position_comp1.position -=
                  movement_comp1.direction * movement_comp1.current_speed;
            }
          }
        } else if (intersection.second == core::kTextureSize) {
          if (new_position1.position.x() < new_position2.position.x()) {
            if (!collision_comp1.gravity && collision_comp2.gravity) {
              new_position2.position +=
                  QVector2D {1, 0} * movement_comp1.current_speed;
              if (movement_comp2.direction != QVector2D{0, 0}) {
                movement_comp1.direction = {0, 0};
                continue;
              }
              bool can_move = true;
              for (auto& new_entity : entities_) {
                if (new_entity == entity1 || new_entity == entity2) {
                  continue;
                }
                auto new_position_comp =
                    coordinator_->GetComponent<core::PositionComponent>(
                        new_entity);

                auto new_collision_comp =
                    coordinator_->GetComponent<core::CollisionComponent>(
                        new_entity);

                if (new_collision_comp.is_movable == 1) {
                  auto& new_movement_comp =
                      coordinator_->GetComponent<core::MovementComponent>(
                          new_entity);
                  new_position_comp.position += new_movement_comp.direction
                      * new_movement_comp.current_speed;
                }
                auto& new_graphics_comp = coordinator_->
                    GetComponent<core::GraphicsItemComponent>(new_entity);
                if (graphics_comp2.item->zValue()
                    != new_graphics_comp.item->zValue()) {
                  continue;
                }
                std::pair<double, double> inter =
                    IntersectPositions(new_position_comp, new_position2);
                if (inter.second > eps || inter.first > eps) {
                  can_move = false;
                  break;
                }
              }
              if (can_move) {
                position_comp2.position +=
                    QVector2D {1, 0} * movement_comp1.current_speed;
              } else {
                movement_comp1.direction = {0, 0};
              }
            } else if (collision_comp1.gravity && !collision_comp2.gravity) {
              new_position1.position +=
                  QVector2D {-1, 0} * movement_comp1.current_speed;

              if (movement_comp1.direction != QVector2D{0, 0}) {
                movement_comp2.direction = {0, 0};
                continue;
              }
              bool can_move = true;
              for (auto& new_entity : entities_) {
                if (new_entity == entity1 || new_entity == entity2) {
                  continue;
                }
                auto new_position_comp =
                    coordinator_->GetComponent<core::PositionComponent>(
                        new_entity);

                auto new_collision_comp =
                    coordinator_->GetComponent<core::CollisionComponent>(
                        new_entity);

                if (new_collision_comp.is_movable == 1) {
                  auto& new_movement_comp =
                      coordinator_->GetComponent<core::MovementComponent>(
                          new_entity);
                  new_position_comp.position += new_movement_comp.direction
                      * new_movement_comp.current_speed;
                }
                auto& new_graphics_comp = coordinator_->
                    GetComponent<core::GraphicsItemComponent>(new_entity);
                if (graphics_comp2.item->zValue()
                    != new_graphics_comp.item->zValue()) {
                  continue;
                }
                auto inter =
                    IntersectPositions(new_position_comp, new_position1);
                if (inter.second > eps || inter.first > eps) {
                  can_move = false;
                  break;
                }
              }
              if (can_move) {
                position_comp1.position -=
                    QVector2D{movement_comp2.current_speed, 0};
              } else {
                movement_comp2.direction = {0, 0};
              }
            } else {
              assert(1 == 0 && "bad horizontal collision");
            }
          }
        } else if (collision_comp1.gravity &&
            !collision_comp2.gravity &&
            intersection.second != 0) {
          if (intersection.second > intersection.first) {
            movement_comp2.direction = {0, 0};
          } else {
            movement_comp1.direction = {0, 0};
          }
        } else if (!collision_comp1.gravity &&
            collision_comp2.gravity &&
            intersection.second != 0) {
          if (intersection.second > intersection.first) {
            movement_comp1.direction = {0, 0};
          } else {
            movement_comp2.direction = {0, 0};
          }
        } else if (collision_comp1.gravity &&
            collision_comp2.gravity &&
            intersection.second != 0) {
          if (movement_comp1.direction == QVector2D {1, 0} ||
              movement_comp1.direction == QVector2D {-1, 0}) {
            movement_comp1.direction = {0, 0};
          } else if (movement_comp2.direction == QVector2D {1, 0} ||
              movement_comp2.direction == QVector2D {-1, 0}) {
            movement_comp2.direction = {0, 0};
          }
        } else {
          assert(1 == 0 && "bad collision");
        }
      }
    }
  }


  // start stones
  for (engine::Entity entity1 : movable_entities) {
    bool is_bad = false;
    auto& movement_comp1 =
        coordinator_->GetComponent<core::MovementComponent>(entity1);
    auto& collision_comp1 =
        coordinator_->GetComponent<core::CollisionComponent>(entity1);
    if (!collision_comp1.gravity) {
      break;
    }
    auto& position_comp1 = coordinator_->
        GetComponent<core::PositionComponent>(entity1);

    for (engine::Entity entity2 : entities_) {
      if (entity1 == entity2) {
        continue;
      }
      auto& graphics_comp1 = coordinator_->
          GetComponent<core::GraphicsItemComponent>(entity1);

      auto& graphics_comp2 = coordinator_->
          GetComponent<core::GraphicsItemComponent>(entity2);
      if (graphics_comp1.item->zValue() != graphics_comp2.item->zValue()) {
        continue;
      }
      auto& position_comp2 = coordinator_->
          GetComponent<core::PositionComponent>(entity2);

      auto& collision_comp2 =
          coordinator_->GetComponent<core::CollisionComponent>(entity2);

      if (!collision_comp2.is_movable) {
        auto new_position = position_comp1;
        new_position.position += {0, movement_comp1.current_speed};
        const long double eps = 1e-1;
        if (IntersectPositions(new_position, position_comp2).second > eps ||
            IntersectPositions(new_position, position_comp2).first > eps) {
          movement_comp1.direction = {0, 0};
          is_bad = true;
          break;
        }
      } else {
        auto& movement_comp2 =
            coordinator_->GetComponent<core::MovementComponent>(entity2);

        auto new_position1 = position_comp1;
        new_position1.position += {0, movement_comp1.current_speed};
        auto new_position2 = position_comp2;
        new_position2.position +=
            movement_comp2.direction * movement_comp2.current_speed;

        std::pair<double, double>
            intersection = IntersectPositions(new_position1, new_position2);
        if (intersection.first == 0 && intersection.second == 0) {
          continue;
        }
        is_bad = true;
        break;
      }
    }
    if (!is_bad) {
      movement_comp1.direction = {0, 1};
    }
  }
}
