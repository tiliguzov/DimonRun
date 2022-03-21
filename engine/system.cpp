#include "system.h"

namespace engine {

System::System() : entities_{} {
  entities_.reserve(kMaxEntities);
}

}  // namespace engine
