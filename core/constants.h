#pragma  once

#include "QString"

#include <string>
#include <unordered_map>

#include "dungeon_name.h"

namespace core {

const int kTickTime = 5;

const int kTextureSize = 50;

const int kDefaultWindowHeight = 600;
const int kDefaultWindowWidth = 1000;

const int kLeftUpXCoordinate = -10000;
const int kLeftUpYCoordinate = -10000;
const int kWidth = 20000;
const int kHeight = 20000;

constexpr QRect kResumeButton(450, 220, 35, 35);
constexpr QRect kCoins(270, 220, 25, 25);
constexpr QRect kCoinsCounter(300, 220, 25, 25);
constexpr QRect kPlacesList(310, 290, 140, 50);
constexpr QRect kShortcutsList(310, 370, 140, 50);
constexpr QRect kMusicButton(390, 480, 90, 50);
constexpr QRect kMusicText(310, 480, 80, 50);
constexpr QRect kShortcuts(280, 260, 200, 200);
constexpr QRect kPlaces(280, 260, 200, 200);
constexpr QRect kReturn(310, 480, 140, 50);
constexpr QRect kReturnVault(390, 620, 200, 70);
constexpr QRect kCoinsVault(495, 160, 100, 60);

constexpr QRect kTextScroll(330, 160, 200, 200);
constexpr QRect kMessage(495, 160, 100, 60);

const QString kShortcutsText{
    "ESC -> Open fast menu \nCTRL + M - Mute/unmute \nCTRL + Q - "
    "Exit game \nCTRL + S - сохраниться"};

// Number of bytes occupied by a char array containing the name for every
// source file in binary file of dungeon
const int kMaxPathLength = 100;

// Paths to dungeon files
const std::unordered_map<DungeonName, std::string> source_by_name_default{
    {DungeonName::kHub, "default_hub"},
    {DungeonName::kLevel1, "default_level1"},
    {DungeonName::kLevel2, "default_level2"}
};

const std::unordered_map<DungeonName, std::string> source_by_name_edited{
    {DungeonName::kHub, "edited_hub"},
    {DungeonName::kLevel1, "edited_level1"},
    {DungeonName::kLevel2, "edited_level2"}
};

const std::unordered_map<DungeonName, QString> source_by_name_hand_created{
    {DungeonName::kHub, ":hub.json"},
    {DungeonName::kLevel1, ":level1.json"},
    {DungeonName::kLevel2, ":level2.json"}
};

const std::unordered_map<std::string, DungeonName> dungeon_name_by_note{
    {"hub", DungeonName::kHub},
    {"level1", DungeonName::kLevel1},
    {"level2", DungeonName::kLevel2},
    {"green screen", DungeonName::kNone}
};

}  // namespace core
