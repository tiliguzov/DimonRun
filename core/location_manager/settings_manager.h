#pragma once

#include <string>

#include <QSettings>

namespace core {

class SettingsManager {
 public:
  SettingsManager();
  void SetKeyValue(std::string, std::string);
  QVariant GetByKey(std::string);
  void RemoveKey(std::string);
  void ClearKeys();
 private:
  QSettings settings_;
};

}  // namespace core