#pragma once

#include <QString>

#include <string>
#include <vector>

namespace core {

enum class EventType {
  kSecretRoom,
  kNewDungeonChest,
  kCoinChest,
  kVault,
  kGameExit,
  kProgressReset
};

struct SecretRoomData {
  std::string bin_source;
  std::string json_source;
};

const std::vector<SecretRoomData> secret_rooms_data{
    {"secret_room_1", ":secret_room_1.json"},
    {"secret_room_2", ":secret_room_2.json"},
    {"secret_room_3", ":secret_room_3.json"}
};

struct NewDungeonChestData {
  std::string scroll_massage;
  std::string menu_note;
  std::string bin_source;
  std::string json_source;
};

const std::vector<NewDungeonChestData> new_dungeon_chests_data{
    {"координаты второго уровня", "Dungeon_2", "level2", ":level2.json"},
    {"координаты третьего уровня", "Dungeon_3", "level3", ":level3.json"}
};

struct CoinChest {
  std::string scroll_massage;
  int coins;
};

struct std::vector<CoinChest> coin_chests_data{
    {"+1 coin", 1},
    {"+2 coin", 2}
};

}  // namespace core
