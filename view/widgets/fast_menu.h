#pragma once

#include "view/buttons/menu_button.h"
#include "core/abstract_scene.h"

#include <vector>

#include <QWidget>
#include <QStackedWidget>
#include <QPaintEvent>
#include <QLabel>
#include <QVBoxLayout>
#include <QListWidget>

namespace core {

class FastMenu : public QStackedWidget {
 public:
  FastMenu(core::AbstractScene* parent, const QString& path_to_background);

  void Resize(QSize size);

 private:
  void paintEvent(QPaintEvent* event) override;

  void OpenListPlaces();
  void OpenShortcutsList();
  void ReturnMenu();

  void MuteMusic();

  void ContinueGame();

  void OpenMenuWidgets();
  void DeleteMenuWidgets();
  void OpenShortcutsWidgets();
  void DeleteShortcutsWidgets();
  void OpenPlacesWidgets();
  void DeletePlacesWidgets();

  void PlacesOpen();

  AbstractScene* scene_;

  MenuButton* coins_;
  QLabel* count_coins_;

  MenuButton* resume_button_;
  MenuButton* places_list_;
  MenuButton* shortcuts_list_;
  MenuButton* music_on_;
  QLabel* music_;
  MenuButton* music_off_;
  QPixmap* background_;

  QLabel* shortcuts_;
  MenuButton* return_button_;

  QListWidget* places_;
  std::vector<QString> places_text_;
};

}  // namespace core
