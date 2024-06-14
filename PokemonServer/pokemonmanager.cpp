#include "pokemonmanager.h"

PokemonManager::PokemonManager(DatabaseManager *dbManager) : m_dbManager(dbManager) {}

QString PokemonManager::storePokemon(const QString &username, const QJsonArray &pokemons)
{
    QSqlDatabase db = m_dbManager->createConnection("storepokemon");
    if (db.open()) {
        QSqlQuery query(db);
        if (query.prepare("INSERT INTO pokemons (userid, name, level, exp, attackPower, defensePower, healthPoints, speedValue) VALUES (?, ?, ?, ?, ?, ?, ?, ?)")) {
            int userId = getUserId(username);
            if (userId > 0) {
                for (const QJsonValue &pokemonValue : pokemons) {
                    if (pokemonValue.isObject()) {
                        QJsonObject pokemonObject = pokemonValue.toObject();
                        //MyaddBindValue(query,userId,pokemonObject);
                        query.addBindValue(userId);
                        query.addBindValue(pokemonObject.value("name"));
                        query.addBindValue(pokemonObject.value("level").toInt());
                        query.addBindValue(pokemonObject.value("exp").toInt());
                        query.addBindValue(pokemonObject.value("attackPower").toInt());
                        query.addBindValue(pokemonObject.value("defensePower").toInt());
                        query.addBindValue(pokemonObject.value("healthPoints").toInt());
                        double speed = pokemonObject.value("speedValue").toDouble();
                        QString speedStr = QString::number(speed, 'f', 3); // 'f'表示浮点数格式，第二个参数是小数位数
                        query.addBindValue(speedStr);
                        if (!query.exec()) {
                            qDebug() << "Failed to store pokemon: " << query.lastError().text();
                            return  QString(R"({"status":"error","code":"500","message":"存储精灵时发生错误"})");;
                        }
                    }
                }
                return QString(R"({"status":"success","code":"201","message":"精灵存储成功"})");;
            } else {
                qDebug() << "User ID not found for username: " << username;
                return QString(R"({"status":"error","code":"500","message":"User ID not found: %1"})").arg(query.lastError().text());
            }
        } else {
            qDebug() << "Failed to prepare query: " << query.lastError().text();
            return QString(R"({"status":"error","code":"500","message":"Prepare failed: %1"})").arg(query.lastError().text());
        }
    } else {
        qDebug() << "Failed to open database: " << db.lastError().text();
        return QString(R"({"status":"error","code":"500","message":"Open database failed"})");
    }
        db.close();
}

int PokemonManager::getUserId(const QString &username)
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

QString PokemonManager::update(QJsonObject pokemonObject){
    QSqlDatabase db = m_dbManager->createConnection("storepokemon");
    if (db.open()) {
        QSqlQuery query(db);
        if(query.prepare("UPDATE pokemons SET level = :level,attackPower =:attackPower, defensePower=:defensePower,healthPoints=:healthPoints,speedValue=:speedValue,exp =:exp WHERE id = :id"))
        {
            query.bindValue(":level",pokemonObject.value("level").toInt());
            query.bindValue(":exp",pokemonObject.value("exp").toInt());
            query.bindValue(":attackPower",pokemonObject.value("attackPower").toInt());
            query.bindValue(":defensePower",pokemonObject.value("defensePower").toInt());
            query.bindValue(":healthPoints",pokemonObject.value("healthPoints").toInt());
            double speed = pokemonObject.value("speedValue").toDouble();
            QString speedStr = QString::number(speed, 'f', 3); // 'f'表示浮点数格式，第二个参数是小数位数
            query.bindValue(":speedValue",speedStr);
            query.bindValue(":id",pokemonObject.value("id").toInt());
            if( !query.exec())
            {
                return QString(R"({"status":"failed","code":"401","message":"更新pokemon失败"})");
            }
        }
        //更新高级徽章
        query.prepare("SELECT COUNT(*) FROM pokemons WHERE userid = :userid AND level = 15");
        query.bindValue(":userid",pokemonObject.value("userid").toInt());
        if (query.exec() && query.next()) {
            // 获取查询结果中的计数
            int pokemonCount = query.value(0).toInt();
            QString superbadge;
            if(pokemonCount == 1||pokemonCount == 5||pokemonCount == 10)
            {
                if(pokemonCount == 1)
                {
                    superbadge="铜";
                }
                else if(pokemonCount == 5)
                {
                    superbadge="银";
                }
                else if(pokemonCount == 10)
                {
                    superbadge="金";
                }
                QSqlQuery query2(db);
                query2.prepare("UPDATE users SET superbadge = :superbadge WHERE userid =:userid");
                query2.bindValue(":userid",pokemonObject.value("userid").toInt());
                query2.bindValue(":superbadge",superbadge);
                if(query2.exec())
                {
                    qDebug()<<QString(R"({"status":"success","code":"200","message":"更新成功!"})");
                    return QString(R"({"status":"success","code":"200","message":"更新成功!"})");
                }
            }
            qDebug() << "用户数量：" << pokemonCount;
        } else {
            qDebug() << "查询失败：" << query.lastError().text();
        }
    }
    else
        return QString(R"({"status":"error","code":"500","message":"Open database failed"})");

    db.close();
    return QString(R"({"status":"error","code":"500","message":"Open database failed"})");
}

QString PokemonManager::addPokemon(QJsonObject pokemonObject)
{
    QSqlDatabase db = m_dbManager->createConnection("storepokemon");
    if (db.open()) {
        QSqlQuery query(db);
        if (query.prepare("INSERT INTO pokemons (userid, name, level, exp, attackPower, defensePower, healthPoints, speedValue) VALUES (?, ?, ?, ?, ?, ?, ?, ?)")) {
                        //MyaddBindValue(query,userId,pokemonObject);
                        query.addBindValue(pokemonObject.value("userid").toInt());
                        query.addBindValue(pokemonObject.value("name").toString());
                        query.addBindValue(pokemonObject.value("level").toInt());
                        query.addBindValue(pokemonObject.value("exp").toInt());
                        query.addBindValue(pokemonObject.value("attackPower").toInt());
                        query.addBindValue(pokemonObject.value("defensePower").toInt());
                        query.addBindValue(pokemonObject.value("healthPoints").toInt());
                        double speed = pokemonObject.value("speedValue").toDouble();
                        QString speedStr = QString::number(speed, 'f', 3); // 'f'表示浮点数格式，第二个参数是小数位数
                        query.addBindValue(speedStr);
                        if (!query.exec()) {
                            qDebug() << "Failed to store pokemon: " << query.lastError().text();
                            return  QString(R"({"status":"error","code":"500","message":"添加精灵时发生错误"})");
                        }
        } else {
            qDebug() << "Failed to prepare query: " << query.lastError().text();
            return QString(R"({"status":"error","code":"500","message":"Prepare failed: %1"})").arg(query.lastError().text());
        }
        //更新徽章
        if(query.prepare("SELECT COUNT(*) FROM pokemons WHERE userid = :userid"))
        {
            query.bindValue(":userid",pokemonObject.value("userid").toInt());
            if (query.exec() && query.next()) {
                // 获取查询结果中的计数
                int pokemonCount = query.value(0).toInt();
                QString countbadge;
                if(pokemonCount == 5||pokemonCount == 10||pokemonCount == 15)
                {
                    if(pokemonCount == 5)
                    {
                        countbadge="铜";
                    }
                    else if(pokemonCount == 10)
                    {
                        countbadge="银";
                    }
                    else if(pokemonCount == 15)
                    {
                        countbadge="金";
                    }
                    QSqlQuery query2(db);
                    query2.prepare("UPDATE users SET countbadge = :countbadge WHERE userid =:userid");
                    query2.bindValue(":userid",pokemonObject.value("userid").toInt());
                    query2.bindValue(":countbadge",countbadge);
                    if(query2.exec())
                        qDebug()<<QString(R"({"status":"success","code":"200","message":"更新成功!"})");
                }
                qDebug() << "用户数量：" << pokemonCount;
            } else {
                qDebug() << "查询失败：" << query.lastError().text();
            }
        }

        QSqlQuery query3(db);
        if(query3.prepare("UPDATE users SET gamenumber =gamenumber+1,winnumber =winnumber+1 WHERE userid = :userid"))
        {
            query3.bindValue(":userid",pokemonObject.value("userid").toInt());
            if( !query3.exec())
            {
                return QString(R"({"status":"failed","code":"401","message":"更新pokemon失败"})");
            }
            else return QString(R"({"status":"success","code":"501","message":"精灵添加成功,信息更新成功"})");
        }

    } else {
        qDebug() << "Failed to open database: " << db.lastError().text();
        return QString(R"({"status":"error","code":"500","message":"Open database failed"})");
    }

    db.close();
}

QString PokemonManager::discardpokemon(int idToDelete)
{
    QSqlDatabase db = m_dbManager->createConnection("storepokemon");
    if(db.open())
    {
        QSqlQuery query(db);
        QString userId;
        qDebug()<<"deleting:"<<idToDelete;
        if(query.prepare("SELECT userid FROM pokemons WHERE id = ?"))
        {
            query.addBindValue(idToDelete);
            if (query.exec() && query.next()) {
                // 获取查询结果
                userId = query.value("userid").toString();
                qDebug() << "Userid for id" << idToDelete << "is:" << userId;
            }else {
                if (query.lastError().type() != QSqlError::NoError) {
                    qDebug() << "Query error:" << query.lastError().text();
                } else {
                    qDebug() << "No data found for the given id.";
                }
            }
        }
        if(query.prepare("DELETE FROM pokemons WHERE id = ?"))
        {
            query.addBindValue(idToDelete);
            qDebug()<<"deleting:"<<idToDelete;
            if (!query.exec()) {
                qDebug() << "Delete failed:" << query.lastError().text();
                QString(R"({"status":"failed","code":"500","message":"精灵删除失败"})");
            }
        }
        if(query.prepare("SELECT COUNT (*) AS count FROM pokemons WHERE userid = ?"))
        {
            query.addBindValue(userId);
            if (query.exec()) {
                if (query.next()) {
                    int count = query.value("count").toInt();
                    if (count == 0) {
                        qDebug() << "No pokemons found for user with id" << userId;
                        qDebug() << "Pokemon with ID" << idToDelete << "successfully deleted.";
                        return QString(R"({"status":"success_but_no_pokemon","code":"201","message":"精灵删除成功,该用户无宝可梦"})");
                    } else {
                        QSqlQuery query3(db);
                        if(query3.prepare("UPDATE users SET gamenumber =gamenumber+1 WHERE userid = :userid"))
                        {
                            query3.bindValue(":userid",userId);
                            if( !query3.exec())
                            {
                                return QString(R"({"status":"failed","code":"401","message":"更新pokemon失败"})");
                            }
                            else qDebug()<<QString(R"({"status":"success","code":"501","message":"信息更新成功"})");
                        }
                        qDebug() << "Found" << count << "pokemons for user with id" << userId;
                        qDebug() << "Pokemon with ID" << idToDelete << "successfully deleted.";
                        return QString(R"({"status":"success","code":"201","message":"精灵删除成功"})");
                    }
                } else {
                    qDebug() << "Error fetching query results: " << query.lastError().text();
                }
            } else {
                qDebug() << "Error executing query: " << query.lastError().text();
            }
        }
    }
    return QString(R"({"status":"error","code":"500","message":"Open database failed"})");
    db.close();
}
