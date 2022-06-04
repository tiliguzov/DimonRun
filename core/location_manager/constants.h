#pragma once

namespace core {

const std::unordered_map<std::string, DungeonName> dungeon_name_by_note{
    {"level1", DungeonName::kLevel1},
    {"level2", DungeonName::kLevel2},
    {"level3", DungeonName::kLevel3},
    {"level4", DungeonName::kLevel4}
};

const std::unordered_map<DungeonName, std::string> note_by_dungeon_name{
    {DungeonName::kLevel1, "level1"},
    {DungeonName::kLevel2, "level2"},
    {DungeonName::kLevel3, "level3"},
    {DungeonName::kLevel4, "level4"""}
};


const std::string kVaultCoins = "vault_coins";
const std::string kCurrentDungeon = "current_dungeon";

}  // namespace core
