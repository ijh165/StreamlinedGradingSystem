#include "streamlinedgradingsystem.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  StreamlinedGradingSystem w;
  w.show();

  return a.exec();
}
