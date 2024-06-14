#include "usermanager.h"
UserManager::UserManager(DatabaseManager *dbManager) : m_dbManager(dbManager) {

}

QString UserManager::registerUser(const QString &username, const QString &password)
{
    //建立register的连接到SQLite
    QSqlDatabase db = m_dbManager->createConnection("register");
    db.setDatabaseName("Users.db");
    //打开数据库
    if (!db.open()) {
        qDebug() << "无法打开数据库：" << db.lastError().text();
        return QString(R"({"status":"error","code":"500","message":"无法打开数据库"})");
    }
    QSqlQuery query(db);
    //查询用户是否已经存在
    if(query.prepare("SELECT COUNT(*) FROM users WHERE username = :username AND password = :password")){
        query.bindValue(":username", username);
        query.bindValue(":password", password);
        if(query.exec() && query.next()){
            int count = query.value(0).toInt();
            if(count > 0){
                db.close();
                return QString(R"({"status":"failed","code":"409","message":"用户已存在!"})");
            }
        } else {
            return QString(R"({"status":"error","code":"500","message":"%1"})").arg(query.lastError().text());
        }
    } else {
        return QString(R"({"status":"error","code":"500","message":"Prepare failed: %1"})").arg(query.lastError().text());
    }
    //插入用户
    if(query.prepare("INSERT INTO users (username, password,status,gamenumber,winnumber,countbadge,superbadge) VALUES (:username, :password,:status,:gamenumber,:winnumber,:countbadge,:superbadge)")){
        query.bindValue(":username", username);
        query.bindValue(":password", password);
        query.bindValue(":status", "False");
        query.bindValue(":gamenumber", 0);
        query.bindValue(":winnumber", 0);
        query.bindValue(":countbadge", "无");
        query.bindValue(":superbadge", "无");
        if(query.exec()) {
            db.close();
            return QString(R"({"status":"success","code":"201","message":"注册成功!"})");
        }
        else{
            return QString(R"({"status":"error","code":"500","message":"%1"})").arg(query.lastError().text());
        }
    }
    else
    {
        return QString(R"({"status":"error","code":"500","message":"Prepare failed: %1"})").arg(query.lastError().text());
    }
}

QString UserManager::checkInfo(const int &userid)
{
    QJsonObject AllInfo;
    QJsonArray PokejsonArray;
    QJsonArray UserjsonArray;
    QSqlDatabase db =m_dbManager->createConnection("check");
    db.setDatabaseName("Users.db");
    if(!db.isOpen()){
        qDebug() << "无法打开数据库：" << db.lastError().text();
        return QString(R"({"status":"error","code":"500","message":"无法打开数据库"})");
    }
    QSqlQuery query(db);
    if(!query.prepare("SELECT * FROM pokemons WHERE userid = :userid")){
        qDebug()<<"error";
    }
    query.bindValue(":userid",userid);
    query.exec();
    // 用于存放所有精灵的JSON对象
    while (query.next()) {
        QJsonObject pokemonObj; // 用于构建单个精灵的JSON对象
        // 假设精灵表中有id, name, level, type等字段
        pokemonObj["id"] = query.value("id").toInt();
        pokemonObj["name"] = query.value("name").toString();
        pokemonObj["level"] = query.value("level").toInt();
        pokemonObj["exp"] = query.value("exp").toString(); // 假设type也是字符串类型
        pokemonObj["attackPower"] = query.value("attackPower").toString();
        pokemonObj["defensePower"] = query.value("defensePower").toString();
        pokemonObj["healthPoints"] = query.value("healthPoints").toString();
        pokemonObj["speedValue"] = query.value("speedValue").toString();
        // 添加到数组中
        PokejsonArray.append(pokemonObj);
    }
    AllInfo["pokemon"]=PokejsonArray;

    //用于存放所有status为True的用户
    if(!query.prepare("SELECT * FROM users WHERE status = :status")){
        qDebug()<<"error";
    }
    query.bindValue(":status","True");
    query.exec();
    while (query.next()) {
        QJsonObject UserStatus; // 用于构建单个精灵的JSON对象
        // 假设精灵表中有id, name, level, type等字段
        UserStatus["userid"] = query.value("userid").toString();
        UserStatus["username"] = query.value("username").toString();
        // 添加到数组中
        UserjsonArray.append(UserStatus);
    }
    AllInfo["truestatus"]=UserjsonArray;

    //用于存放 当前账户的用户名和密码
    if(!query.prepare("SELECT * FROM users WHERE userid = :userid")){
        qDebug()<<"error";
    }
    query.bindValue(":userid",userid);
    query.exec();
    query.next();
    AllInfo["username"]=query.value("username").toString();
    AllInfo["password"]=query.value("password").toString();
    AllInfo["status"]=query.value("status").toString();
    AllInfo["countbadge"]= query.value("countbadge").toString();
    AllInfo["superbadge"]= query.value("superbadge").toString();
    if(query.value("gamenumber").toInt()==0)
        AllInfo["winrate"]=0;
    else
        AllInfo["winrate"]=(query.value("winnumber").toDouble()/query.value("gamenumber").toInt());
    QJsonDocument jsonDoc(AllInfo);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Compact);
    return jsonString;
}

QString UserManager::loginUser(const QString &username, const QString &password)
{
    QSqlDatabase db = m_dbManager->createConnection("login");
    db.setDatabaseName("Users.db");
    if (!db.open()) {
        qDebug() << "无法打开数据库：" << db.lastError().text();
        return QString(R"({"status":"error","code":"500","message":"无法打开数据库"})");
    }
    QSqlQuery query(db);
    int userId;
    if(query.prepare("SELECT COUNT(*) FROM users WHERE username = :username AND password = :password")){
        query.bindValue(":username", username);
        query.bindValue(":password", password);
        if(query.exec() && query.next()){
            int count = query.value(0).toInt();
            if(count > 0){
                userId = getUserId(username);
            } else {
                return QString(R"({"status":"failed","code":"401","message":"账号和密码不正确!!!"})");
            }
        } else {
            return QString(R"({"status":"error","code":"500","message":"%1"})").arg(query.lastError().text());
        }
    } else {
        return QString(R"({"status":"error","code":"500","message":"Prepare failed: %1"})").arg(query.lastError().text());
    }
    query.prepare("SELECT status FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    if (query.exec() && query.next()) {
        // 获取查询结果中的status
        QString userStatus = query.value(0).toString(); // 假设status是字符串类型
        qDebug() << "User Status:" << userStatus;
        if (userStatus == "True")
            return QString(R"({"status":"failed","code":"401","message":"用户已登录"})");
    } else {
        // 如果没有找到匹配的用户，或者查询出错
        if (query.lastError().type() != QSqlError::NoError) {
            qDebug() << "Query failed:" << query.lastError().text();
        } else {
            qDebug() << "No user found with the given username and password.";
        }
    }
    if(query.prepare("UPDATE users SET status = :status WHERE userid = :userid"))
    {
        query.bindValue(":status","True");
        query.bindValue(":userid",userId);
        if(!query.exec())
        {
            return QString(R"({"status":"failed","code":"401","message":"更改Status失败"})");
        }
        else return QString(R"({"status":"success","code":"200","message":"登陆成功!","userid":%1})").arg(userId);
    }
    db.close();
    return QString(R"({"status":"failed","code":"401","message":"更改Status失败"})");
}

int UserManager::getUserId(const QString &username)
{
    QSqlDatabase db = m_dbManager->createConnection("login");
    if (db.open()) {
        QSqlQuery query(db);
        if (query.prepare("SELECT userid FROM users WHERE username = ?")) {
            query.addBindValue(username);
            if (query.exec() && query.next()) {
                return query.value(0).toInt();
            }
            else{
                if (query.lastError().type() == QSqlError::NoError) {
                    // 没有找到用户名，可以视为错误处理
                    qCritical() << "用户名未找到: " << username;
                } else {
                    qCritical() << "查询执行失败: " << query.lastError().text();
                }
            }
        } else {
            qDebug() << "Failed to prepare query: " << query.lastError().text();
        }
    } else {
        qDebug() << "Failed to open database: " << db.lastError().text();
    }
    return -1;
}

QString UserManager::logOut(const int &userid)
{
    QSqlDatabase db = m_dbManager->createConnection("login");
    if(db.open()){
        QSqlQuery query(db);
        if(query.prepare("UPDATE users SET status =:status WHERE userid =:userid")){
            query.bindValue(":status","False");
            query.bindValue(":userid",userid);
            if (!query.exec()) {
                return QString(R"({"status":"failed","code":"401","message":"更改Status失败"})");
            }
            else
                return QString(R"({"status":"success","code":"200","message":"登出成功!"})");
        }
    }
    else
    {    qDebug() << "无法打开数据库：" << db.lastError().text();
        return QString(R"({"status":"error","code":"500","message":"无法打开数据库"})");
    }
    return QString(R"({"status":"failed","code":"401","message":"logout失败"})");
}
