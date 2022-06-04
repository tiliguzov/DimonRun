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
    {"Координаты 56.45|23.23\nBремя: 208 год", DungeonName::kLevel2},
    {"Координаты 24.34|45.03\nВремя: 704 год", DungeonName::kLevel3}
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
