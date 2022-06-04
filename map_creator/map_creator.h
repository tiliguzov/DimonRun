#pragma once

#include "core/connector.h"
#include "core/constants.h"
#include "core/animation_pack.h"

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QListWidget>
#include <QMouseEvent>
#include <QJsonObject>
#include <QPushButton>
#include <QTimerEvent>
#include <QResizeEvent>
#include <vector>
#include <QKeyEvent>
#include <QJsonDocument>
#include <utility>
#include <string>
#include <unordered_map>

class MapCreator : public QWidget {
  Q_OBJECT

 public:
  MapCreator(QWidget* parent, core::Connector* connector);
  void CreateDefaultScene();
  void LoadTextures();
  void CreateGrid();
  void AddTexture(QPointF point, int layer, const std::string &source);
  void DeleteTexture(QPointF point, int layer);
  void DeleteAllTextures();
  void ChangeLayer();
  void ChooseItem();
  void SetDefaultScaleAndRotate();
  void ReadFromJson(const QJsonObject& file);
  QJsonDocument AllEntities();
  QPointF MousePositionOnSceneView();
  static QJsonObject GetJsonObject(const std::string& path_to_json);

  void mousePressEvent(QMouseEvent* event) override;
  void timerEvent(QTimerEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;

 private:
  core::Connector* connector_;
  int32_t timer_id_;
  QGraphicsScene* scene_;
  QGraphicsView* scene_view_;
  QListWidget* list_of_textures_[3]{};
  QListWidgetItem* last_item_;
  bool new_texture_on{false};
  QPixmap new_texture_;
  std::pair<double, double> new_texture_scale_{1, 1};
  int new_texture_rotate_{0};
  QPushButton* layer_button_;
  QPushButton* save_button_;
  QPushButton* download_button_;
  QPointF pos_of_hero_{0, 0};
  int layer_{0};
  std::vector <engine::Entity> items_;
  std::unordered_map<QListWidgetItem*, std::string> source_;
  std::unordered_map<std::string, std::string> animation_source_;
  std::unordered_map<std::string, core::HorizontalDirection> direction_;
  std::unordered_map<std::string, core::MovementType> movement_type_;
  std::unordered_map<std::string, int> start_time_;

  std::unordered_map<std::string, bool> is_movable_;
  std::unordered_map<std::string, bool> gravity_;
  std::unordered_map<std::string, bool> can_use_;
  std::unordered_map<std::string, bool> is_usable_;
  std::unordered_map<std::string, bool> is_breakable_;

  std::unordered_map<std::string, int> kill_time_;
  std::unordered_map<std::string, bool> is_ill_;

  std::unordered_map<std::string, double> current_speed_;
  std::unordered_map<std::string, double> direction_x_;
  std::unordered_map<std::string, double> direction_y_;

  std::unordered_map<std::string, int> value_;

  std::unordered_map<std::string, core::MovementType> str_to_type =
      {{"static_in_air", core::MovementType::kStaticInAir},
       {"flies_up", core::MovementType::kFliesUp},
       {"flies_down", core::MovementType::kFliesDown},
       {"flies_horizontal", core::MovementType::kFliesHorizontal},
       {"torch_burning", core::MovementType::kTorchBurning},
       {"water_moving", core::MovementType::kWaterMoving},
       {"wave_moving", core::MovementType::kWaveMoving},
       {"coin_moving", core::MovementType::kCoinMoving},
       {"stone_moving", core::MovementType::kStoneMoving},
       {"rubbish_destroy", core::MovementType::kRubbishDestroy}};
};
