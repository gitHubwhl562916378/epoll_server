#include <iostream>
#include <QApplication>
#include "MainWindow.h"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    a.setApplicationName(QObject::tr("EPollServer Demo"));

    MainWindow w;
    w.show();

    return a.exec();
};