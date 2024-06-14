#include "discard.h"
#include "ui_discard.h"
#include <QTableWidget>
#include <QPushButton>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include"mainscene.h"
Discard::Discard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Discard)
    , d_manager(new QNetworkAccessManager(this))
{

    ui->setupUi(this);
    ui->discardTable->setColumnCount(8);
    ui->discardTable->setHorizontalHeaderLabels(QStringList()<<"精灵名字"<<"等级"<<"经验"<<"血量"<<"攻击力"<<"防御力"<<"闪避率"<<"ID");
    connect(ui->pushBtn,&QPushButton::clicked,this,[=](){
        int inputRow = ui->discardEdit->text().toInt();
        QString id  = ui->discardTable->item(inputRow-1,7)->text();
        QByteArray SendId = id.toUtf8();
        QUrl url("http://localhost/api/discard");
        QNetworkRequest request(url);
        QNetworkReply *reply = d_manager->post(request,SendId);
        connect(reply,&QNetworkReply::finished,this,[=](){
            QByteArray response = reply->readAll();
            QJsonParseError jsonError;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &jsonError);
            if (jsonError.error != QJsonParseError::NoError) {
                qDebug() << "JSON解析错误：" << jsonError.errorString();
                return;
            }
            QJsonObject jsonObj = jsonDoc.object();
            // 检查 "status" 和 "message" 字段
            if (jsonObj["status"].toString() == "success" && jsonObj["message"].toString() == "精灵删除成功") {
                qDebug() << "删除成功";
            } else if(jsonObj["status"].toString() == "success_but_no_pokemon"){
                qDebug()<<"删除成功但是该用户无宝可梦";
                std::vector<Pokemon*> pokemons = getRandomPokemons(1);
                // 序列化宝可梦为JSON
                QJsonObject addPokemon = pokemons[0]->toJsonObject();
                addPokemon["userid"]=MainScene::UserID;
                qDebug()<<addPokemon;
                QJsonDocument doc2(addPokemon);
                QByteArray jsonData2 = doc2.toJson();
                QUrl url2("http://localhost/api/add_pokemon");
                QNetworkRequest request2(url2);
                request2.setHeader(QNetworkRequest::ContentTypeHeader, "application/json"); // 设置Content-Type为application/json
                QNetworkReply *reply_s = d_manager->post(request2, jsonData2);
                connect(reply_s, &QNetworkReply::finished, this, [=]() {
                    if (reply_s->error()) {
                        qWarning() << "POST Error: " << reply_s->errorString();
                    }
                    else {
                        qDebug() << "POST Success: " << reply_s->readAll();
                    }
                });
            }
        });
        this->close();
    });

}

Discard::~Discard()
{
    delete ui;
}
