#include "system.h"

System::System() : entities_{} {
  entities_.reserve(kMaxEntities);
}
