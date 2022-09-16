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
    {"Координаты 56.45543|23.23213\n"
     "Bремя 208г. 11.20 20:11.342\n"
     "Ты знаешь, что делать", DungeonName::kLevel2},
    {"Координаты 24.33464|45.08603\n"
     "Время 704г. 6.11 21:44.100\n"
     "Привет из будущего", DungeonName::kLevel3}
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
