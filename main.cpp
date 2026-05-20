#include "mainwindow.h"
#include "gpacalculator.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!GpaCalculator::instance().init()) {
        qDebug() << "数据库初始化失败！";
        return 1;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
