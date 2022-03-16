#include "entity_manager.h"

EntityManager::EntityManager() : used_entities_count_(0) {
  for(int entity = 0; entity < kMaxEntities; entity++) {  // todo maybe from 1
    available_entities_.push(entity);
  }
}

Entity EntityManager::CreateEntity() {
  // todo entities overflow check
  Entity new_entity = available_entities_.front();
  available_entities_.pop();
  used_entities_count_++;
  return new_entity;
}

void EntityManager::DestroyEntity(Entity entity) {
  // todo entity out_of_range check
  available_entities_.push(entity);
  component_signatures_[entity].reset();  // todo ?
  used_entities_count_--;
}

void EntityManager::SetComponentSignature(
    Entity entity,
    ComponentSignature new_component_signature) {
  // todo entity out_of_range check
  component_signatures_[entity] = new_component_signature;
}

ComponentSignature EntityManager::GetComponentSignature(Entity entity) const {
  // todo entity out_of_range check
  return component_signatures_[entity];
}
