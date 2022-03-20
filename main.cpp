#include <iostream>

#include <QApplication>
#include <QPushButton>

#include "core/connector.h"
#include "core/scene.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  Connector connector;
  Scene scene(nullptr, &connector);
  return QApplication::exec();
}
