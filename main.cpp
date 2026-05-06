#include <QCoreApplication>
#include <QDebug>
#include <iostream>        // 用于 std::cin.get()
#include "GpaCalculator.h"
#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "========== 程序启动 ==========";

    // 初始化数据库
    if (!GpaCalculator::instance().init()) {
        qDebug() << " 数据库初始化失败！";
        std::cout << "按回车键退出...";
        std::cin.get();
        return 1;
    }
    qDebug() << "数据库初始化成功";

    // 测试添加课程
    GpaCalculator::instance().addCourse("数学", 3.0, 3.7);
    GpaCalculator::instance().addCourse("物理", 4.0, 3.3);
    qDebug() << " 课程数量:" << GpaCalculator::instance().getCourseCount();

    auto courses = GpaCalculator::instance().getAllCourses();
    for (const auto& c : courses) {
        qDebug() << "课程:" << c.name << "| 学分:" << c.credit << "| 绩点:" << c.gradePoint;
    }

    // 测试经历
    GpaCalculator::instance().addExperience("黑客马拉松", "获得一等奖", "2025-03-15");
    auto exps = GpaCalculator::instance().getAllExperiences();
    for (const auto& e : exps) {
        qDebug() << "经历:" << e.title << "-" << e.description;
    }

    // 测试奖项
    GpaCalculator::instance().addAward("奖学金", "学业优秀奖学金", "2025-01-10");
    auto awards = GpaCalculator::instance().getAllAwards();
    for (const auto& a : awards) {
        qDebug() << "奖项:" << a.title << "-" << a.description;
    }

    qDebug() << "========== 程序执行完毕 ==========";
    std::cout << "输入回车退出";
    std::cin.get();
    return 0;
>>>>>>> origin/feature-db
}
