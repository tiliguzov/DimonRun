#pragma once

#include <unordered_map>

#include "i_component_array.h"

template <typename ComponentType>
class ComponentArray : public IComponentArray {
 public:
  ComponentArray();
  void InsertComponent(Entity, const ComponentType&);  // todo why const& ?
  void DeleteComponent(Entity);
  void EntityDestroyed(Entity) override;
  ComponentType& GetComponent(Entity);
  // todo some void IsEntityValid(Entity) checker (exceptions?)
  // todo do we need bool IsComponentOfEntityExist(Entity) function ?
  // todo do we need destructor ?
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
  // todo check entity
  components_[size_] = new_component;
  index_by_entity_.emplace(entity, size_);
  entity_by_index_.emplace(size_, entity);
  size_++;
}

template<typename ComponentType>
void ComponentArray<ComponentType>::DeleteComponent(Entity entity_of_deleted) {
  // todo check entity
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
ComponentType& ComponentArray<ComponentType>::GetComponent(Entity entity) {
  // todo check
  return components_[index_by_entity_[entity]];
}

template<typename ComponentType>
void ComponentArray<ComponentType>::EntityDestroyed(Entity entity) {
  if (index_by_entity_.find(entity) != index_by_entity_.end()) {
    DeleteComponent(entity);
  }
}
