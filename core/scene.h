#pragma once

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimerEvent>
#include <QWidget>

#include <memory>

#include "engine/coordinator.h"
#include "view/widgets/fast_menu.h"
#include "view/widgets/vault.h"

namespace core {

class Connector;

enum ZValues {
  kBackgroundZIndex,
  kPlayerZIndex
};

class Scene : public QStackedWidget, AbstractScene {
  Q_OBJECT

 public:
  Scene(QStackedWidget* parent, Connector* connector);

  QGraphicsScene* GetScene();
  QGraphicsView* GetSceneView();

 private:
  void paintEvent(QPaintEvent*) override;
  void timerEvent(QTimerEvent*) override;
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;

  void SetDefaultSceneSettings();

  bool eventFilter(QObject* object, QEvent* event) override;

  void OpenFastMenu() override;
  void ContinueGame() override;
  void OpenVault() override;

  DungeonName GetCurrentDungeon() override;
  void SetCurrentDungeon(DungeonName) override;

  void DownloadDungeon(DungeonName dungeon_name,
                       DungeonType dungeon_type) override;
  void UploadDungeon(DungeonName dungeon_name,
                     DungeonType dungeon_type) override;
  void RemoveDungeon(DungeonName dungeon_name) override;

  int32_t timer_id_;
  QWidget* parent_;
  Connector* connector_;
  QGraphicsItem* hero_item_;
  QGraphicsScene* scene_;
  QGraphicsView* scene_view_;

  FastMenu* fast_menu_;
  bool is_menu_showed_{0};

  Vault* vault_;
  bool is_vault_showed_{0};
};

}  // namespace core
