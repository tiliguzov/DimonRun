#pragma once

#include <QKeyEvent>

#include "constants.h"

namespace core {

class AbstractScene {
 public:
  virtual void OpenFastMenu() = 0;
  virtual void ContinueGame() = 0;
  virtual void OpenVault(std::string) = 0;
  virtual void OpenScroll(std::string) = 0;

  virtual void OpenNewDungeon(DungeonName) = 0;
};

}  // namespace core
