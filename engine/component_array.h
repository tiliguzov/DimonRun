#pragma once

#include <unordered_map>

#include "types_and_constants.h"

namespace engine {

class IComponentArray {
 public:
  virtual ~IComponentArray() = default;
  virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename ComponentType>
class ComponentArray : public IComponentArray {
 public:
  ComponentArray();

  void InsertComponent(Entity entity, const ComponentType& new_component);
  ComponentType& GetComponent(Entity entity);

  void DeleteComponent(Entity entity_of_deleted);
  void EntityDestroyed(Entity entity) override;

 private:
  std::array<ComponentType, kMaxEntities> components_;
  std::unordered_map<Entity, id_type> index_by_entity_;
  std::unordered_map<id_type, Entity> entity_by_index_;
  id_type size_;
};

template<typename ComponentType>
ComponentArray<ComponentType>::ComponentArray() : size_(0) {}

template<typename ComponentType>
void ComponentArray<ComponentType>::InsertComponent(
    Entity entity,
    const ComponentType& new_component) {
  assert(entity < kMaxEntities &&
      "Try to insert component by entity not in range [0, kMaxEntities)");

  components_[size_] = new_component;
  index_by_entity_.emplace(entity, size_);
  entity_by_index_.emplace(size_, entity);
  size_++;
}

template<typename ComponentType>
ComponentType& ComponentArray<ComponentType>::GetComponent(Entity entity) {
  assert(entity < kMaxEntities &&
      "Try to get component by entity not in range [0, kMaxEntities)");
  // assert(index_by_entity_.find(entity) != index_by_entity_.end() &&
  //     "Try to get not existing component by entity");

  return components_[index_by_entity_[entity]];
}

template<typename ComponentType>
void ComponentArray<ComponentType>::DeleteComponent(Entity entity_of_deleted) {
  assert(entity_of_deleted < kMaxEntities &&
      "Try to delete component by entity not in range [0, kMaxEntities)");
  assert(index_by_entity_.find(entity_of_deleted) != index_by_entity_.end() &&
      "Try to delete not existing component by entity");

  id_type index_of_deleted = index_by_entity_[entity_of_deleted];

  id_type index_of_replaced = size_ - 1;
  Entity entity_of_replaced = entity_by_index_[index_of_replaced];

  components_[index_of_deleted] = components_[index_of_replaced];
  entity_by_index_[index_of_deleted] = entity_of_replaced;
  index_by_entity_[entity_of_replaced] = index_of_deleted;

  entity_by_index_.erase(index_of_deleted);
  index_by_entity_.erase(entity_of_deleted);
  size_--;
}

template<typename ComponentType>
void ComponentArray<ComponentType>::EntityDestroyed(Entity entity) {
  if (index_by_entity_.find(entity) != index_by_entity_.end()) {
    DeleteComponent(entity);
  }
}

}  // namespace engine
