#include "component_manager.h"

ComponentManager::ComponentManager() : next_component_id_(0) {}

void ComponentManager::EntityDestroyed(Entity entity) {
  for (auto [component_name, component_array] : component_array_by_name_) {
    component_array->EntityDestroyed(entity);
  }
}
