#include "vault.h"

#include <QPainter>

namespace core {

Vault::Vault(core::AbstractScene* scene,
                   const QString& path_to_background) :
    scene_(scene),
    background_(new QPixmap(path_to_background)),
    count_coins_(new QLabel("0", this)),
    return_button_(new MenuButton(":/view/return.png", this, kReturnVault)) {
  count_coins_->setGeometry(kCoinsVault);
  count_coins_->setFont(QFont("Copperplate", 20));
  count_coins_->setStyleSheet("color: #4A211E; font-weight: bold");

  connect(return_button_, &QPushButton::clicked,
          this, &Vault::ContinueGame);

  repaint();
}

void Vault::paintEvent(QPaintEvent* event) {
  QPainter* painter = new QPainter(this);
  painter->drawPixmap(event->rect(), *background_);
  delete painter;
}

void Vault::ContinueGame() {
  scene_->ContinueGame();

  repaint();
  resize(size());
}
void Vault::Resize(QSize size) {
  return_button_->setGeometry(return_button_->CalculateActualPos(size));
  count_coins_->setGeometry(kCoinsVault.x() * size.width() / 1000,
                            kCoinsVault.y() * size.height() / 1000,
                            kCoinsVault.width() * size.width() / 1000,
                            kCoinsVault.height() * size.height() / 1000);
}

}  // namespace core

