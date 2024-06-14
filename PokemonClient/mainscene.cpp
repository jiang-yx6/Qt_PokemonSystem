#include "mainscene.h"
#include "ui_mainscene.h"
#include <QNetworkReply>
#include <QDialog>
#include <QSqlQuery>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
int MainScene::UserID = 0;
MainScene::MainScene(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainScene)
    ,m_manager(new QNetworkAccessManager(this))
{

    ui->setupUi(this);
    this->w1 = new RegisterScene;
    this->w2 = new StartGame;
    connect(this->w2,&StartGame::back,this,[=](){
        this->show();
        w2->close();
    });
}

MainScene::~MainScene()
{
    delete ui;
}

void MainScene::on_loginBtn_released()
{
    //获取usrname & password
    auto username = ui->usernameEdit->text();
    auto password = ui->passwordEdit->text();
    //用户名不能为空
    if(username.isEmpty())
    {
        ui->tiplabel->setText("用户名不能为空");
        return;
    }
    //密码不能为空
    if(password.isEmpty())
    {
        ui->tiplabel->setText("密码不能为空");
        return;
    }
    QUrlQuery query;
    query.addQueryItem("username", username); // 假设用户名是'bupt01'
    query.addQueryItem("password", password);
    QUrl url("http://localhost/api/login");
    url.setQuery(query);

    //网络验证
    QNetworkReply * reply = m_manager->get(QNetworkRequest(url));
    //关联信号

    connect(reply,&QNetworkReply::finished,[=](){
        QByteArray response = reply->readAll();
        QJsonParseError jsonError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &jsonError);
        if (jsonError.error != QJsonParseError::NoError) {
            qDebug() << "JSON解析错误：" << jsonError.errorString();
            return;
        }
        QJsonObject jsonObj = jsonDoc.object();
        // 检查 "status" 和 "message" 字段
        if (jsonObj["status"].toString() == "success" && jsonObj["message"].toString() == "登陆成功!") {
            qDebug() << "登录成功";
            MainScene::UserID = jsonObj["userid"].toInt();
            this->hide();
            w2->show();
        }
        else if(jsonObj["message"].toString() == "用户已登录"){
           ui->tiplabel->setText("用户已登录");
        }
        else
        {
            qDebug() << "登录失败或响应格式不正确";
            ui->tiplabel->setText("登录失败，账号或密码不正确！");
        }
    });
}

void MainScene::on_registerBtn_released()
{
    w1->show();
}




