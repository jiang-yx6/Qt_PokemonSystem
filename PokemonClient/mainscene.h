#ifndef MAINSCENE_H
#define MAINSCENE_H
#include <QWidget>
#include <QNetworkAccessManager> //网络访问头文件
#include "registerscene.h"
#include "startgame.h"
#include "userrequest.h"
QT_BEGIN_NAMESPACE

namespace Ui {
class MainScene;
}
QT_END_NAMESPACE

class MainScene : public QWidget
{
    Q_OBJECT
    friend class InfoForm;
public:
    MainScene(QWidget *parent = nullptr);
    ~MainScene();
    RegisterScene * w1;
    StartGame* w2;
    static int UserID;
private slots:
    void on_loginBtn_released();

    void on_registerBtn_released();
signals:
    void loginSuccess();
private:
    Ui::MainScene *ui;
    QNetworkAccessManager * m_manager;
};

#endif // MAINSCENE_H
