#include <QCoreApplication>
#include <QHttpServer> // Http服务器接口s
#include <QTcpServer>
#include <QSqlDatabase> // 用于访问SQLite数据库
#include <QSqlQuery> // 用于执行数据查询
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "databasemanager.h"
#include "usermanager.h"
#include "pokemonmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DatabaseManager *manager =new DatabaseManager;
    manager->initConnection();
    if(!manager->initializeDatabase())
    {
        qDebug() << "Failed to create the users table";
        return -1;
    }
    if(!manager->createPokemonTable())
    {
        qDebug() << "Failed to create the pokemons table";
        return -1;
    }

    UserManager* usrmanager = new UserManager(manager);
    PokemonManager* pokemanager =new PokemonManager(manager);
    QHttpServer server;
    server.afterRequest([](QHttpServerResponse&& resp){
        resp.setHeader("Content-Type","application/json;utf-8");
        return std::move(resp);
    });

    server.route("/api/login",[=](const QHttpServerRequest& request){
        QUrlQuery uquery = request.query();
        auto username = uquery.queryItemValue("username");
        auto password = uquery.queryItemValue("password");
        return usrmanager->loginUser(username,password);
    });

    server.route("/api/register",[=](const QHttpServerRequest& request){
        QUrlQuery uquery =request.query();
        auto username =uquery.queryItemValue("username");
        auto password = uquery.queryItemValue("password");
        // UserManager* usrmanager = new UserManager(manager);
        return usrmanager->registerUser(username,password);
    });


    server.route("/api/store_pokemon", [=](const QHttpServerRequest& request) {
        QByteArray jsonData = request.body();
        if (jsonData.isEmpty()) {
            qDebug() << "Received empty JSON data";
        }
        QJsonDocument jsonDoc;

        jsonDoc = QJsonDocument::fromJson(jsonData);

        QJsonObject pokemonObject = jsonDoc.object();
        //获取username
        QString username = pokemonObject.value("username").toString();
        qDebug() << "username: "<<username;

        QJsonArray pokemonJSONArray =  pokemonObject.value("pokemon").toArray();

        return pokemanager->storePokemon(username,pokemonJSONArray);
    });

    server.route("/api/checkpokemon",[=](const QHttpServerRequest& request){
        QByteArray recvData =request.body();
        if(recvData.isEmpty()){
            qDebug()<<"Received empty Userid";
        }

        QString idString =QString(recvData);
        int Userid = idString.toInt();

        return usrmanager->checkInfo(Userid);
    });

    server.route("/api/logout",[=](const QHttpServerRequest& request){
        QByteArray recvData =request.body();
        qDebug()<<recvData;
        if(recvData.isEmpty()){
            qDebug()<<"Received empty Userid";
        }
        QString idString =QString(recvData);
        int Userid = idString.toInt();
        return usrmanager->logOut(Userid);
    });

    server.route("/api/update_pokemon",[=](const QHttpServerRequest& request){
        QByteArray recvData =request.body();
        qDebug()<<recvData;
        if(recvData.isEmpty()){
            qDebug()<<"Received empty Update_pokemon";
        }
        QJsonDocument jsonDoc;

        jsonDoc = QJsonDocument::fromJson(recvData);

        QJsonObject pokemonObject = jsonDoc.object();
        return pokemanager->update(pokemonObject);
    });


    server.route("/api/add_pokemon",[=](const QHttpServerRequest& request){
        QByteArray recvData =request.body();
        if(recvData.isEmpty()){
            qDebug()<<"Received empty Update_pokemon";
        }
        QJsonDocument jsonDoc;
        qDebug()<<recvData;
        jsonDoc = QJsonDocument::fromJson(recvData);

        QJsonObject pokemonObject = jsonDoc.object();
        return pokemanager->addPokemon(pokemonObject);
    });

    server.route("/api/discard",[=](const QHttpServerRequest&request){
        QByteArray recvId = request.body();
        if(recvId.isEmpty()){
            qDebug()<<"Received empty discard id";
        }
        int id = recvId.toInt();
        qDebug()<<id;
        return pokemanager->discardpokemon(id);
    });

    if(server.listen(QHostAddress::Any, 80))
    {
        return a.exec();
    } else {
        qCritical("Failed to start server.");
        return 1;
    }
}
