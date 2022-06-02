#include <QApplication>

#include "mc_connector.h"
#include "map_creator/map_creator.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  MCConnector connector;
  MapCreator scene(nullptr, &connector);
  return QApplication::exec();
}
