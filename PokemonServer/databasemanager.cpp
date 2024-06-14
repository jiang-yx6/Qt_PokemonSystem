#include "databasemanager.h"

DatabaseManager::DatabaseManager() {}
DatabaseManager::~DatabaseManager() {}

bool DatabaseManager::initializeDatabase()
{
    //创建数据库 命名为Users.db
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","initialize");
    db.setDatabaseName("Users.db");
    //打开数据库
    if(!db.open())
    {
        qWarning("users.db open failed~");
        return false;
    }
    //建立query连接 创建users表
    QSqlQuery query(db);
    if(!query.exec("CREATE TABLE IF NOT EXISTS users(userid INTEGER PRIMARY KEY AUTOINCREMENT, username VARCHAR(20) UNIQUE NOT NULL, password VARCHAR(20)  NOT NULL, status VARCHAR(10),gamenumber INTEGER,winnumber INTEGER,countbadge VARCHAR(10),superbadge VARCHAR(10))"))
    {
        qWarning("create table users failed: %s", qPrintable(query.lastError().text()));
        return false;
    }
    db.close();
    return true;
}

bool DatabaseManager::createPokemonTable()
{
    if (!QSqlDatabase::contains("initialize")) {
        qWarning("Database connection 'initialize' not found");
        return false;
    }
    // 使用已存在的数据库连接
    QSqlDatabase db = QSqlDatabase::database("initialize");
    if (!db.isOpen()) {
        qWarning("Database not open");
        return false;
    }
    //创建pokemons表并与users表通过userid关联
    QSqlQuery query(db);
    if (!query.exec("CREATE TABLE IF NOT EXISTS pokemons(id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(20),level INTEGER, exp INTEGER, attackPower INTEGER,"
                    " defensePower INTEGER, healthPoints INTEGER , speedValue INTEGER , userid INTEGER, FOREIGN KEY (userid) REFERENCES users(userid))")) {
        qWarning("Create pokemons table failed: %s", qPrintable(query.lastError().text()));
        return false;
    }
    db.close();
    return true;
}

void DatabaseManager::initConnection() {
    QSqlDatabase db_1 = QSqlDatabase::addDatabase("QSQLITE", "login");
    db_1.setDatabaseName("Users.db");
    if (!db_1.open()) {
        qWarning() << "无法打开login数据库：" << db_1.lastError().text();
    }

    QSqlDatabase db_2 = QSqlDatabase::addDatabase("QSQLITE", "register");
    db_2.setDatabaseName("Users.db");
    if (!db_2.open()) {
        qWarning() << "无法打开register数据库：" << db_2.lastError().text();
    }

    QSqlDatabase db_3 = QSqlDatabase::addDatabase("QSQLITE", "storepokemon");
    db_3.setDatabaseName("Users.db");
    if (!db_3.open()) {
        qWarning() << "无法打开storepokemon数据库：" << db_3.lastError().text();
    }

    QSqlDatabase db_4 = QSqlDatabase::addDatabase("QSQLITE", "check");
    db_4.setDatabaseName("Users.db");
    if (!db_4.open()) {
        qWarning() << "无法打开check数据库：" << db_4.lastError().text();
    }

    QSqlDatabase db_5 = QSqlDatabase::addDatabase("QSQLITE", "logout");
    db_5.setDatabaseName("Users.db");
    if (!db_5.open()) {
        qWarning() << "无法打开check数据库：" << db_5.lastError().text();
    }
}


QSqlDatabase DatabaseManager::createConnection(const QString &connectionName)
{
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    db.setDatabaseName("Users.db");
    if (db.isValid() && db.isOpen()) {
        return db;
    } else {
        qWarning() << "无法获取数据库连接：" << connectionName;
        return QSqlDatabase();
    }
}
