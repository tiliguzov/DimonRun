#pragma once

#include "view/buttons/menu_button.h"
#include "core/abstract_scene.h"

#include <QStackedWidget>
#include <QLabel>
//
namespace core {

class Message : public QWidget {
 public:
  Message(AbstractScene* parent,
          const QString& path_to_background);

 private:
  void paintEvent(QPaintEvent* event) override;

  AbstractScene* scene_;
  QPixmap* background_;
};

} // namespace core
