#include "mainwindow.h"
#include <QApplication>
#include "myserver.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  MyServer Server;
  Server.StartServer();

//  MainWindow w;
//  w.show();

  return a.exec();
}
