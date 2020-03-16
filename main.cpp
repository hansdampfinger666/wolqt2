#include "mainwindow.h"
#include <file_loader.h>
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(nullptr, argv);
    w.show();
    return a.exec();
}
