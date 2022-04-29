#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <iostream>

#include "animation_pack.h"

core::AnimationPack::AnimationPack(const std::string& path_to_json) {
  QFile file(QString::fromStdString(path_to_json));
  file.open(QIODevice::ReadOnly);
  QJsonObject input_object = QJsonDocument::fromJson(file.readAll()).object();
  file.close();

  auto animation_types = input_object["animations"].toObject();

  for (const auto &key : animation_types.keys()) {
    size_t type = static_cast<size_t>(str_to_type.at(key.toStdString()));

    for (const auto &frame_path : animation_types[key].toArray()) {
      frames_[type].push_back(new QPixmap(frame_path.toString()));
    }
  }

  frame_duration_ = input_object["frame_duration"].toInt();
}

QPixmap* core::AnimationPack::GetFrame(core::MovementType type,
                                       int current_time) const {
  int count_of_frames = current_time /
      frame_duration_;
  count_of_frames %= frames_[static_cast<size_t>(type)].size();
  return frames_[static_cast<size_t>(type)][count_of_frames];
}

int core::AnimationPack::GetFrameDuration() const {
  return frame_duration_;
}

