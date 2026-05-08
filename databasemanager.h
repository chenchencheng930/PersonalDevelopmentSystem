#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariantList>
#include <QString>

// 更新结构体：课程增加 score 和 semester
struct Course {
    int id;
    QString name;
    double credit;
    double score;      // 百分制分数
    QString semester;  // 学期，如 "2024-1"
};

// 经历增加 type 和 role
struct Experience {
    int id;
    QString type;       // 如 "实习"、"项目"
    QString title;
    QString description;
    QString date;
    QString role;       // 角色，如 "负责人"
};

// 奖项增加 level
struct Award {
    int id;
    QString title;
    QString level;      // 如 "国家级"
    QString description;
    QString date;
};

class DatabaseManager
{
public:
    static DatabaseManager& instance();

    // 初始化与建表
    bool initDatabase(const QString& path = "personal.db");
    bool createTables();

    // ---------- 底层通用方法----------
    bool executeQuery(const QString& sql);
    bool executeQuery(const QString& sql, const QVariantList& params);
    QSqlQuery executeSelect(const QString& sql, const QVariantList& params = {});

    // ---------- 课程业务接口 ----------
    bool addCourse(const QString& name, double credit, double score, const QString& semester);
    QSqlQuery getAllCourses();          // 返回查询结果，包含 id, name, credit, score, semester
    bool deleteCourse(int id);
    int getCourseCount();

    // ---------- 经历业务接口 ----------
    bool addExperience(const QString& type, const QString& title, const QString& date,
                       const QString& role, const QString& description);
    QSqlQuery getAllExperiences();      // 返回 id, type, title, date, role, description
    bool deleteExperience(int id);
    int getExperienceCount();
    bool hasInternship();               // 判断是否存在 type 为 "实习" 的经历

    // ---------- 奖项业务接口 ----------
    bool addAward(const QString& title, const QString& level, const QString& date, const QString& description);
    QSqlQuery getAllAwards();           // 返回 id, title, level, date, description
    bool deleteAward(int id);
    int getAwardCount();

private:
    DatabaseManager() = default;
    ~DatabaseManager();

    QSqlDatabase m_db;
    bool m_isOpen = false;

    // 辅助：为已有表添加新列（用于数据库升级）
    void addColumnIfMissing(const QString& table, const QString& columnDef);
};

#endif // DATABASEMANAGER_H
