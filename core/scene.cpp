#include <QKeyEvent>
#include <QPainter>
#include <QTimerEvent>

#include "scene.h"
#include "connector.h"
#include "constants.h"

namespace core {

Scene::Scene(QStackedWidget* parent, Connector* connector) :
    QStackedWidget(parent),
    parent_(parent),
    connector_(connector),
    timer_id_(startTimer(kTickTime)),
    scene_(new QGraphicsScene(this)),
    scene_view_(new QGraphicsView(this)),
    fast_menu_(new FastMenu(this,
                            ":/view/fast_menu.png",
                            connector_->GetLocationManager())),
    vault_(new Vault(this, ":/view/vault.png")),
    scroll_(new Scroll(this, ":/view/scroll.png")) {
  SetDefaultSceneSettings();

  addWidget(scene_view_);
  addWidget(fast_menu_);
  addWidget(vault_);
  addWidget(scroll_);

  show();
  setFocus();
  // example of interacting with engine
  connector->StartGame(this);
  ContinueGame();
  scene_view_->scale(1.7, 1.7);
}

void Scene::timerEvent(QTimerEvent* event) {
  if (event->timerId() != timer_id_) {
    return;
  }
  connector_->OnTick();
  scene_view_->centerOn(connector_->GetCoordinator()->
      GetComponent<GraphicsItemComponent>(hero_entity_).item);

  auto pos = connector_->GetCoordinator()->
      GetComponent<PositionComponent>(hero_entity_).position;

  assert(background_image_ != nullptr);
  background_image_->setPos(pos.x() - background_image_->pixmap().width() / 2,
                            pos.y() - background_image_->pixmap().height() / 2);
}

void Scene::paintEvent(QPaintEvent*) {
  QPainter painter(this);
}

void Scene::keyPressEvent(QKeyEvent* event) {
  connector_->OnKeyPress(static_cast<Qt::Key>(event->key()));
  if (event->key() == Qt::Key_Escape) {
    if (is_menu_showed_) {
      ContinueGame();
    } else {
      OpenFastMenu();
    }
    is_menu_showed_ = !is_menu_showed_;
  }
  if (event->key() == Qt::Key_V) {
    if (is_vault_showed_) {
      ContinueGame();
    } else {
      // OpenVault();
    }
    is_vault_showed_ = !is_vault_showed_;
  }
  if (event->key() == Qt::Key_O) {
    if (is_scroll_showed_) {
      ContinueGame();
    } else {
      OpenScroll("Some text");
    }
    is_scroll_showed_ = !is_scroll_showed_;
  }
}

void Scene::keyReleaseEvent(QKeyEvent* event) {
  connector_->OnKeyRelease(static_cast<Qt::Key>(event->key()));
}

void Scene::SetDefaultSceneSettings() {
  scene_view_->setScene(scene_);
  scene_view_->setGeometry(0, 0, kDefaultWindowWidth, kDefaultWindowHeight);
  scene_view_->setAutoFillBackground(true);
  scene_view_->setBackgroundBrush(Qt::darkGreen);
  scene_view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scene_view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scene_view_->viewport()->installEventFilter(this);  // wheel
  // scrolling disabled
  scene_view_->setSceneRect(
      kLeftUpXCoordinate, kLeftUpYCoordinate, kWidth, kHeight);
}

QGraphicsScene* Scene::GetScene() {
  return scene_;
}

QGraphicsView* Scene::GetSceneView() {
  return scene_view_;
}

bool Scene::eventFilter(QObject* object, QEvent* event) {
  if (object == scene_view_->viewport() && event->type() == QEvent::Wheel) {
    return true;
  }
  return false;
}

void Scene::OpenFastMenu() {
  fast_menu_->setGeometry(0, 0, kDefaultWindowWidth, kDefaultWindowHeight);
  setCurrentWidget(fast_menu_);
}

void Scene::OpenVault(std::string value) {
  vault_->setGeometry(0, 0, kDefaultWindowWidth, kDefaultWindowHeight);
  setCurrentWidget(fast_menu_);
  setCurrentWidget(vault_);
  vault_->GetLabel()->setText(value.c_str());
}

void Scene::OpenScroll(std::string message) {
  scroll_->setGeometry(0, 0, kDefaultWindowWidth, kDefaultWindowHeight);
  setCurrentWidget(fast_menu_);
  setCurrentWidget(scroll_);
  scroll_->GetLabel()->setText(message.c_str());
}

void Scene::ContinueGame() {
  setCurrentWidget(scene_view_);
}

void Scene::resizeEvent(QResizeEvent* event) {
  QSize new_size = size();
  resize(new_size);
  fast_menu_->Resize(new_size);
  vault_->Resize(new_size);
  scroll_->Resize(new_size);
}

void Scene::OpenNewDungeon(DungeonName dungeon_name) {
  connector_->OpenNewDungeon(dungeon_name);
}

void Scene::SetHeroEntity(engine::Entity entity) {
  hero_entity_ = entity;
}

void Scene::SetBackgroundImage(QGraphicsPixmapItem* item) {
  background_image_ = item;
}

}  // namespace core
