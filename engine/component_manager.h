#pragma once

#include <cassert>
#include <memory>
#include <typeindex>
#include <unordered_map>

#include "component_array.h"
#include "types_and_constants.h"

class ComponentManager {
 public:
  ComponentManager();

  template<typename ComponentType>
  void RegisterComponent();

  template<typename ComponentType>
  ComponentID GetComponentID();

  template<typename ComponentType>
  void AddComponent(Entity entity, const ComponentType& component);

  template<typename ComponentType>
  void DeleteComponent(Entity entity);

  template<typename ComponentType>
  ComponentType& GetComponent(Entity entity);

  void EntityDestroyed(Entity entity);

 private:
  std::unordered_map<std::type_index, ComponentID> component_id_by_index_;
  std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>>
      component_array_by_index_;
  ComponentID first_unregistered_id_;

 private:
  template<typename ComponentType>
  std::shared_ptr<ComponentArray<ComponentType>> GetComponentArray();
};

template<typename ComponentType>
void ComponentManager::RegisterComponent() {
  auto component_index = std::type_index(typeid(ComponentType));

  assert(component_id_by_index_.find(component_index)
             == component_id_by_index_.end()
             && "Try to register component that was already registered");

  component_id_by_index_.emplace(component_index, first_unregistered_id_);
  component_array_by_index_.emplace(component_index,
                                    std::make_shared<ComponentArray<
                                        ComponentType>>());
  first_unregistered_id_++;
}

template<typename ComponentType>
ComponentID ComponentManager::GetComponentID() {
  auto component_index = std::type_index(typeid(ComponentType));

  assert(component_id_by_index_.find(component_index)
             != component_id_by_index_.end()
             && "Try to get component by invalid component type");

  return component_id_by_index_[component_index];
}

template<typename ComponentType>
void ComponentManager::AddComponent(Entity entity,
                                    const ComponentType& component) {
  GetComponentArray<ComponentType>()->InsertComponent(entity, component);
}

template<typename ComponentType>
void ComponentManager::DeleteComponent(Entity entity) {
  GetComponentArray<ComponentType>()->DeleteComponent(entity);
}

template<typename ComponentType>
ComponentType& ComponentManager::GetComponent(Entity entity) {
  return GetComponentArray<ComponentType>()->GetComponent(entity);
}

template<typename ComponentType>
std::shared_ptr<ComponentArray<ComponentType>>
ComponentManager::GetComponentArray() {
  auto component_index = std::type_index(typeid(ComponentType));

  assert(component_array_by_index_.find(component_index)
             != component_array_by_index_.end()
             && "Try to get component array by invalid component type");

  return std::dynamic_pointer_cast<ComponentArray<ComponentType>>(
      component_array_by_index_[component_index]);
}
