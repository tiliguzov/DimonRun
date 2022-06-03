#include <QApplication>

#include "core/connector.h"
#include "map_creator/map_creator.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  core::Connector connector;
  MapCreator scene(nullptr, &connector);
  return QApplication::exec();
}
