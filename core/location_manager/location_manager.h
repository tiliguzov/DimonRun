#pragma once

#include <string>
#include <vector>

#include "core/location_manager/settings_manager.h"
#include "core/dungeon_name.h"


namespace core {

class LocationManager {
 public:
    LocationManager();

    int GetVaultCoins() const;
    void SetVaultCoins(int);
    void AddCoins(int);

    void UnlockLocation(std::string);
    void UnlockLocation(DungeonName);

    std::vector<std::string> GetLocationsNames() const;

    void Reset();

    void SetCurrentCoins(int);
    int GetCurrentCoins();

    void GoToLocation(std::string);
 private:
  SettingsManager* settings_manager_;
  DungeonName current_dungeon_;
  int current_coins_;
};

}  // namespace core
