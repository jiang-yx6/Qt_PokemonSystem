#ifndef USERMANAGER_H
#define USERMANAGER_H
#include <QString>
#include <QJsonArray>
#include "databasemanager.h"
#include <QJsonDocument>
#include <QJsonObject>
class UserManager
{
public:
    explicit UserManager(DatabaseManager *dbManager);
    QString registerUser(const QString &username, const QString &password);
    QString loginUser(const QString &username, const QString &password);
    QString checkInfo(const int &userid);
    QString logOut(const int &userid);
    int getUserId(const QString &username);


    DatabaseManager *m_dbManager;
};


#endif // USERMANAGER_H
