#include "coordinator.h"

#include <memory>

namespace engine {

Coordinator::Coordinator() :
    entity_manager_(std::make_unique<EntityManager>()),
    component_manager_(std::make_unique<ComponentManager>()),
    system_manager_(std::make_unique<SystemManager>()) {}

Entity Coordinator::CreateEntity() {
  return entity_manager_->CreateEntity();
}

void Coordinator::DestroyEntity(Entity entity) {
  entity_manager_->DestroyEntity(entity);
  component_manager_->EntityDestroyed(entity);
  system_manager_->EntityDestroyed(entity);
}

ComponentSignature Coordinator::GetComponentSignature(Entity entity) const {
  return entity_manager_->GetComponentSignature(entity);
}

void Coordinator::SetComponentSignature(Entity entity,
                                        ComponentSignature new_signature) {
  entity_manager_->SetComponentSignature(entity, new_signature);
}

}  // namespace engine
