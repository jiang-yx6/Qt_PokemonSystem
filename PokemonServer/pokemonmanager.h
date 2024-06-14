#ifndef POKEMONMANAGER_H
#define POKEMONMANAGER_H

#include <QString>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include"databasemanager.h"
class PokemonManager:public DatabaseManager
{
public:
    explicit PokemonManager(DatabaseManager *dbManager);
    QString storePokemon(const QString &username, const QJsonArray &pokemons);
    int getUserId(const QString &username);
    QString update(QJsonObject pokemonObject);
    QString addPokemon(QJsonObject pokemonObject);
    QString discardpokemon(int id);
    //void MyaddBindValue(QSqlQuery &query,int userid , QJsonObject pokemonObject);
    DatabaseManager *m_dbManager;
};

#endif // POKEMONMANAGER_H
