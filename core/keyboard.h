#pragma once

#include <QObject>

#include <unordered_map>
#include <vector>

namespace core {

enum class KeyAction {
  kE,
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

 private:
  bool is_blocked_;

  std::unordered_map<Qt::Key, bool> is_key_pressed_;
  std::unordered_map<KeyAction, Qt::Key> bindings_;
};

}  // namespace core
