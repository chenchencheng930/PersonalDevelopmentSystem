#include <QApplication>
#include "coursepage.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CoursePage w; // 直接启动课程页，不打开旧主窗口
    w.show();
    return a.exec();
}
