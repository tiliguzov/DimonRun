#include "illness_system.h"

#include <vector>

systems::IllnessSystem::IllnessSystem(engine::Coordinator* coordinator,
                                      core::Connector* connector)
    : coordinator_(coordinator), connector_(connector) {}

void systems::IllnessSystem::Update() {
  std::vector<engine::Entity> to_del;
  for (auto entity : entities_) {
    auto& illness_comp =
        coordinator_->GetComponent<core::IllnessComponent>(entity);
    if (!illness_comp.is_ill) {
      continue;
    }
    illness_comp.kill_time -= core::kTickTime;
    if (illness_comp.kill_time <= 0) {
      to_del.push_back(entity);
    }
  }
  for (auto entity : to_del) {
    connector_->DeleteEntity(entity);
  }
}


