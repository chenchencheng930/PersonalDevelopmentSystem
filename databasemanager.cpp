#include "DatabaseManager.h"
#include <QDebug>

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager manager;
    return manager;
}

DatabaseManager::~DatabaseManager() {
    if (m_isOpen && m_db.isOpen()) {
        m_db.close();
    }
}

bool DatabaseManager::initDatabase(const QString& path) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);
    if (!m_db.open()) {
        qDebug() << "数据库打开失败:" << m_db.lastError().text();
        return false;
    }
    m_isOpen = true;
    return true;
}

// 辅助：为已有表添加新列（如果不存在）
void DatabaseManager::addColumnIfMissing(const QString& table, const QString& columnDef) {
    QSqlQuery q = executeSelect("PRAGMA table_info(" + table + ")");
    QString colName = columnDef.split(' ').first();
    while (q.next()) {
        if (q.value(1).toString() == colName) return;
    }
    executeQuery("ALTER TABLE " + table + " ADD COLUMN " + columnDef);
}

bool DatabaseManager::createTables() {
    if (!m_isOpen) return false;

    // 1. course 表（自动迁移旧表结构）
    QString sqlCourse = R"(
        CREATE TABLE IF NOT EXISTS course (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            credit REAL NOT NULL,
            score REAL,
            semester TEXT
        );
    )";
    if (!executeQuery(sqlCourse)) return false;

    // 兼容旧数据库：检测并移除 gradePoint 列（SQLite 需要重建表）
    QSqlQuery checkCol = executeSelect("PRAGMA table_info(course)");
    bool hasGradePoint = false;
    while (checkCol.next()) {
        if (checkCol.value(1).toString() == "gradePoint") {
            hasGradePoint = true;
            break;
        }
    }
    if (hasGradePoint) {
        executeQuery("ALTER TABLE course RENAME TO course_old");
        executeQuery(sqlCourse);
        executeQuery("INSERT INTO course (id, name, credit, score, semester) "
                     "SELECT id, name, credit, score, semester FROM course_old");
        executeQuery("DROP TABLE course_old");
    }

    // 确保新列存在（兼容旧数据库）
    addColumnIfMissing("course", "score REAL");
    addColumnIfMissing("course", "semester TEXT");

    // 2. experience 表（增加 type 和 role 列）
    QString sqlExperience = R"(
        CREATE TABLE IF NOT EXISTS experience (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            type TEXT,
            title TEXT NOT NULL,
            date TEXT,
            role TEXT,
            description TEXT
        );
    )";
    if (!executeQuery(sqlExperience)) return false;
    addColumnIfMissing("experience", "type TEXT");
    addColumnIfMissing("experience", "role TEXT");

    // 3. award 表（增加 level 列）
    QString sqlAward = R"(
        CREATE TABLE IF NOT EXISTS award (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            level TEXT,
            date TEXT,
            description TEXT
        );
    )";
    if (!executeQuery(sqlAward)) return false;
    addColumnIfMissing("award", "level TEXT");

    return true;
}

// ---------- 底层通用方法 ----------
bool DatabaseManager::executeQuery(const QString& sql) {
    QSqlQuery query(m_db);
    if (!query.exec(sql)) {
        qDebug() << "SQL执行失败:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::executeQuery(const QString& sql, const QVariantList& params) {
    QSqlQuery query(m_db);
    query.prepare(sql);
    for (const QVariant& param : params) {
        query.addBindValue(param);
    }
    if (!query.exec()) {
        qDebug() << "带参SQL执行失败:" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQuery DatabaseManager::executeSelect(const QString& sql, const QVariantList& params) {
    QSqlQuery query(m_db);
    query.prepare(sql);
    for (const QVariant& param : params) {
        query.addBindValue(param);
    }
    if (!query.exec()) {
        qDebug() << "查询执行失败:" << query.lastError().text();
    }
    return query;
}

// ---------- 课程业务接口 ----------
bool DatabaseManager::addCourse(const QString& name, double credit, double score, const QString& semester) {
    return executeQuery("INSERT INTO course (name, credit, score, semester) VALUES (?, ?, ?, ?)",
                        {name, credit, score, semester});
}

QSqlQuery DatabaseManager::getAllCourses() {
    return executeSelect("SELECT id, name, credit, score, semester FROM course ORDER BY id");
}

bool DatabaseManager::deleteCourse(int id) {
    return executeQuery("DELETE FROM course WHERE id = ?", {id});
}

int DatabaseManager::getCourseCount() {
    QSqlQuery q = executeSelect("SELECT COUNT(*) FROM course");
    if (q.next()) return q.value(0).toInt();
    return 0;
}

// ---------- 经历业务接口 ----------
bool DatabaseManager::addExperience(const QString& type, const QString& title, const QString& date,
                                    const QString& role, const QString& description) {
    return executeQuery("INSERT INTO experience (type, title, date, role, description) VALUES (?, ?, ?, ?, ?)",
                        {type, title, date, role, description});
}

QSqlQuery DatabaseManager::getAllExperiences() {
    return executeSelect("SELECT id, type, title, date, role, description FROM experience ORDER BY id");
}

bool DatabaseManager::deleteExperience(int id) {
    return executeQuery("DELETE FROM experience WHERE id = ?", {id});
}

int DatabaseManager::getExperienceCount() {
    QSqlQuery q = executeSelect("SELECT COUNT(*) FROM experience");
    if (q.next()) return q.value(0).toInt();
    return 0;
}

bool DatabaseManager::hasInternship() {
    QSqlQuery q = executeSelect("SELECT COUNT(*) FROM experience WHERE type = '实习'");
    if (q.next()) return q.value(0).toInt() > 0;
    return false;
}

// ---------- 奖项业务接口 ----------
bool DatabaseManager::addAward(const QString& title, const QString& level, const QString& date, const QString& description) {
    return executeQuery("INSERT INTO award (title, level, date, description) VALUES (?, ?, ?, ?)",
                        {title, level, date, description});
}

QSqlQuery DatabaseManager::getAllAwards() {
    return executeSelect("SELECT id, title, level, date, description FROM award ORDER BY id");
}

bool DatabaseManager::deleteAward(int id) {
    return executeQuery("DELETE FROM award WHERE id = ?", {id});
}

int DatabaseManager::getAwardCount() {
    QSqlQuery q = executeSelect("SELECT COUNT(*) FROM award");
    if (q.next()) return q.value(0).toInt();
    return 0;
}
