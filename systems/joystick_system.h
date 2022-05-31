#pragma once

#include <QVector2D>

#include "core/components.h"
#include "core/constants.h"
#include "core/keyboard.h"
#include "engine/coordinator.h"

namespace systems {

class JoystickSystem : public engine::System {
 public:
  JoystickSystem(engine::Coordinator* coordinator, core::Keyboard* keyboard);
  void Update() override;

 private:
  engine::Coordinator* coordinator_;
  core::Keyboard* keyboard_;
};

}  // namespace systems
