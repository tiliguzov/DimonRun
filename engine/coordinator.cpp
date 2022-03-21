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

}  // namespace engine
