#ifndef FIGHT_H
#define FIGHT_H

#include <QWidget>
#include <QJsonObject>
#include <QThread>
#include <QTimer>
#include <QNetworkReply>
#include <QTableWidget>
#include"pokemon.h"
#include "discard.h"
namespace Ui {
class Fight;
}

class Fight : public QWidget
{
    Q_OBJECT

public:
    explicit Fight(QWidget *parent = nullptr);
    void loadingPokemon(Pokemon* &mypokemon,Pokemon* &syspokemon);
    void battleRound(Pokemon* mypokemon, Pokemon* syspokemon, int &myHp, int &sysHp, int &round,int randnumber1,int randnumber2,int randHit1,int randHit2);
    // void onTimeout();
    // void stopTimer();
    void closeEvent(QCloseEvent *event);
    ~Fight();
    QJsonObject Pokemon_my;
    QJsonObject Pokemon_sys;
    Ui::Fight *ui;
    QTimer*timer;
    int myHp;
    int sysHp;
    int round;
    QString battleType;
    Pokemon* mypokemon=NULL;
    Pokemon* syspokemon=NULL;
    QNetworkAccessManager * u_manager;
    Discard* discardscene;
signals:
    void back();
private slots:
    void on_start_clicked();
};

#endif // FIGHT_H
