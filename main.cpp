#include "mainwindow.h"
#include <QApplication>
#include "sim.h"
#include "model.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    return 0;
    return a.exec();
}
