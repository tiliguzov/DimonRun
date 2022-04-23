#pragma once

#include <QPixmap>

#include <array>
#include <vector>
#include <memory>
#include <string>

namespace core {

enum class MovementType {
  kStaticInAir,
  kFliesUp,
  kFliesDown,
  kFliesHorizontal,

  kEnumSize
};

class AnimationPack {
 public:
  explicit AnimationPack(const std::string& path_to_json);

 private:
  std::array<std::vector<QPixmap*>, (size_t)(MovementType::kEnumSize)>
      animations_;
  uint32_t frame_duration_;

  const std::unordered_map<std::string, MovementType> str_to_type =
      {{"static_in_air", MovementType::kStaticInAir},
       {"flies_up", MovementType::kFliesUp},
       {"flies_down", MovementType::kFliesDown},
       {"flies_horizontal", MovementType::kFliesHorizontal}};
};

}  // namespace core