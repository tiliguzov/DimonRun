#pragma once

#include <QString>

#include <string>
#include <vector>

#include "dungeon_name.h"

namespace core {

enum class EventType {
  kSecretRoom,
  kNextDungeon,
  kCoinChest,
  kVault,
  kGameExit,
  kProgressReset,
  kCreator
};

struct SecretRoomData {
  DungeonName name;
};

const std::vector<SecretRoomData> secret_rooms_data{
    {DungeonName::kSecretRoom1},
    {DungeonName::kSecretRoom2}
};

struct NewDungeonChestData {
  std::string scroll_massage;
  DungeonName name;
};

const std::vector<NewDungeonChestData> new_dungeon_chests_data{
    {"координаты второго уровня", DungeonName::kLevel2},
    {"координаты третьего уровня", DungeonName::kLevel3}
};

// struct CoinChest {
//   std::string scroll_massage;
//   int coins;
// };

// struct std::vector<CoinChest> coin_chests_data{
//     {"+1 coin", 1},
//     {"+2 coin", 2}
// };

}  // namespace core
