#include "system_manager.h"

void SystemManager::EntityDestroyed(Entity entity) {
  for (auto const &[index, system] : system_by_index_) {
    system->entities_.erase(entity);
  }
}

void SystemManager::EntitySignatureChanged(
    Entity entity,
    ComponentSignature new_signature) {
  for (auto const &[index, system] : system_by_index_) {
    auto const& system_signature = signature_by_index_[index];

    if ((new_signature & system_signature) == system_signature) {
      system->entities_.insert(entity);
    } else {
      system->entities_.erase(entity);
    }
  }
}
