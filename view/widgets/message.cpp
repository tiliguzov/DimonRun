#include <QPainter>

#include "message.h"

namespace core {

core::Message::Message(AbstractScene* scene,
                       const QString& path_to_background) :
    scene_(scene),
    background_(new QPixmap(path_to_background)) {
  repaint();
}

void Message::paintEvent(QPaintEvent* event) {
  QPainter* painter = new QPainter(this);
  painter->drawPixmap(event->rect(), *background_);
  delete painter;
}

}  // namespace core