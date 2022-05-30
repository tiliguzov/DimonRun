#include <QApplication>

#include "core/connector.h"
#include "core/scene.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  core::Connector connector;
  core::Scene scene(nullptr, &connector);
  return QApplication::exec();
}
