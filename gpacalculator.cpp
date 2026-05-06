#include "GpaCalculator.h"

GpaCalculator& GpaCalculator::instance() {
    static GpaCalculator calculator;
    return calculator;
}

bool GpaCalculator::init() {
    DatabaseManager& db = DatabaseManager::instance();
    if (!db.initDatabase("personal.db")) return false;
    if (!db.createTables()) return false;
    return true;
}

// ---------- 课程 ----------
bool GpaCalculator::addCourse(const QString& name, double credit, double gradePoint) {
    QString sql = "INSERT INTO course (name, credit, gradePoint) VALUES (?, ?, ?)";
    return DatabaseManager::instance().executeQuery(sql, {name, credit, gradePoint});
}

QVector<Course> GpaCalculator::getAllCourses() {
    QVector<Course> courses;
    QSqlQuery query = DatabaseManager::instance().executeSelect("SELECT id, name, credit, gradePoint FROM course");
    while (query.next()) {
        Course c;
        c.id = query.value(0).toInt();
        c.name = query.value(1).toString();
        c.credit = query.value(2).toDouble();
        c.gradePoint = query.value(3).toDouble();
        courses.append(c);
    }
    return courses;
}

bool GpaCalculator::deleteCourse(int courseId) {
    QString sql = "DELETE FROM course WHERE id = ?";
    return DatabaseManager::instance().executeQuery(sql, {courseId});
}

int GpaCalculator::getCourseCount() {
    QSqlQuery query = DatabaseManager::instance().executeSelect("SELECT COUNT(*) FROM course");
    if (query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

// ---------- 经历 ----------
bool GpaCalculator::addExperience(const QString& title, const QString& description, const QString& date) {
    QString sql = "INSERT INTO experience (title, description, date) VALUES (?, ?, ?)";
    return DatabaseManager::instance().executeQuery(sql, {title, description, date});
}

QVector<Experience> GpaCalculator::getAllExperiences() {
    QVector<Experience> experiences;
    QSqlQuery query = DatabaseManager::instance().executeSelect("SELECT id, title, description, date FROM experience");
    while (query.next()) {
        Experience e;
        e.id = query.value(0).toInt();
        e.title = query.value(1).toString();
        e.description = query.value(2).toString();
        e.date = query.value(3).toString();
        experiences.append(e);
    }
    return experiences;
}

// ---------- 奖项 ----------
bool GpaCalculator::addAward(const QString& title, const QString& description, const QString& date) {
    QString sql = "INSERT INTO award (title, description, date) VALUES (?, ?, ?)";
    return DatabaseManager::instance().executeQuery(sql, {title, description, date});
}

QVector<Award> GpaCalculator::getAllAwards() {
    QVector<Award> awards;
    QSqlQuery query = DatabaseManager::instance().executeSelect("SELECT id, title, description, date FROM award");
    while (query.next()) {
        Award a;
        a.id = query.value(0).toInt();
        a.title = query.value(1).toString();
        a.description = query.value(2).toString();
        a.date = query.value(3).toString();
        awards.append(a);
    }
    return awards;
}
