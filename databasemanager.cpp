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

bool DatabaseManager::createTables() {
    if (!m_isOpen) return false;

    QString sqlCourse = R"(
        CREATE TABLE IF NOT EXISTS course (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            credit REAL NOT NULL,
            gradePoint REAL NOT NULL
        );
    )";
    if (!executeQuery(sqlCourse)) return false;

    QString sqlExperience = R"(
        CREATE TABLE IF NOT EXISTS experience (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            description TEXT,
            date TEXT
        );
    )";
    if (!executeQuery(sqlExperience)) return false;

    QString sqlAward = R"(
        CREATE TABLE IF NOT EXISTS award (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            description TEXT,
            date TEXT
        );
    )";
    if (!executeQuery(sqlAward)) return false;

    return true;
}

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
