#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Cow Model");
    w.iniBtn();
    //w.setStyleSheet("background-color: rgb(240,255,255)");
    w.show();

    return a.exec();
}
