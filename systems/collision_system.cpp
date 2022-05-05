#include "collision_system.h"
#include "core/constants.h"

#include <algorithm>
#include <iostream>
systems::CollisionSystem::CollisionSystem(engine::Coordinator *coordinator)
    :coordinator_(coordinator) {}

void systems::CollisionSystem::Update() {
   std::vector<engine::Entity> movable_entities;
   for (engine::Entity entity : entities_) {
       auto& collision_comp =
               coordinator_->GetComponent<core::CollisionComponent>(entity);
       if (collision_comp.IsMovable) {
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

        core::PositionComponent position1 = position_comp1;
        core::PositionComponent position2 = position_comp2;

        if (IsIntersectPositions(position1, position2)) {
            if ((static_cast<int>(position_comp1.position.x()) + 1)
                    % core::kTextureSize == 0) {
                position_comp1.position.setX(position_comp1.position.x() + 1);
            }
            if ((static_cast<int>(position_comp1.position.x()) - 1)
                    % core::kTextureSize == 0) {
                position_comp1.position.setX(position_comp1.position.x() - 1);
            }
            if ((static_cast<int>(position_comp1.position.y()) + 1)
                    % core::kTextureSize == 0) {
                position_comp1.position.setY(position_comp1.position.y() + 1);
            }
            if ((static_cast<int>(position_comp1.position.y()) - 1)
                    % core::kTextureSize == 0) {
                position_comp1.position.setY(position_comp1.position.y() - 1);
            }

        }
      }
   }
}

bool systems::CollisionSystem::IsIntersectPositions(
        core::PositionComponent& position1,
        core::PositionComponent& position2) {
    int first_x = position1.position.x(), first_y = position1.position.y();
    int second_x = position2.position.x(), second_y = position2.position.y();
    if (first_x > second_x) {
        std::swap(first_x, second_x);
        std::swap(first_y, second_y);
    }
    return (first_x + 50 > second_x && abs(first_y - second_y) < 50);
}


