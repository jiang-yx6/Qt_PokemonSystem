#include"mainscene.h"
#include "startgame.h"
#include "ui_startgame.h"
#include "ui_chooseenemy.h"
StartGame::StartGame(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StartGame)
    , manager_net(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    this->widget =new InfoForm;
    this->widget2 =new ChooseEnemy;
    connect(this->widget,&InfoForm::back,this,[=](){
        this->show();
        widget->close();
    });
    connect(this->widget2,&ChooseEnemy::back,this,[=](){
        this->show();
        widget2->close();
    });
    setWindowFlags(Qt::CustomizeWindowHint);
}

StartGame::~StartGame()
{
    delete ui;
}

void StartGame::on_logoutBtn_clicked()
{
    QUrl url("http://localhost/api/logout");
    QNetworkRequest request(url);
    QByteArray SendData = QString::number(MainScene::UserID).toUtf8();
    qDebug()<<MainScene::UserID;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json"); // 设置Content-Type为application/json
    qDebug()<<SendData;
    QNetworkReply *reply_o = manager_net->post(request, SendData);
    connect(reply_o, &QNetworkReply::finished, this, [=]() {
        QByteArray response = reply_o->readAll();
        QJsonParseError jsonError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &jsonError);
        if (jsonError.error != QJsonParseError::NoError) {
            qDebug() << "JSON解析错误：" << jsonError.errorString();
            return;
        }
        QJsonObject jsonObj = jsonDoc.object();
        // 检查 "status" 和 "message" 字段
        if (jsonObj["status"].toString() == "success" && jsonObj["message"].toString() == "登出成功!") {
            qDebug() << "登出成功";
        } else {
            qDebug() << "登出失败或响应格式不正确";
        }
    });
    emit back();
}


void StartGame::on_selfinfoBtn_clicked()
{
    widget->show();
    this->hide();
}


void StartGame::on_upgradeBtn_clicked()
{
    widget2->show();
    widget2->setLabelText("升级赛");
    loadmypoke();
    this->hide();
}


void StartGame::on_juedouBtn_clicked()
{
    widget2->show();
    widget2->setLabelText("决斗赛");
    loadmypoke();
    this->hide();
}


void StartGame::loadmypoke()
{
    QUrl url("http://localhost/api/checkpokemon");
    QNetworkRequest request(url);
    QByteArray SendData = QString::number(MainScene::UserID).toUtf8();
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json"); // 设置Content-Type为application/json
    QNetworkReply *reply_c = manager_net->post(request, SendData);
    connect(reply_c, &QNetworkReply::finished, this, [=]() {
        //处理返回信息
        QByteArray response = reply_c->readAll();
        QJsonParseError jsonError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &jsonError);
        if (jsonError.error != QJsonParseError::NoError) {
            qDebug() << "JSON解析错误：" << jsonError.errorString();
            return;
        }
        QJsonObject AllInfo = jsonDoc.object();
        QJsonArray PokemonArray = AllInfo["pokemon"].toArray();
        widget2->ui->myTab->clearContents(); // 清空表格内容
        widget2->ui->myTab->setRowCount(PokemonArray.size()); // 设置行数
        int row = 0;
        for (const QJsonValue &value : PokemonArray) {
            if (value.isObject()) {
                const QJsonObject &pokemonObj = value.toObject();
                widget2->ui->myTab->setItem(row, 0, new QTableWidgetItem(pokemonObj["name"].toString()));
                widget2->ui->myTab->setItem(row, 1, new QTableWidgetItem(QString::number(pokemonObj["level"].toInt())));
                widget2->ui->myTab->setItem(row, 2, new QTableWidgetItem(pokemonObj["exp"].toString()));
                widget2->ui->myTab->setItem(row, 3, new QTableWidgetItem(pokemonObj["healthPoints"].toString()));
                widget2->ui->myTab->setItem(row, 4, new QTableWidgetItem(pokemonObj["attackPower"].toString()));
                widget2->ui->myTab->setItem(row, 5, new QTableWidgetItem(pokemonObj["defensePower"].toString()));
                widget2->ui->myTab->setItem(row, 6, new QTableWidgetItem(pokemonObj["speedValue"].toString()));
                widget2->ui->myTab->setItem(row, 7, new QTableWidgetItem(QString::number(pokemonObj["id"].toInt())));
                row++;
            }
        }

    });
}
