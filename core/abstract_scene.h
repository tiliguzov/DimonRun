#pragma once

#include <QKeyEvent>

#include "constants.h"

namespace core {

class AbstractScene {
 public:
  virtual void OpenFastMenu() = 0;
  virtual void ContinueGame() = 0;
  virtual void OpenVault() = 0;

  virtual DungeonName GetCurrentDungeon() = 0;
  virtual void SetCurrentDungeon(DungeonName) = 0;

  virtual void DownloadDungeon(DungeonName dungeon_name,
                               DungeonType dungeon_type) = 0;
  virtual void UploadDungeon(DungeonName dungeon_name,
                             DungeonType dungeon_type) = 0;
  virtual void RemoveDungeon(DungeonName dungeon_name) = 0;
};

}  // namespace core