#include "component_manager.h"

namespace engine {

ComponentManager::ComponentManager() : first_unregistered_id_(0) {}

void ComponentManager::EntityDestroyed(Entity entity) {
  for (auto[component_index, component_array] : component_array_by_index_) {
    component_array->EntityDestroyed(entity);
  }
}

}  // namespace engine
