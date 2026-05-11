#include "mainwindow.h"
#include "gpacalculator.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GpaCalculator::instance().init();

    MainWindow w;
    w.show();

    return a.exec();
}
