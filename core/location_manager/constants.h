#pragma once

namespace core {

const std::unordered_map<std::string, DungeonName> dungeon_name_by_note{
    {"level1", DungeonName::kLevel1},
    {"level2", DungeonName::kLevel2}
};

const std::unordered_map<DungeonName, std::string> note_by_dungeon_name{
    {DungeonName::kLevel1, "level1"},
    {DungeonName::kLevel2, "level2"}
};


const std::string kVaultCoins = "vault_coins";

}  // namespace core
