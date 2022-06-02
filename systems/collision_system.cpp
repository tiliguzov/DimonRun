#include "collision_system.h"
#include "core/constants.h"

#include <algorithm>
#include <iostream>
#include <assert.h>


namespace {

bool IsIntersectPositions(
        core::PositionComponent& position1,
        core::PositionComponent& position2) {
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
        core::PositionComponent& position1,
        core::PositionComponent& position2) {
    if (!IsIntersectPositions(position1, position2)) {
        return {0, 0};
    }
    double first_x = position1.position.x(), first_y = position1.position.y();
    double second_x = position2.position.x(), second_y = position2.position.y();
    if (first_x > second_x) {
        std::swap(first_x, second_x);
        std::swap(first_y, second_y);
    }
    return {std::min(first_x, second_x) - std::max(first_x, second_x) + core::kTextureSize,
            std::abs(std::min(first_y, second_y) - std::max(first_y, second_y) + core::kTextureSize)};
}

}


systems::CollisionSystem::CollisionSystem(engine::Coordinator *coordinator, core::Connector* connector)
    :coordinator_(coordinator), connector_(connector) {}

void systems::CollisionSystem::Update() {
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
        new_position1.position += movement_comp1.direction * movement_comp1.current_speed;
        const long double eps = 1e-1;
        if (collision_comp1.can_use && collision_comp2.is_usable &&
                (IntersectPositions(new_position1, position_comp2).first > eps ||
                 IntersectPositions(new_position1, position_comp2).second > eps)) {

            std::cout << "THIS IS USSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSEEEEEEEEEEEEEEEEEEEEEEE" << std::endl;
            continue;
        }
        if (collision_comp1.can_use && collision_comp2.is_breakable &&
                (IntersectPositions(new_position1, position_comp2).first > eps ||
                 IntersectPositions(new_position1, position_comp2).second > eps)) {
            // TODO: set illness. start animation
            std::cout << "THIS IS ILNEEEEEEEEEEEEEEEEEEEEEEEESSSSSSSSSSSSSSSSSSS" << std::endl;
            continue;
        }
        // vertical collisions
        if (collision_comp2.is_movable == false) {
            std::cout << "inter :" << IntersectPositions(new_position1, position_comp2).first << " " << IntersectPositions(new_position1, position_comp2).second << std::endl;
            if (IntersectPositions(new_position1, position_comp2).second > eps) {
                movement_comp1.direction = {0, 0};
                std::cout << "collision with wall!" << std::endl;
            }
        } else {
            auto& movement_comp2 = coordinator_->GetComponent<core::MovementComponent>(entity2);

            auto new_position2 = position_comp2;
            new_position2.position += movement_comp2.direction * movement_comp2.current_speed;
            const long double eps = 1e-1;
            std::pair<double, double> intersection = IntersectPositions(new_position1, new_position2);
            if (intersection.first < eps) {
                intersection.first = 0;
            }
            if (intersection.second < eps) {
                intersection.second = 0;
            }
            if (intersection.first == 0 || intersection.second == 0) {
                std::cout << "gg" << std::endl;
                continue;
            }

            std::cout << "iiiintttttttttttttttttttttttttttttttttttteeeeeeeeeeer :" << intersection.first << " " << intersection.second << std::endl;

            // vertical collisions
            if (intersection.first == core::kTextureSize) {
                if (new_position1.position.y() < new_position2.position.y()) {
                    if (collision_comp1.gravity && collision_comp2.gravity) {
                        if (movement_comp1.direction == QVector2D{0, 0}) {
                            position_comp1.position -= {0.0, movement_comp1.current_speed};
                        }
                        movement_comp1.direction = {0, 0};
                    } else if (collision_comp1.gravity && !collision_comp2.gravity) {


                        if (std::abs(position_comp1.position.y() - std::round(position_comp1.position.y())) > eps ||
                                (std::abs(position_comp1.position.y() - std::round(position_comp1.position.y())) < eps &&
                                static_cast<int>(std::round(position_comp1.position.y())) % core::kTextureSize != 0)) {
                            position_comp2.position += {0.0, movement_comp2.current_speed};
                            movement_comp2.direction = {0.0, 0.0};
                        } else {
                            movement_comp1.direction = {0.0, 0.0};
                            movement_comp2.direction = {0.0, 0.0};
                        }
                    } else {
                        position_comp1.position -= movement_comp1.direction * movement_comp1.current_speed;
                    }
                }
            } else if (intersection.second == core::kTextureSize) {
                if (new_position1.position.x() < new_position2.position.x()) {
                    std::cout << "left :" << entity1 << " right " << entity2 << std::endl;
                    new_position2.position += QVector2D{1, 0} * movement_comp1.current_speed;

                    if (!collision_comp1.gravity && collision_comp2.gravity) {
                        if (movement_comp2.direction != QVector2D{0, 0}) {
                            movement_comp1.direction = {0, 0};
                            continue;
                        }
                        bool can_move = true;
                        for (auto& new_entity : entities_) {
                            if (new_entity == entity1 || new_entity == entity2) {
                                continue;
                            }
                            auto new_position_comp = coordinator_->GetComponent<core::PositionComponent>(new_entity);

                            auto new_collison_comp = coordinator_->GetComponent<core::CollisionComponent>(new_entity);

                            if (new_collison_comp.is_movable == 1) {
                                auto& new_movement_comp = coordinator_->GetComponent<core::MovementComponent>(new_entity);
                                new_position_comp.position += new_movement_comp.direction * new_movement_comp.current_speed;
                            }
                            auto& new_graphics_comp = coordinator_->
                                    GetComponent<core::GraphicsItemComponent>(new_entity);
                            if (graphics_comp2.item->zValue() != new_graphics_comp.item->zValue()) {
                                continue;
                            }
                            std::pair<double, double> inter = IntersectPositions(new_position_comp, new_position2);
                            std::cout << "STONEEEEEEEEEEEEEEEEEEEEEEEEEEEEE " << inter.first << " " << inter.second << std::endl;
                            std::cout << new_position_comp.position.x() << " " << new_position_comp.position.y() << std::endl;
                            std::cout << new_position2.position.x() << " " << new_position2.position.y() << std::endl;
                            if (inter.second > eps || inter.first > eps) {
                                can_move = false;
                                break;
                            }
                        }
                        if (can_move) {
                            position_comp2.position += QVector2D{1, 0} * movement_comp1.current_speed;
                        } else {
                            movement_comp1.direction = {0, 0};
                        }
                    } else if (collision_comp1.gravity && !collision_comp2.gravity) {
                        if (movement_comp1.direction != QVector2D{0, 0}) {
                            movement_comp2.direction = {0, 0};
                            continue;
                        }
                        bool can_move = true;
                        for (auto& new_entity : entities_) {
                            if (new_entity == entity1 || new_entity == entity2) {
                                continue;
                            }
                            auto new_position_comp = coordinator_->GetComponent<core::PositionComponent>(new_entity);

                            auto new_collison_comp = coordinator_->GetComponent<core::CollisionComponent>(new_entity);

                            if (new_collison_comp.is_movable == 1) {
                                auto& new_movement_comp = coordinator_->GetComponent<core::MovementComponent>(new_entity);
                                new_position_comp.position += new_movement_comp.direction * new_movement_comp.current_speed;
                            }
                            auto& new_graphics_comp = coordinator_->
                                    GetComponent<core::GraphicsItemComponent>(new_entity);
                            if (graphics_comp2.item->zValue() != new_graphics_comp.item->zValue()) {
                                continue;
                            }
                            std::pair<double, double> inter = IntersectPositions(new_position_comp, new_position1);
                            if (inter.second > eps || inter.first > eps) {
                                can_move = false;
                                break;
                            }
                        }
                        if (can_move) {
                            position_comp1.position -= QVector2D{1, 0} * movement_comp2.current_speed;
                        } else {
                            movement_comp2.direction = {0, 0};
                        }
                    } else {
                        assert(1 == 0 && "bad horizontal collision");
                    }
                }
            } else if (collision_comp1.gravity && !collision_comp2.gravity && intersection.second != 0) {
                if (intersection.second > intersection.first) {
                    movement_comp2.direction = {0, 0};
                } else {
                    movement_comp1.direction = {0, 0};
                }
            } else if (!collision_comp1.gravity && collision_comp2.gravity && intersection.second != 0) {
                if (intersection.second > intersection.first) {
                    movement_comp1.direction = {0, 0};
                } else {
                    movement_comp2.direction = {0, 0};
                }
            } else if (collision_comp1.gravity && collision_comp2.gravity && intersection.second != 0) {
                if (movement_comp1.direction == QVector2D{1, 0} || movement_comp1.direction == QVector2D{-1, 0}) {
                    movement_comp1.direction = {0, 0};
                } else if (movement_comp2.direction == QVector2D{1, 0} || movement_comp2.direction == QVector2D{-1, 0}) {
                    movement_comp2.direction = {0, 0};
                }
            } else {
                std::cout << entity1 << " " << entity2 << std::endl;
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
      std::cout << "grav : " << entity1 << " " << collision_comp1.gravity << std::endl;
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


        if (collision_comp2.is_movable == false) {
            auto new_position = position_comp1;
            new_position.position += {0, movement_comp1.current_speed};
            const long double eps = 1e-1;
            if (IntersectPositions(new_position, position_comp2).second > eps) {
                movement_comp1.direction = {0, 0};
                is_bad = true;
                break;
            }
        } else {
            auto& movement_comp2 = coordinator_->GetComponent<core::MovementComponent>(entity2);

            auto new_position1 = position_comp1;
            new_position1.position += {0, movement_comp1.current_speed};
            auto new_position2 = position_comp2;
            new_position2.position += movement_comp2.direction * movement_comp2.current_speed;

            std::pair<double, double> intersection = IntersectPositions(new_position1, new_position2);
            if (intersection.first == 0 && intersection.second == 0) {
                continue;
            }
            is_bad = true;
            break;
        }
      }
      const long double eps = 1e-1;
      if (!is_bad) {
          movement_comp1.direction = {0, 1};
      }
   }
}
