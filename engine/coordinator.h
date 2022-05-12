#pragma once

#include <memory>
#include <utility>

#include "component_manager.h"
#include "entity_manager.h"
#include "system_manager.h"

namespace engine {

class Coordinator {
 public:
  Coordinator();

  Entity CreateEntity();
  void DestroyEntity(Entity entity);

  template<typename ComponentType>
  void RegisterComponent();

  template<typename ComponentType>
  void AddComponent(Entity entity, const ComponentType& component);

  template<typename ComponentType>
  void RemoveComponent(Entity entity);

  template<typename ComponentType>
  ComponentType& GetComponent(Entity entity) const;

  template<typename ComponentType>
  ComponentID GetComponentID() const;

  template<typename SystemType, typename... Args>
  std::shared_ptr<SystemType> RegisterSystem(Args&& ... args);

  template<typename SystemType>
  void SetSystemSignature(
      std::initializer_list<ComponentID> included_components);

  template<typename ComponentType>
  void UpdateSignature(Entity entity, bool has_component);

  ComponentSignature GetComponentSignature(Entity entity) const;
  void SetComponentSignature(Entity entity, ComponentSignature new_signature);

  template<typename ComponentType>
  bool HasComponent(Entity entity) const;

 private:
  std::unique_ptr<ComponentManager> component_manager_;
  std::unique_ptr<EntityManager> entity_manager_;
  std::unique_ptr<SystemManager> system_manager_;
};

template<typename ComponentType>
void Coordinator::RegisterComponent() {
  component_manager_->RegisterComponent<ComponentType>();
}

template<typename ComponentType>
void Coordinator::AddComponent(Entity entity, const ComponentType& component) {
  component_manager_->AddComponent<ComponentType>(entity, component);
  UpdateSignature<ComponentType>(entity, true);
}

template<typename ComponentType>
void Coordinator::RemoveComponent(Entity entity) {
  component_manager_->DeleteComponent<ComponentType>(entity);
  UpdateSignature<ComponentType>(entity, false);
}

template<typename ComponentType>
ComponentType& Coordinator::GetComponent(Entity entity) const {
  return component_manager_->GetComponent<ComponentType>(entity);
}

template<typename ComponentType>
ComponentID Coordinator::GetComponentID() const {
  return component_manager_->GetComponentID<ComponentType>();
}

template<typename SystemType, typename... Args>
std::shared_ptr<SystemType> Coordinator::RegisterSystem(Args&& ... args) {
  return system_manager_->RegisterSystem<SystemType>(
      std::forward<Args>(args)...);
}

template<typename SystemType>
void Coordinator::SetSystemSignature
    (std::initializer_list<ComponentID> included_components) {
  ComponentSignature signature;
  for (const auto& pos : included_components) {
    signature.set(pos);
  }
  system_manager_->SetSignature<SystemType>(signature);
}

template<typename ComponentType>
void Coordinator::UpdateSignature(Entity entity, bool has_component) {
  auto signature = entity_manager_->GetComponentSignature(entity);
  signature.set(component_manager_->GetComponentID<ComponentType>(),
                has_component);

  entity_manager_->SetComponentSignature(entity, signature);
  system_manager_->EntitySignatureChanged(entity, signature);
}

template<typename ComponentType>
bool Coordinator::HasComponent(Entity entity) const {
  auto signature = entity_manager_->GetComponentSignature(entity);
  ComponentID pos = component_manager_->GetComponentID<ComponentType>();
  return signature[pos];
}

}  // namespace engine
