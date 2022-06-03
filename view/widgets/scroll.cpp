#include "scroll.h"

#include <QPainter>

namespace core {

Scroll::Scroll(AbstractScene* scene, const QString& path_to_background) :
    scene_(scene),
    background_(new QPixmap(path_to_background)),
    return_button_(new MenuButton(":/view/return.png", this, kReturnVault)) {

  connect(return_button_, &QPushButton::clicked,
          this, &Scroll::ContinueGame);

  repaint();
}

void Scroll::paintEvent(QPaintEvent* event) {
  QPainter* painter = new QPainter(this);
  painter->drawPixmap(event->rect(), *background_);
  delete painter;
}

void Scroll::ContinueGame() {
  scene_->ContinueGame();

  repaint();
  resize(size());
}

void Scroll::Resize(QSize size) {
  return_button_->setGeometry(return_button_->CalculateActualPos(size));
}

}  // namespace core
