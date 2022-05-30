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

// Number of bytes occupied by a char array containing the name for every
// source file in binary file of dungeon
const int kMaxPathLength = 100;

// Paths to dungeon files
const std::unordered_map<DungeonName, std::string> source_by_name_default{
    {DungeonName::kHub, "default_hub"}
};

const std::unordered_map<DungeonName, std::string> source_by_name_edited{
    {DungeonName::kHub, "edited_hub"}
};

const std::unordered_map<DungeonName, QString> source_by_name_hand_created{
    {DungeonName::kHub, ":hub.json"}
};

}  // namespace core
