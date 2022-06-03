//
// Created by admin on 03.06.2022.
//

#ifndef VAULT_H
#define VAULT_H

#include "view/buttons/menu_button.h"
#include "core/abstract_scene.h"

#include <QStackedWidget>
#include <QLabel>

namespace core {

class Vault : public QStackedWidget {
 public:
  Vault(core::AbstractScene* parent, const QString& path_to_background);
  void Resize(QSize size);

 private:
  void paintEvent(QPaintEvent* event) override;
  void ContinueGame();

  AbstractScene* scene_;
  QPixmap* background_;

  QLabel* count_coins_;
  MenuButton* return_button_;
};

}

#endif //VAULT_H