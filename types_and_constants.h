#pragma once

#include <bitset>
#include <cstdint>

const uint16_t kMaxEntities = 1000;
const uint8_t kMaxComponents = 10;

using id_type = uint16_t;  // type for all indexes in ComponentArray, Entity etc
using Entity = uint16_t;
using Component = uint8_t;
using ComponentSignature = std::bitset<kMaxComponents>;
