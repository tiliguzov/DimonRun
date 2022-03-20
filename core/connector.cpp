#include "connector.h"
#include "components.h"
#include "systems/movement_system.h"

Connector::Connector() : coordinator_(std::make_unique<Coordinator>()) {
  RegisterComponents();
  RegisterSystems();
  Entity entity = coordinator_->CreateEntity();
  coordinator_->AddComponent(entity, MovementComponent{{0, 0}});
}

void Connector::OnTick() {
  for (auto system : systems_) {
    system->Update();
  }
}

void Connector::RegisterSystems() {
  // Add all systems
  auto movement_system = coordinator_
      ->RegisterSystem<MovementSystem>(coordinator_.get());

  coordinator_->SetSystemSignature<MovementSystem>
      ({coordinator_->GetComponentID<MovementComponent>()});

  systems_.push_back(movement_system);
}
void Connector::RegisterComponents() {
  // Add all components
  coordinator_->RegisterComponent<MovementComponent>();
}
