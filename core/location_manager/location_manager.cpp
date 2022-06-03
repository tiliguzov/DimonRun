#include "core/location_manager/location_manager.h"
#include "core/location_manager/settings_manager.h"
#include "core/location_manager/constants.h"

#include <string>

namespace core {

LocationManager::LocationManager() :
    settings_manager_(new SettingsManager()) {}

int LocationManager::GetVaultCoins() const {
  return settings_manager_->GetByKey(kVaultCoins).toInt();
}

void LocationManager::SetVaultCoins(int count) {
  settings_manager_->SetKeyValue(kVaultCoins, std::to_string(count));
}

void LocationManager::UnlockLocation(std::string location_name) {
  UnlockLocation(dungeon_name_by_note.at(location_name));
  current_dungeon_ = dungeon_name_by_note.at(location_name);
}

void LocationManager::UnlockLocation(DungeonName dungeon_name) {
  settings_manager_->SetKeyValue(
      note_by_dungeon_name.at(dungeon_name),
      settings_manager_->GetByKey(kVaultCoins).toString().toStdString());
  current_dungeon_ = dungeon_name;
}

void LocationManager::GoToLocation(std::string location_name) {
  int first_dungeon = static_cast<int>(dungeon_name_by_note.at(location_name));
  int last_dungeon = static_cast<int>(current_dungeon_);
  for (int i = first_dungeon + 1; i <= last_dungeon; i++) {
    settings_manager_->RemoveKey(
        note_by_dungeon_name.at(static_cast<DungeonName>(i)));
  }
  current_dungeon_ = dungeon_name_by_note.at(location_name);
}

std::vector<std::string> LocationManager::GetLocationsNames() const {
  int dungeon_index = static_cast<int>(current_dungeon_);
  std::vector<std::string> names_list;
  for (int i = 0; i <= dungeon_index; i++) {
    names_list.push_back(note_by_dungeon_name.at(static_cast<DungeonName>(i)));
  }
  return names_list;
}

void LocationManager::Reset() {
  GoToLocation(note_by_dungeon_name.at(static_cast<DungeonName>(0)));
}

}  // namespace core