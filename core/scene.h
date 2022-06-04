#pragma once

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimerEvent>
#include <QWidget>

#include <memory>
#include <view/widgets/scroll.h>

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
  void SetHeroEntity(engine::Entity entity);
  void SetBackgroundImage(QGraphicsPixmapItem* item);

  void OpenFastMenu() override;
  void ContinueGame() override;
  void OpenVault(std::string) override;
  void OpenScroll(std::string) override;

 private:
  void paintEvent(QPaintEvent*) override;
  void timerEvent(QTimerEvent*) override;
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;

  void SetDefaultSceneSettings();

  bool eventFilter(QObject* object, QEvent* event) override;

  void OpenNewDungeon(DungeonName) override;

  int32_t timer_id_;
  QWidget* parent_;
  Connector* connector_;
  engine::Entity hero_entity_;
  QGraphicsScene* scene_;
  QGraphicsView* scene_view_;
  QGraphicsPixmapItem* background_image_;

  FastMenu* fast_menu_;
  bool is_menu_showed_{0};

  Vault* vault_;
  bool is_vault_showed_{0};

  Scroll* scroll_;
  bool is_scroll_showed_{0};
};

}  // namespace core
