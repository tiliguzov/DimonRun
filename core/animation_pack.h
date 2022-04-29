#pragma once

#include <QPixmap>

#include <array>
#include <vector>
#include <string>
#include <unordered_map>

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

  QPixmap* GetFrame(MovementType type, int32_t current_time) const;
  int32_t GetFrameDuration() const;

 private:
  std::array<std::vector<QPixmap*>,
      static_cast<size_t>(MovementType::kEnumSize)>
      frames_;
  uint32_t frame_duration_;

  const std::unordered_map<std::string, MovementType> str_to_type =
      {{"static_in_air", MovementType::kStaticInAir},
       {"flies_up", MovementType::kFliesUp},
       {"flies_down", MovementType::kFliesDown},
       {"flies_horizontal", MovementType::kFliesHorizontal}};
};

}  // namespace core
