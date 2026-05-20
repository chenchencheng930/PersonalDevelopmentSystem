#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include "GpaCalculator.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // 1. 初始化数据库（会创建 personal.db 文件和三张表）
    if (!GpaCalculator::instance().init()) {
        qDebug() << " 数据库初始化失败！";
        return 1;
    }
    qDebug() << "数据库初始化成功";

    // 2. 添加课程（含学分、分数、学期）
    GpaCalculator::instance().addCourse("高等数学", 5.0, 88, "2024-1");
    GpaCalculator::instance().addCourse("大学英语", 3.0, 92, "2024-1");
    GpaCalculator::instance().addCourse("数据结构", 4.0, 75, "2024-2");
    GpaCalculator::instance().addCourse("计算机组成原理", 4.0, 68, "2024-2");
    qDebug() << "课程总数:" << GpaCalculator::instance().getCourseCount();

    // 3. 添加经历（实习、项目）
    GpaCalculator::instance().addExperience("实习", "腾讯前端实习", "2024-07", "前端开发", "负责页面实现");
    GpaCalculator::instance().addExperience("项目", "智能车竞赛", "2024-05", "队长", "获得省级一等奖");
    qDebug() << " 经历总数:" << GpaCalculator::instance().getExperienceCount();
    qDebug() << "是否有实习:" << (GpaCalculator::instance().hasInternship() ? "是" : "否");

    // 4. 添加奖项
    GpaCalculator::instance().addAward("国家奖学金", "国家级", "2024-10", "成绩专业第一");
    GpaCalculator::instance().addAward("优秀学生干部", "校级", "2024-12", "组织能力强");
    qDebug() << " 奖项总数:" << GpaCalculator::instance().getAwardCount();

    // 5. 测试总绩点和学期绩点
    double totalGpa = GpaCalculator::instance().getTotalGpa();
    qDebug() << " 总绩点 (加权平均):" << totalGpa;

    // 计算 2024-1 学期绩点
    auto allCourses = GpaCalculator::instance().getAllCourses();
    double sem1Gpa = GpaCalculator::instance().calculateSemesterGpa(allCourses, "2024-1");
    qDebug() << "2024-1 学期绩点:" << sem1Gpa;

    // 6. 展示所有课程详情
    qDebug() << "\n========== 课程列表 ==========";
    QSqlQuery courseQuery = GpaCalculator::instance().getAllCourses();
    while (courseQuery.next()) {
        qDebug() << courseQuery.value("id").toInt()
                 << courseQuery.value("name").toString()
                 << "学分:" << courseQuery.value("credit").toDouble()
                 << "分数:" << courseQuery.value("score").toDouble()
                 << "学期:" << courseQuery.value("semester").toString();
    }

    // 7. 测试删除功能（以删除一门课程为例）
    qDebug() << "\n 删除 ID=3 的课程（数据结构）...";
    if (GpaCalculator::instance().deleteCourse(3)) {
        qDebug() << "删除成功，剩余课程数:" << GpaCalculator::instance().getCourseCount();
    }

    qDebug() << "\n 测试完成！按回车键退出...";
    std::cin.get();
    return 0;
}
