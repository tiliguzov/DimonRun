#pragma once

#include <QPushButton>

class MenuButton : public QPushButton {
  Q_OBJECT
 public:
  MenuButton(
      const QString& path_to_pixmap,
      QWidget* parent,
      QRect relative_pos);
  QRect CalculateActualPos(QSize size);
  void ChangePixmap(const QString& path_to_pixmap);
  QRect GetPos() const;

 private:
  void paintEvent(QPaintEvent* event) override;

  QRect relative_pos_;
  QPixmap* pixmap_;
};
