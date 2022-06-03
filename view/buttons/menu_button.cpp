#include <QPainter>
#include <QPaintEvent>

#include "menu_button.h"

MenuButton::MenuButton(
    const QString& path_to_pixmap,
    QWidget* parent,
    QRect relative_pos) :
    QPushButton(parent),
    relative_pos_(relative_pos),
    pixmap_(new QPixmap(path_to_pixmap)) {
}

void MenuButton::ChangePixmap(const QString& path_to_pixmap) {
  delete pixmap_;
  pixmap_ = new QPixmap(path_to_pixmap);
}

void MenuButton::paintEvent(QPaintEvent* event) {
  QPainter* painter = new QPainter(this);
  painter->drawPixmap(event->rect(), *pixmap_);
  delete painter;
}

QRect MenuButton::CalculateActualPos(QSize size) {
  return QRect(relative_pos_.x() * size.width() / 1000,
               relative_pos_.y() * size.height() / 1000,
               relative_pos_.width() * size.width() / 1000,
               relative_pos_.height() * size.height() / 1000);
}

QRect MenuButton::GetPos() const {
  return relative_pos_;
}
