#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    char f;
    f=wiringPiSPISetup(0,61000);

    w.show();
    
    return a.exec();
}
