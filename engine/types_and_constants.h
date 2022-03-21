#pragma once

#include <bitset>
#include <cstdint>

namespace engine {

using Entity = uint16_t;
using ComponentID = uint8_t;
using id_type = uint16_t;  // type for all indexes in ComponentArray, Entity etc

const Entity kMaxEntities = 1000;
const ComponentID kMaxComponents = 10;

using ComponentSignature = std::bitset<kMaxComponents>;

}  // namespace engine
