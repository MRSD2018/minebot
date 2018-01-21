#include "mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDesktopWidget dw;
    MainWindow w;

    int x=dw.width()*0.4;
    int y=dw.height()*0.7;
    w.setFixedSize(x,y);
    w.setStyleSheet("QMainWindow {background: 'white';}");
    w.show();

    return a.exec();
}
