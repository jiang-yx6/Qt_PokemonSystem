#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QSqlDatabase> // 用于访问SQLite数据库
#include <QSqlQuery> // 用于执行数据查询
#include <QSqlError>
class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();

    bool initializeDatabase();
    bool createPokemonTable();
    void initConnection();

    QSqlDatabase createConnection(const QString &connectionName);
};

#endif // DATABASEMANAGER_H
