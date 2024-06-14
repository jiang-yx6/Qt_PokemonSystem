#include "infoform.h"
#include "ui_infoform.h"
#include "mainscene.h"
InfoForm::InfoForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InfoForm)
    ,check_manager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget_3->setColumnCount(7);
    ui->tableWidget_2->setColumnCount(2);
    ui->tableWidget_2->setColumnWidth(0, 160);
    ui->tableWidget_2->setColumnWidth(1, 160);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"精灵名字"<<"等级"<<"经验"<<"血量"<<"攻击力"<<"防御力"<<"闪避率");

    ui->tableWidget_3->setHorizontalHeaderLabels(QStringList()<<"精灵名字"<<"等级"<<"经验"<<"血量"<<"攻击力"<<"防御力"<<"闪避率");
    ui->tableWidget_2->setHorizontalHeaderLabels(QStringList()<<"用户名称"<<"用户id");
}

void InfoForm::closeEvent(QCloseEvent *event)
{
    emit back();
}

InfoForm::~InfoForm()
{
    delete ui;
}


void InfoForm::on_checkBtn_clicked()
{
    QUrl url("http://localhost/api/checkpokemon");
    QNetworkRequest request(url);
    QByteArray SendData = QString::number(MainScene::UserID).toUtf8();
    qDebug()<<MainScene::UserID;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json"); // 设置Content-Type为application/json
    qDebug()<<SendData;
    QNetworkReply *reply_c = check_manager->post(request, SendData);

    connect(reply_c, &QNetworkReply::finished, this, [=]() {
        //处理返回信息
        QByteArray response = reply_c->readAll();
        qDebug()<<response;
        QJsonParseError jsonError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &jsonError);
        if (jsonError.error != QJsonParseError::NoError) {
            qDebug() << "JSON解析错误：" << jsonError.errorString();
            return;
        }
        QJsonObject AllInfo = jsonDoc.object();
        QJsonArray PokemonArray = AllInfo["pokemon"].toArray();
        QJsonArray UserArray = AllInfo["truestatus"].toArray();
        ui->tableWidget->clearContents(); // 清空表格内容
        ui->tableWidget->setRowCount(PokemonArray.size()); // 设置行数
        int row = 0;
        for (const QJsonValue &value : PokemonArray) {
            if (value.isObject()) {
                const QJsonObject &pokemonObj = value.toObject();
                ui->tableWidget->setItem(row, 0, new QTableWidgetItem(pokemonObj["name"].toString()));
                ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(pokemonObj["level"].toInt())));
                ui->tableWidget->setItem(row, 2, new QTableWidgetItem(pokemonObj["exp"].toString()));
                ui->tableWidget->setItem(row, 3, new QTableWidgetItem(pokemonObj["healthPoints"].toString()));
                ui->tableWidget->setItem(row, 4, new QTableWidgetItem(pokemonObj["attackPower"].toString()));
                ui->tableWidget->setItem(row, 5, new QTableWidgetItem(pokemonObj["defensePower"].toString()));
                ui->tableWidget->setItem(row, 6, new QTableWidgetItem(pokemonObj["speedValue"].toString()));
                // ...（依次设置其他列）
                row++;
            }
        }

        ui->tableWidget_2->clearContents(); // 清空表格内容
        ui->tableWidget_2->setRowCount(UserArray.size()); // 设置行数
        row = 0;
        for (const QJsonValue &value : UserArray) {
            if (value.isObject()) {
                const QJsonObject &userObj = value.toObject();
                qDebug()<<"check user: "<<userObj;
                ui->tableWidget_2->setItem(row, 0, new QTableWidgetItem(userObj["username"].toString()));
                ui->tableWidget_2->setItem(row, 1, new QTableWidgetItem(userObj["userid"].toString()));
                // ...（依次设置其他列）
                row++;
            }
        }
        qDebug()<<AllInfo["winrate"];
        ui->username2->setText(AllInfo["username"].toString());
        ui->password2->setText(AllInfo["password"].toString());
        ui->status2->setText(AllInfo["status"].toString());
        ui->count2->setText(QString::number(PokemonArray.count()));
        ui->badge2->setText(AllInfo["countbadge"].toString()+'/'+AllInfo["superbadge"].toString());
        ui->winrate2->setText(QString::number(AllInfo["winrate"].toDouble()));
    });

}


void InfoForm::on_userBtn_clicked()
{
    int userid = ui->useridInput->text().toInt();
    QUrl url("http://localhost/api/checkpokemon");
    QNetworkRequest request(url);
    QByteArray SendData = QString::number(userid).toUtf8();
    qDebug()<<MainScene::UserID;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json"); // 设置Content-Type为application/json
    qDebug()<<SendData;
    QNetworkReply *reply_c = check_manager->post(request, SendData);
    connect(reply_c, &QNetworkReply::finished, this, [=]() {
        //处理返回信息
        QByteArray response = reply_c->readAll();
        qDebug()<<response;
        QJsonParseError jsonError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &jsonError);
        if (jsonError.error != QJsonParseError::NoError) {
            qDebug() << "JSON解析错误：" << jsonError.errorString();
            return;
        }
        QJsonObject AllInfo = jsonDoc.object();
        QJsonArray PokemonArray = AllInfo["pokemon"].toArray();
        ui->tableWidget_3->clearContents(); // 清空表格内容
        ui->tableWidget_3->setRowCount(PokemonArray.size()); // 设置行数
        int row = 0;
        for (const QJsonValue &value : PokemonArray) {
            if (value.isObject()) {
                const QJsonObject &pokemonObj = value.toObject();
                ui->tableWidget_3->setItem(row, 0, new QTableWidgetItem(pokemonObj["name"].toString()));
                ui->tableWidget_3->setItem(row, 1, new QTableWidgetItem(QString::number(pokemonObj["level"].toInt())));
                ui->tableWidget_3->setItem(row, 2, new QTableWidgetItem(pokemonObj["exp"].toString()));
                ui->tableWidget_3->setItem(row, 3, new QTableWidgetItem(pokemonObj["healthPoints"].toString()));
                ui->tableWidget_3->setItem(row, 4, new QTableWidgetItem(pokemonObj["attackPower"].toString()));
                ui->tableWidget_3->setItem(row, 5, new QTableWidgetItem(pokemonObj["defensePower"].toString()));
                ui->tableWidget_3->setItem(row, 6, new QTableWidgetItem(pokemonObj["speedValue"].toString()));
                // ...（依次设置其他列）
                row++;
            }
        }
    });

}

