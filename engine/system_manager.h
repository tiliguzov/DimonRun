#pragma once

#include <cassert>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>

#include "system.h"
#include "types_and_constants.h"

class SystemManager {
 public:
  template<typename SystemType, typename... Args>
  std::shared_ptr<SystemType> RegisterSystem(Args&& ...);

  template<typename SystemType>
  void SetSignature(const ComponentSignature& signature);

  void EntityDestroyed(Entity entity);
  void EntitySignatureChanged(Entity entity, ComponentSignature new_signature);

 private:
  std::unordered_map<std::type_index, ComponentSignature> signature_by_index_;
  std::unordered_map<std::type_index, std::shared_ptr<System>> system_by_index_;
};

template<typename SystemType, typename... Args>
std::shared_ptr<SystemType> SystemManager::RegisterSystem(Args&& ... args) {
  auto index = std::type_index(typeid(SystemType));

  assert(system_by_index_.find(index) == system_by_index_.end()
             && "Try to register system that was already registered");

  auto system = std::make_shared<SystemType>(std::forward<Args>(args) ...);
  system_by_index_.emplace(index, system);
  return system;
}

template<typename SystemType>
void SystemManager::SetSignature(const ComponentSignature& signature) {
  auto index = std::type_index(typeid(SystemType));

  assert(system_by_index_.find(index) != system_by_index_.end()
             && "System used before registered");

  signature_by_index_.emplace(index, signature);
}
