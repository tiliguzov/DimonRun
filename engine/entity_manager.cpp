#include <cassert>

#include "entity_manager.h"

EntityManager::EntityManager() : used_entities_count_(0) {
  for(int entity = 0; entity < kMaxEntities; entity++) {
    available_entities_.push(entity);
  }
}

Entity EntityManager::CreateEntity() {
  assert(used_entities_count_ + 1 <= kMaxEntities
      && "Try to create entities more than kMaxEntities");
  Entity new_entity = available_entities_.front();
  available_entities_.pop();
  used_entities_count_++;
  return new_entity;
}

void EntityManager::DestroyEntity(Entity entity) {
  assert(entity < kMaxEntities
      && "Try to destroy entity not in range [0, kMaxEntities)");
  available_entities_.push(entity);
  component_signatures_[entity].reset();
  used_entities_count_--;
}

void EntityManager::SetComponentSignature(
    Entity entity,
    ComponentSignature new_component_signature) {
  assert(entity < kMaxEntities
      && "Try to set signature to entity not in range [0, kMaxEntities)");
  component_signatures_[entity] = new_component_signature;
}

ComponentSignature EntityManager::GetComponentSignature(Entity entity) const {
  assert(entity < kMaxEntities
      && "Try to get signature of entity not in range [0, kMaxEntities)");
  return component_signatures_[entity];
}
