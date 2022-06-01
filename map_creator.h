#pragma once

#include "mc_connector.h"
#include "core/constants.h"

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QListWidget>
#include <QMouseEvent>
#include <QJsonObject>
#include <QPushButton>
#include <QTimerEvent>
#include <vector>
#include <QKeyEvent>
#include <QJsonDocument>

class MapCreator : public QWidget {
  Q_OBJECT

 public:
  MapCreator(QWidget* parent, MCConnector* connector);
  void CreateDefaultScene();
  void LoadTextures();
  void CreateGrid();
  void AddTexture(QPointF point, int layer, const std::string &source);
  void DeleteTexture(QPointF point, int layer);
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

 private:
  MCConnector* connector_;
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
  int layer_{0};
  std::vector <engine::Entity> items_;
  std::unordered_map<engine::Entity, std::pair <double, double> > scale_of_items_;
  std::unordered_map<engine::Entity, int> rotate_of_items_;
  std::unordered_map<QListWidgetItem*, std::string> source_;
};