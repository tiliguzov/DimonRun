#include "core/location_manager/location_manager.h"
#include "core/location_manager/settings_manager.h"
#include "core/location_manager/constants.h"

#include <iostream>
#include <string>

namespace core {

LocationManager::LocationManager() :
    settings_manager_(new SettingsManager()) {
  current_dungeon_ =
      static_cast<DungeonName>(
          settings_manager_->GetByKey(kCurrentDungeon).toInt());
}

int LocationManager::GetVaultCoins() const {
  return settings_manager_->GetByKey(kVaultCoins).toInt();
}

void LocationManager::SetVaultCoins(int count) {
  settings_manager_->SetKeyValue(kVaultCoins, std::to_string(count));
}


void LocationManager::UnlockLocation(std::string location_name) {
  UnlockLocation(dungeon_name_by_note.at(location_name));
  current_dungeon_ = dungeon_name_by_note.at(location_name);
  settings_manager_->SetKeyValue(
      kCurrentDungeon,
      std::to_string(static_cast<int>(current_dungeon_)));
}

void LocationManager::UnlockLocation(DungeonName dungeon_name) {
  settings_manager_->SetKeyValue(
      note_by_dungeon_name.at(dungeon_name),
      settings_manager_->GetByKey(kVaultCoins).toString().toStdString());
  current_dungeon_ = dungeon_name;
  settings_manager_->SetKeyValue(
      kCurrentDungeon,
      std::to_string(static_cast<int>(current_dungeon_)));
}

void LocationManager::GoToLocation(std::string location_name) {
  int first_dungeon = static_cast<int>(dungeon_name_by_note.at(location_name));
  int last_dungeon = static_cast<int>(current_dungeon_);
  for (int i = first_dungeon + 1; i <= last_dungeon; i++) {
    settings_manager_->RemoveKey(
        note_by_dungeon_name.at(static_cast<DungeonName>(i)));
  }
  current_dungeon_ = dungeon_name_by_note.at(location_name);
  settings_manager_->SetKeyValue(
      kCurrentDungeon,
      std::to_string(static_cast<int>(current_dungeon_)));
}

std::vector<std::string> LocationManager::GetLocationsNames() const {
  int dungeon_index = static_cast<int>(current_dungeon_);
  std::vector<std::string> names_list;
  std::cout << dungeon_index << " sadasd" << std::endl;
  for (int i = 0; i <= dungeon_index; i++) {
    names_list.push_back(note_by_dungeon_name.at(static_cast<DungeonName>(i + 1)));
  }
  return names_list;
}

void LocationManager::Reset() {
  std::cout << "aa" << std::endl;
  GoToLocation(note_by_dungeon_name.at(static_cast<DungeonName>(1)));
  current_dungeon_ = DungeonName::kLevel1;
  settings_manager_->SetKeyValue(
      kCurrentDungeon,
      std::to_string(static_cast<int>(current_dungeon_)));
  std::cout << "bb" << std::endl;
}

void LocationManager::AddCoins(int coins) {
  SetVaultCoins(GetVaultCoins() + coins);
}

}  // namespace core