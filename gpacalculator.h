#ifndef GPACALCULATOR_H
#define GPACALCULATOR_H

#include <QSqlQuery>
#include <QString>

class GpaCalculator
{
public:
    static GpaCalculator& instance();

    bool init();

    // 绩点转换与计算
    double scoreToGpa(double score);
    double calculateTotalGpa(QSqlQuery query);
    double calculateSemesterGpa(QSqlQuery query, const QString& semester);

    // ========== 课程接口 ==========
    // 增加课程（需要百分制分数和学期）
    bool addCourse(const QString& name, double credit, double score, const QString& semester);
    QSqlQuery getAllCourses();          // 返回结果集，包含 id, name, credit, score, semester
    bool deleteCourse(int id);          // 已存在，但参数名改为 id
    int getCourseCount();               // 课程总数

    // ========== 经历接口 ==========
    // 增加经历（包含类型、角色）
    bool addExperience(const QString& type, const QString& title, const QString& date,
                       const QString& role, const QString& description);
    QSqlQuery getAllExperiences();      // 返回 id, type, title, date, role, description
    bool deleteExperience(int id);      // 删除指定经历
    int getExperienceCount();           // 经历总数
    bool hasInternship();               // 判断是否有实习经历（type == "实习"）

    // ========== 奖项接口 ==========
    // 增加奖项（包含级别）
    bool addAward(const QString& title, const QString& level, const QString& date, const QString& description);
    QSqlQuery getAllAwards();           // 返回 id, title, level, date, description
    bool deleteAward(int id);           // 删除指定奖项
    int getAwardCount();                // 奖项总数

    // ========== 创新功能：为 GrowthAdvisor 提供数据 ==========
    double getTotalGpa();               // 所有课程的总加权平均绩点
    // 以下方法已在上面声明，但为清晰再次列出
    // int getCourseCount();
    // int getExperienceCount();
    // int getAwardCount();
    // bool hasInternship();

private:
    GpaCalculator() = default;
    ~GpaCalculator() = default;
};

#endif // GPACALCULATOR_H
