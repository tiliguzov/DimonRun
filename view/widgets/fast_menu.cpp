#include "fast_menu.h"

#include "core/location_manager/constants.h"
#include "core/location_manager/location_manager.h"
#include <QPainter>

namespace core {

FastMenu::FastMenu(AbstractScene* scene,
                   const QString& path_to_background,
                   LocationManager* location_manager) :
    scene_(scene),
    background_(new QPixmap(path_to_background)),
    resume_button_(new MenuButton(":/view/close.png", this, kResumeButton)),
    music_on_(new MenuButton(":/view/on.png", this, kMusicButton)),
    music_off_(new MenuButton(":/view/off.png", this, kMusicButton)),
    coins_(new MenuButton(":/view/coin.png", this, kCoins)),
    count_coins_(new QLabel("0/0", this)),
    places_list_(new MenuButton(":/view/places.png", this, kPlacesList)),
    shortcuts_list_(new MenuButton(":/view/shortcuts.png",
                                   this,
                                   kShortcutsList)),
    music_(new QLabel("MUSIC", this)),
    shortcuts_(new QLabel(kShortcutsText, this)),
    return_button_(new MenuButton(":/view/return.png", this, kReturn)),
    places_(new QListWidget(this)),
    location_manager_(location_manager) {
  music_->setFont(QFont("Copperplate", 22));
  music_->setStyleSheet("color: rgb(250, 250, 250); background: #241711;");
  music_->setGeometry(kMusicText);

  coins_->setGeometry(kCoins);
  count_coins_->setGeometry(kCoinsCounter);
  count_coins_->setFont(QFont("Copperplate", 14));
  count_coins_->setStyleSheet("color: rgb(250, 250, 250);"
                              "background: #241711;");

  shortcuts_->setFont(QFont("Copperplate", 14));
  shortcuts_->setStyleSheet("color: rgb(250, 250, 250);"
                            "background: #241711;");
  shortcuts_->setGeometry(kShortcuts);

  places_->setGeometry(kShortcuts);
  places_->setFont(QFont("Copperplate", 14));
  places_->setStyleSheet(
      "background: #241711; color: rgb(250, 250, 250); border: #241711;");

  OpenMenuWidgets();
  DeleteShortcutsWidgets();
  DeletePlacesWidgets();

  connect(places_list_, &QPushButton::clicked,
          this, &FastMenu::OpenListPlaces);
  connect(shortcuts_list_, &QPushButton::clicked,
          this, &FastMenu::OpenShortcutsList);
  connect(resume_button_, &QPushButton::clicked,
          this, &FastMenu::ContinueGame);
  connect(return_button_, &QPushButton::clicked,
          this, &FastMenu::ReturnMenu);
  connect(music_on_, &QPushButton::clicked,
          this, &FastMenu::MuteMusic);
  connect(music_off_, &QPushButton::clicked,
          this, &FastMenu::MuteMusic);
  repaint();
}

void FastMenu::paintEvent(QPaintEvent* event) {
  QPainter* painter = new QPainter(this);
  painter->drawPixmap(event->rect(), *background_);
  delete painter;
}

void FastMenu::Resize(QSize size) {
  resume_button_->setGeometry(resume_button_->CalculateActualPos(size));
  music_on_->setGeometry(music_on_->CalculateActualPos(size));
  music_off_->setGeometry(music_off_->CalculateActualPos(size));
  coins_->setGeometry(coins_->CalculateActualPos(size));
  places_list_->setGeometry(places_list_->CalculateActualPos(size));
  shortcuts_list_->setGeometry(shortcuts_list_->CalculateActualPos(size));
  music_->setGeometry(kMusicText.x() * size.width() / 1000,
                      kMusicText.y() * size.height() / 1000,
                      kMusicText.width() * size.width() / 1000,
                      kMusicText.height() * size.height() / 1000);
  shortcuts_->setGeometry(kShortcuts.x() * size.width() / 1000,
                          kShortcuts.y() * size.height() / 1000,
                          kShortcuts.width() * size.width() / 1000,
                          kShortcuts.height() * size.height() / 1000);
  return_button_->setGeometry(return_button_->CalculateActualPos(size));
  places_->setGeometry(kPlaces.x() * size.width() / 1000,
                       kPlaces.y() * size.height() / 1000,
                       kPlaces.width() * size.width() / 1000,
                       kPlaces.height() * size.height() / 1000);
  count_coins_->setGeometry(kCoinsCounter.x() * size.width() / 1000,
                            kCoinsCounter.y() * size.height() / 1000,
                            kCoinsCounter.width() * size.width() / 1000,
                            kCoinsCounter.height() * size.height() / 1000);
}

void FastMenu::OpenListPlaces() {
  DeleteMenuWidgets();
  DeleteShortcutsWidgets();
  OpenPlacesWidgets();
  places_text_.clear();
  places_->clear();
  places_text_.emplace_back("hub");
  places_text_.emplace_back("level1");
  places_text_.emplace_back("level2");
  places_text_.emplace_back("green screen");
  for (int i = 0; i < places_text_.size(); ++i) {
    auto* item = new QListWidgetItem(places_text_[i], places_, i + 1);
  }
  connect(places_, &QListWidget::itemClicked,
          this, &FastMenu::PlacesOpen);

  repaint();
  resize(size());
}

void FastMenu::OpenShortcutsList() {
  DeleteMenuWidgets();
  DeletePlacesWidgets();
  OpenShortcutsWidgets();
  repaint();
  resize(size());
}

void FastMenu::ReturnMenu() {
  DeletePlacesWidgets();
  DeleteShortcutsWidgets();
  OpenMenuWidgets();
  repaint();
  resize(size());
}

void FastMenu::OpenMenuWidgets() {
  music_on_->setVisible(true);
  music_off_->setVisible(false);
  coins_->setVisible(true);
  places_list_->setVisible(true);
  shortcuts_list_->setVisible(true);
  music_->setVisible(true);
}

void FastMenu::DeleteMenuWidgets() {
  music_on_->setVisible(false);
  music_off_->setVisible(false);
  coins_->setVisible(false);
  places_list_->setVisible(false);
  shortcuts_list_->setVisible(false);
  music_->setVisible(false);
}

void FastMenu::OpenPlacesWidgets() {
  places_->setVisible(true);
  return_button_->setVisible(true);
}

void FastMenu::DeletePlacesWidgets() {
  places_->setVisible(false);
  return_button_->setVisible(false);
}

void FastMenu::OpenShortcutsWidgets() {
  shortcuts_->setVisible(true);
  return_button_->setVisible(true);
}

void FastMenu::DeleteShortcutsWidgets() {
  shortcuts_->setVisible(false);
  return_button_->setVisible(false);
}

void FastMenu::ContinueGame() {
  scene_->ContinueGame();

  DeleteShortcutsWidgets();
  DeletePlacesWidgets();
  OpenMenuWidgets();

  repaint();
  resize(size());
}

void FastMenu::MuteMusic() {
  if (music_on_->isVisible()) {
    music_off_->setVisible(true);
    music_on_->setVisible(false);
  } else {
    music_off_->setVisible(false);
    music_on_->setVisible(true);
  }
}

void FastMenu::PlacesOpen() {
  QListWidgetItem* item = places_->currentItem();
  if (item->text() != "hub") {
    location_manager_->GoToLocation(item->text().toStdString());
    scene_->OpenNewDungeon(dungeon_name_by_note.at(item->text().toStdString()));
  } else {
    scene_->OpenNewDungeon(DungeonName::kHub);
  }
  ContinueGame();
}

}  // namespace core
