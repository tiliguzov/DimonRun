#pragma once

#include <unordered_map>
#include <memory>

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
  void AddComponent(Entity, ComponentType*);

  template<typename ComponentType>
  void DeleteComponent(Entity);

  template<typename ComponentType>
  ComponentType& GetComponent(Entity);  // todo & or * or smart_ptr

  void EntityDestroyed(Entity);

 private:
  std::unordered_map<const char*, ComponentID> component_id_by_name_;
  std::unordered_map<const char*, IComponentArray*> component_array_by_name_;  // todo think about shared_ptr later
  ComponentID next_component_id_;

 private:
  template<typename ComponentType>
  ComponentArray<ComponentType>* GetComponentArray();
};

template<typename ComponentType>
void ComponentManager::RegisterComponent() {
  const char* component_name = typeid(ComponentType).name();
  // todo maybe create IComponent class with static field ComponentID type
  // todo component already registered check
  component_id_by_name_.emplace(component_name, next_component_id_);
  component_array_by_name_.emplace(component_name, std::static_pointer_cast<IComponentArray>(new ComponentArray<ComponentType>));
  next_component_id_++;
}

template<typename ComponentType>
ComponentID ComponentManager::GetComponentID() {
  const char* component_name = typeid(ComponentType).name();
  // todo valid component name check
  return component_id_by_name_[component_name];
}

template<typename ComponentType>
void ComponentManager::AddComponent(Entity entity, ComponentType* component) {
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
ComponentArray<ComponentType>* ComponentManager::GetComponentArray()  {
  const char* component_name = typeid(ComponentType).name();
  // todo component out of range check
  return std::static_pointer_cast<ComponentArray<ComponentType>>(
      component_array_by_name_[component_name]);
}
