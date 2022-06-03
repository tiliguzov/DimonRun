#include "core/location_manager/settings_manager.h"

namespace core {

SettingsManager::SettingsManager() :
    settings_{"Dream team", "DimonRun"} {}

void SettingsManager::SetKeyValue(std::string key, std::string value) {
  settings_.setValue(key.c_str(), value.c_str());
}

QVariant SettingsManager::GetByKey(std::string key) {
  return settings_.value(key.c_str());
}

void SettingsManager::RemoveKey(std::string key) {
  settings_.remove(key.c_str());
}

void SettingsManager::ClearKeys() {
  settings_.clear();
}

}  // namespace core