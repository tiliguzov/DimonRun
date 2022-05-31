#pragma once

#include <array>
#include <queue>

#include "types_and_constants.h"

namespace engine {

class EntityManager {
 public:
  EntityManager();
  Entity CreateEntity();
  void DestroyEntity(Entity entity);
  void SetComponentSignature(
      Entity entity, ComponentSignature new_component_signature);
  ComponentSignature GetComponentSignature(Entity entity) const;

  int GetEntityAliveCount();

 private:
  std::array<ComponentSignature, kMaxEntities> component_signatures_;
  std::queue<Entity> available_entities_;
  Entity used_entities_count_;
};

}  // namespace engine
