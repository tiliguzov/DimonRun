#include "scroll.h"

#include "core/constants.h"
#include <QPainter>

namespace core {

Scroll::Scroll(AbstractScene* scene, const QString& path_to_background) :
    scene_(scene),
    background_(new QPixmap(path_to_background)),
    return_button_(new MenuButton(":/view/return.png", this, kReturnVault)),
    text_(new QLabel(this)) {

  text_->setGeometry(kTextScroll);
  text_->setFont(QFont("Copperplate", 22));
  text_->setStyleSheet("color: #241711;");

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
  text_->setGeometry(kTextScroll.x() * size.width() / 1000,
                     kTextScroll.y() * size.height() / 1000,
                     kTextScroll.width() * size.width() / 1000,
                     kTextScroll.height() * size.height() / 1000);
}

QLabel* Scroll::GetLabel() {
  return text_;
}

}  // namespace core
