#ifndef GPACALCULATOR_H
#define GPACALCULATOR_H

#include "DatabaseManager.h"
#include <QVector>

class GpaCalculator
{
public:
    static GpaCalculator& instance();

    bool init();

    // 课程接口
    bool addCourse(const QString& name, double credit, double gradePoint);
    QVector<Course> getAllCourses();
    bool deleteCourse(int courseId);
    int getCourseCount();

    // 经历接口
    bool addExperience(const QString& title, const QString& description, const QString& date);
    QVector<Experience> getAllExperiences();

    // 奖项接口
    bool addAward(const QString& title, const QString& description, const QString& date);
    QVector<Award> getAllAwards();

private:
    GpaCalculator() = default;
    ~GpaCalculator() = default;
};

#endif // GPACALCULATOR_H
