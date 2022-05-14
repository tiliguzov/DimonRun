#pragma once

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QListWidget>
#include <QMouseEvent>
#include <QJsonObject>
#include <QPushButton>
#include <vector>

class MapCreator : public QWidget {
  Q_OBJECT

 public:
  explicit MapCreator(QWidget* parent);
  void LoadTextures();
  void CreateGrid();
  void AddTexture();
  void DeleteTexture();
  QPointF MousePositionOnSceneView();
  static QJsonObject GetJsonObject(const std::string& path_to_json);

  void mousePressEvent(QMouseEvent* event) override;

 private:
  QGraphicsScene* scene_;
  QGraphicsView* scene_view_;
  QListWidget* list_of_textures_[3]{};
  bool new_texture_on{false};
  QPixmap new_texture_;
  QPushButton* layer_button_;
  int layer_{0};
  std::vector <QGraphicsPixmapItem*> items_;
};