#pragma once

#include <array>
#include <queue>

#include "types_and_constants.h"

class EntityManager {
 public:
  EntityManager() ;
  Entity CreateEntity();
  void DestroyEntity(Entity);
  void SetComponentSignature(Entity, ComponentSignature);
  ComponentSignature GetComponentSignature(Entity) const;
 private:
  std::array<ComponentSignature, kMaxEntities> component_signatures_;
  std::queue<Entity> available_entities_;
  Entity used_entities_count_;
};
