#pragma once

#include <QPixmap>

#include <array>
#include <vector>
#include <string>
#include <unordered_map>
#include <vector>

namespace core {

//  the direction hero face to, left or right
enum class HorizontalDirection {
  kLeft,
  kRight
};

enum class MovementType {
  kStaticInAir,
  kFliesUp,
  kFliesDown,
  kFliesHorizontal,
  kTorchBurning,
  kWaterMoving,
  kWaveMoving
};

class AnimationPack {
 public:
  AnimationPack() = default;
  explicit AnimationPack(const std::string& path_to_json);
  QJsonObject GetJsonObject(const std::string& path_to_json);

  QPixmap& GetFrame(MovementType type, int current_time);
  int GetFrameDuration() const;

 private:
  std::unordered_map<core::MovementType, std::vector<QPixmap>>
      frames_;
  uint32_t frame_duration_{100};

  std::unordered_map<std::string, MovementType> str_to_type =
      {{"static_in_air", MovementType::kStaticInAir},
       {"flies_up", MovementType::kFliesUp},
       {"flies_down", MovementType::kFliesDown},
       {"flies_horizontal", MovementType::kFliesHorizontal},
       {"torch_burning", MovementType::kTorchBurning},
       {"water_moving", MovementType::kWaterMoving},
       {"wave_moving", MovementType::kWaveMoving}};
};

}  // namespace core
