#pragma once

namespace core {

enum class DungeonName {
  kHub,
  kLevel1,
  kLevel2,
  kLevel3,
  kLevel4,
  kSecretRoom1,
  kSecretRoom2
};

// Every dungeon can have three files: default binary, edited binary and json
enum class DungeonType {
  kDefault,
  kEdited,
  kHandCreated
};

}  // namespace core
