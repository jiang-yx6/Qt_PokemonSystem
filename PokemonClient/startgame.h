#ifndef STARTGAME_H
#define STARTGAME_H
#include "infoform.h"
#include "chooseenemy.h"
//#include "mainscene.h"
#include <QWidget>
#include <QNetworkAccessManager>
namespace Ui {
class StartGame;
}

class StartGame : public QWidget
{
    Q_OBJECT

public:
    explicit StartGame(QWidget *parent = nullptr);
    InfoForm* widget;
    ChooseEnemy * widget2;
    ~StartGame();

signals:
    void back();

private slots:
    void on_logoutBtn_clicked();

    void on_selfinfoBtn_clicked();

    void on_upgradeBtn_clicked();

    void on_juedouBtn_clicked();

    void loadmypoke();
private:
    Ui::StartGame *ui;
    QNetworkAccessManager * manager_net;
};

#endif // STARTGAME_H
