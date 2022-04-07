#include "keyboard.h"

namespace core {

bool Keyboard::IsKeyPressed(KeyAction bind) const {
  if (is_key_pressed_.find(bindings_.at(bind)) == is_key_pressed_.end()) {
    return false;
  }
  return is_key_pressed_.at(bindings_.at(bind));
}

void Keyboard::OnPress(Qt::Key key) {
  if (key == Qt::Key_D
          || key == Qt::Key_A
          || key == Qt::Key_W
          || key == Qt::Key_S) {
    OnRelease(Qt::Key_D);
    OnRelease(Qt::Key_A);
    OnRelease(Qt::Key_W);
    OnRelease(Qt::Key_S);
  }
  is_key_pressed_[key] = true;
}

void Keyboard::OnRelease(Qt::Key key) {
  is_key_pressed_[key] = false;
}

Keyboard::Keyboard() : is_blocked_(false) {
  bindings_ = {
      {KeyAction::kMoveRight, Qt::Key_D},
      {KeyAction::kMoveLeft, Qt::Key_A},
      {KeyAction::kMoveUp, Qt::Key_W},
      {KeyAction::kMoveDown, Qt::Key_S}
  };
}

void Keyboard::Block() {
  is_blocked_ = true;
}

void Keyboard::Unblock() {
  is_blocked_ = false;
}

bool Keyboard::IsBlocked() const {
  return is_blocked_;
}

} // namespace core
