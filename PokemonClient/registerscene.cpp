#include "registerscene.h"
#include "ui_registerscene.h"
#include <QNetworkReply>

#include <QDialog>
#include <QUrlQuery>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "pokemon.h"
RegisterScene::RegisterScene(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegisterScene)
    ,r_manager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
}

RegisterScene::~RegisterScene()
{
    delete ui;
}

void RegisterScene::on_register2Btn_released()
{
    //获取usrname & password
    auto username = ui->regUsername->text();
    auto password = ui->regPassword->text();
    auto confirm = ui ->regConfirm->text();
    //用户名不能为空
    //ui->tip2label->show();
    qDebug()<<"register";
    if(username.isEmpty())
    {
        qWarning()<<"USERNAME EMPTY";
        ui->tip2label->setText("用户名不能为空");
        return;
    }
    //密码不能为空
    if(password.isEmpty())
    {
        qWarning()<<"PASSWORD EMPTY";
        ui->tip2label->setText("密码不能为空");
        return;
    }
    if(confirm.isEmpty())
    {
        ui->tip2label->setText("确认密码不能为空");
        return;
    }
    if(confirm.compare(password)!=0)
    {
        ui->tip2label->setText("确认密码与密码不一致");
        return;
    }

    QUrlQuery query;
    query.addQueryItem("username", username); // 假设用户名是'bupt01'
    query.addQueryItem("password", password);
    QUrl url("http://localhost/api/register");
    url.setQuery(query);
    //网络验证
    QNetworkReply * reply = r_manager->get(QNetworkRequest(url));
    //关联信号
    connect(reply,&QNetworkReply::finished,[=](){
        if (reply->error()) {
            qWarning() << "Network error: " << reply->errorString();
            ui->tip2label->setText("网络错误: " + reply->errorString());
        }
        else {
            if (reply->readAll().compare("{\"status\":\"failed\",\"code\":\"409\",\"message\":\"用户已存在!\"}", Qt::CaseSensitive) == 0) {
               ui->tip2label->setText("用户已存在");
                qDebug() << "POST Success: " << reply->readAll();
            }
            else
            {
                qDebug() << "POST Success: " << reply->readAll();
                ui->tip2label->setText("注册成功"); // 如果没有错误，假设注册成功
                qInfo() << QString::fromUtf8(reply->readAll());
                qInfo()<<"1: store...";
                std::vector<Pokemon*> pokemons = getRandomPokemons(3);
                // 序列化宝可梦为JSON
                QJsonArray pokemonJSONArray;
                for (const Pokemon* pokemon : pokemons) {
                    QJsonObject pokemonObject = pokemon->toJsonObject();
                    pokemonJSONArray.append(pokemonObject);
                }
                QJsonObject pokemonObject;
                pokemonObject["pokemon"] =pokemonJSONArray;
                pokemonObject["username"]=username;
                // 创建JSON字符串
                QJsonDocument doc(pokemonObject);
                QByteArray jsonData = doc.toJson();
                //qDebug()<<"2: "<<jsonData;
                // 发送POST请求到服务端
                QUrl url("http://localhost/api/store_pokemon");
                QNetworkRequest request(url);
                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json"); // 设置Content-Type为application/json
                QNetworkReply *reply_s = r_manager->post(request, jsonData);
                connect(reply_s, &QNetworkReply::finished, this, [=]() {
                    if (reply_s->error()) {
                        qWarning() << "POST Error: " << reply_s->errorString();
                        // 可以在这里添加更详细的错误处理，比如显示错误信息给用户
                        ui->tip2label->setText("保存数据失败: " + reply_s->errorString());
                    }
                    else {
                        ui->tip2label->setText("保存数据成功");
                        qDebug() << "POST Success: " << reply_s->readAll();
                    }
                });
                this->close();
            }
        }
    });
}
