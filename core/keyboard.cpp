#include "keyboard.h"

namespace core {

bool Keyboard::IsKeyPressed(KeyAction bind) const {
  if (is_key_pressed_.find(bindings_.at(bind)) == is_key_pressed_.end()) {
    return false;
  }
  return is_key_pressed_.at(bindings_.at(bind));
}

void Keyboard::OnPress(Qt::Key key) {
  is_key_pressed_[key] = true;
}

void Keyboard::OnRelease(Qt::Key key) {
    is_key_pressed_[key] = false;
}

Keyboard::Keyboard() : is_blocked_(false) {
  bindings_ = {
      {KeyAction::kE, Qt::Key_E},
      {KeyAction::kMoveRight, Qt::Key_D},
      {KeyAction::kMoveLeft, Qt::Key_A},
      {KeyAction::kMoveUp, Qt::Key_W},
      {KeyAction::kMoveDown, Qt::Key_S}
  };
}

}  // namespace core
