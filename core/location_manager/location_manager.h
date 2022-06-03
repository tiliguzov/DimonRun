#pragma once

#include "core/location_manager/settings_manager.h"
#include "core/dungeon_name.h"

#include <string>
#include <vector>

namespace core {

class LocationManager {
 public:
    LocationManager();

    int GetVaultCoins() const;
    void SetVaultCoins(int);

    void UnlockLocation(std::string);
    void UnlockLocation(DungeonName);

    std::vector<std::string> GetLocationsNames() const;

    void Reset();

    void GoToLocation(std::string);
 private:
  SettingsManager* settings_manager_;
  DungeonName current_dungeon_;
};

}  // namespace core
