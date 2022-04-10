#pragma once

#include <unordered_map>
#include <vector>
#include <QObject>

namespace core {

enum class KeyAction {
  kMoveUp,
  kMoveDown,
  kMoveLeft,
  kMoveRight
};

class Keyboard : public QObject {
 public:
  Keyboard();

  void OnPress(Qt::Key);
  void OnRelease(Qt::Key);

  bool IsKeyPressed(KeyAction bind) const;

  void Block();
  void Unblock();
  bool IsBlocked() const;

 private:
  bool is_blocked_;
  Qt::Key last_pressed_movement_key_;

  std::vector<Qt::Key> movement_keys_history_;
  std::unordered_map<Qt::Key, bool> is_key_pressed_;
  std::unordered_map<KeyAction, Qt::Key> bindings_;
};

}  // namespace core
