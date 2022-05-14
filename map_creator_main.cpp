#include <QApplication>

#include "map_creator.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  MapCreator scene(nullptr);
  return QApplication::exec();
}
