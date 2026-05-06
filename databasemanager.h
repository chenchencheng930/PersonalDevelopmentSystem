#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariantList>
#include <QVector>
#include <QString>

struct Course {
    int id;
    QString name;
    double credit;
    double gradePoint;
};

struct Experience {
    int id;
    QString title;
    QString description;
    QString date;
};

struct Award {
    int id;
    QString title;
    QString description;
    QString date;
};

class DatabaseManager
{
public:
    static DatabaseManager& instance();

    bool initDatabase(const QString& path = "personal.db");
    bool createTables();

    bool executeQuery(const QString& sql);
    bool executeQuery(const QString& sql, const QVariantList& params);
    QSqlQuery executeSelect(const QString& sql, const QVariantList& params = {});

private:
    DatabaseManager() = default;
    ~DatabaseManager();
    QSqlDatabase m_db;
    bool m_isOpen = false;
};

#endif // DATABASEMANAGER_H
