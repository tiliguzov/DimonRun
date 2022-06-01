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

class MapCreator : public QWidget {
  Q_OBJECT

 public:
  MapCreator(QWidget* parent, MCConnector* connector);
  void LoadTextures();
  void CreateGrid();
  void AddTexture();
  void DeleteTexture();
  void ChangeLayer();
  void ChooseItem();
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
  bool new_texture_on{false};
  QPixmap new_texture_;
  QPushButton* layer_button_;
  int layer_{0};
  std::vector <engine::Entity> items_;
};