#include "GpaCalculator.h"
#include "DatabaseManager.h"

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

// ========== 绩点转换与计算 ==========
double GpaCalculator::scoreToGpa(double score) {
    if (score >= 90) return 4.0;
    if (score >= 85) return 3.7;
    if (score >= 82) return 3.3;
    if (score >= 78) return 3.0;
    if (score >= 75) return 2.7;
    if (score >= 72) return 2.3;
    if (score >= 68) return 2.0;
    if (score >= 64) return 1.5;
    if (score >= 60) return 1.0;
    return 0.0;
}

double GpaCalculator::calculateTotalGpa(QSqlQuery query) {
    double totalCredits = 0.0;
    double totalPoints = 0.0;
    while (query.next()) {
        double credit = query.value("credit").toDouble();
        double score = query.value("score").toDouble();
        double gpa = scoreToGpa(score);
        totalCredits += credit;
        totalPoints += credit * gpa;
    }
    return totalCredits == 0 ? 0.0 : totalPoints / totalCredits;
}

double GpaCalculator::calculateSemesterGpa(QSqlQuery query, const QString& semester) {
    double totalCredits = 0.0;
    double totalPoints = 0.0;
    while (query.next()) {
        if (query.value("semester").toString() != semester) continue;
        double credit = query.value("credit").toDouble();
        double score = query.value("score").toDouble();
        double gpa = scoreToGpa(score);
        totalCredits += credit;
        totalPoints += credit * gpa;
    }
    return totalCredits == 0 ? 0.0 : totalPoints / totalCredits;
}

// ========== 课程接口 ==========
bool GpaCalculator::addCourse(const QString& name, double credit, double score, const QString& semester) {
    // 直接调用 DatabaseManager 的新版 addCourse（接收 score 和 semester）
    return DatabaseManager::instance().addCourse(name, credit, score, semester);
}

QSqlQuery GpaCalculator::getAllCourses() {
    return DatabaseManager::instance().getAllCourses();
}

bool GpaCalculator::deleteCourse(int id) {
    return DatabaseManager::instance().deleteCourse(id);
}

int GpaCalculator::getCourseCount() {
    return DatabaseManager::instance().getCourseCount();
}

// ========== 经历接口 ==========
bool GpaCalculator::addExperience(const QString& type, const QString& title, const QString& date,
                                  const QString& role, const QString& description) {
    return DatabaseManager::instance().addExperience(type, title, date, role, description);
}

QSqlQuery GpaCalculator::getAllExperiences() {
    return DatabaseManager::instance().getAllExperiences();
}

bool GpaCalculator::deleteExperience(int id) {
    return DatabaseManager::instance().deleteExperience(id);
}

int GpaCalculator::getExperienceCount() {
    return DatabaseManager::instance().getExperienceCount();
}

bool GpaCalculator::hasInternship() {
    return DatabaseManager::instance().hasInternship();
}

// ========== 奖项接口 ==========
bool GpaCalculator::addAward(const QString& title, const QString& level, const QString& date, const QString& description) {
    return DatabaseManager::instance().addAward(title, level, date, description);
}

QSqlQuery GpaCalculator::getAllAwards() {
    return DatabaseManager::instance().getAllAwards();
}

bool GpaCalculator::deleteAward(int id) {
    return DatabaseManager::instance().deleteAward(id);
}

int GpaCalculator::getAwardCount() {
    return DatabaseManager::instance().getAwardCount();
}

// ========== 创新功能：为 GrowthAdvisor 提供数据 ==========
double GpaCalculator::getTotalGpa() {
    QSqlQuery query = getAllCourses();
    return calculateTotalGpa(query);
}
