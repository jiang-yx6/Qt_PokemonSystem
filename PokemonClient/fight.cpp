#include "fight.h"
#include "ui_fight.h"
#include"mainscene.h"
#include "ui_discard.h"
#include <chrono>
Fight::Fight(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Fight)
    , u_manager(new QNetworkAccessManager(this))
{
    this->discardscene = new Discard;
    ui->setupUi(this);
}

void Fight::closeEvent(QCloseEvent *event)
{
    emit back();
}

Fight::~Fight()
{
    delete ui;
}

void Fight::on_start_clicked()
{
    loadingPokemon(mypokemon,syspokemon);

    if(mypokemon!=NULL&&syspokemon!=NULL)
    {
        myHp = mypokemon->healthPoints;
        sysHp = syspokemon->healthPoints;
        round = 1;//初始化回合数为1
        ui->listWidget->clear();
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        srand(seed);
        while(sysHp>0&&myHp>0)
        {

            int randnumber1 = rand()%100+1;
            int randnumber2 = rand()%100+1;
            int randHit1 = rand()%100+1;
            int randHit2 = rand()%100+1;
            qDebug()<<"rand1:"<<randnumber1<<"rand2:"<<randnumber2;
            battleRound(mypokemon, syspokemon, myHp, sysHp,round,randnumber1,randnumber2,randHit1,randHit2);//进入对战逻辑
            round++;
        }
        qDebug()<<"我方血量："<<myHp;
        qDebug()<<"敌方血量："<<sysHp;

        if(myHp>0&&sysHp<=0)//赢得胜利
        {
            //更新对战精灵属性信息
            mypokemon->exp+=100;
            mypokemon->levelUp();
            QJsonObject pokemonObject;
            pokemonObject["id"]=Pokemon_my["ID"].toString().toInt();
            pokemonObject["attackPower"]=mypokemon->attackPower;
            pokemonObject["defensePower"]=mypokemon->defensePower;
            pokemonObject["healthPoints"]=mypokemon->healthPoints;
            pokemonObject["speedValue"]=mypokemon->speedValue;
            pokemonObject["level"]=mypokemon->level;
            pokemonObject["exp"]=mypokemon->exp;
            pokemonObject["userid"]=MainScene::UserID;
            // 创建JSON字符串
            QJsonDocument doc(pokemonObject);
            QByteArray jsonData = doc.toJson();
            QUrl url("http://localhost/api/update_pokemon");
            QNetworkRequest request(url);
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json"); // 设置Content-Type为application/json
            QNetworkReply *reply_u = u_manager->post(request, jsonData);
            connect(reply_u, &QNetworkReply::finished, this, [=]() {
                if (reply_u->error()) {
                    qWarning() << "POST Error: " << reply_u->errorString();
                }
                else {
                    qDebug() << "POST Success: " << reply_u->readAll();
                }
            });

            //决斗赛才能获得精灵
            if(battleType.compare("决斗赛")){
                QJsonObject addPokemon;
                addPokemon["userid"]=MainScene::UserID;
                addPokemon["name"]=syspokemon->name;
                addPokemon["attackPower"]=syspokemon->attackPower;
                addPokemon["defensePower"]=syspokemon->defensePower;
                addPokemon["healthPoints"]=syspokemon->healthPoints;
                addPokemon["speedValue"]=syspokemon->speedValue;
                addPokemon["level"]=syspokemon->level;
                addPokemon["exp"]=syspokemon->exp;
                QJsonDocument doc2(addPokemon);
                QByteArray jsonData2 = doc2.toJson();
                QUrl url2("http://localhost/api/add_pokemon");
                QNetworkRequest request2(url2);
                request2.setHeader(QNetworkRequest::ContentTypeHeader, "application/json"); // 设置Content-Type为application/json
                QNetworkReply *reply_s = u_manager->post(request2, jsonData2);
                connect(reply_s, &QNetworkReply::finished, this, [=]() {
                    if (reply_s->error()) {
                        qWarning() << "POST Error: " << reply_s->errorString();
                    }
                    else {
                        qDebug() << "POST Success: " << reply_s->readAll();
                    }
                });
            }
        }
        else//如果失败啦
        {
            if(battleType.compare("决斗赛"))
            {
                //获取所有宝可梦信息，以便后续选择丢弃精灵
                QUrl url("http://localhost/api/checkpokemon");
                QNetworkRequest request(url);
                QByteArray SendData = QString::number(MainScene::UserID).toUtf8();
                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json"); // 设置Content-Type为application/json
                QNetworkReply *reply_c = u_manager->post(request, SendData);
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
                    discardscene->ui->discardTable->clearContents(); // 清空表格内容
                    discardscene->ui->discardTable->setRowCount(PokemonArray.size()); // 设置行数
                    int row = 0;
                    for (const QJsonValue &value : PokemonArray) {
                        if (value.isObject()) {
                            const QJsonObject &pokemonObj = value.toObject();
                            discardscene->ui->discardTable->setItem(row, 0, new QTableWidgetItem(pokemonObj["name"].toString()));
                            discardscene->ui->discardTable->setItem(row, 1, new QTableWidgetItem(QString::number(pokemonObj["level"].toInt())));
                            discardscene->ui->discardTable->setItem(row, 2, new QTableWidgetItem(pokemonObj["exp"].toString()));
                            discardscene->ui->discardTable->setItem(row, 3, new QTableWidgetItem(pokemonObj["healthPoints"].toString()));
                            discardscene->ui->discardTable->setItem(row, 4, new QTableWidgetItem(pokemonObj["attackPower"].toString()));
                            discardscene->ui->discardTable->setItem(row, 5, new QTableWidgetItem(pokemonObj["defensePower"].toString()));
                            discardscene->ui->discardTable->setItem(row, 6, new QTableWidgetItem(pokemonObj["speedValue"].toString()));
                            discardscene->ui->discardTable->setItem(row, 7, new QTableWidgetItem(QString::number(pokemonObj["id"].toInt())));
                            row++;
                        }
                    }


                    int num1,num2,num3;
                    srand(time(0));
                    QString labelText;
                    if(PokemonArray.size()>3)
                    {
                        num1 = rand()%PokemonArray.size()+1;
                        num2 = num1;
                        while(num2 == num1)
                        {
                            num2 = rand()%PokemonArray.size()+1;
                        }
                        num3 = num2;
                        while(num3==num2 || num3==num1)
                        {
                            num3 = rand()%PokemonArray.size()+1;
                        }
                        labelText = QString("请在序号%1,%2,%3中选择一个").arg(num1).arg(num2).arg(num3);
                    }
                    else
                        labelText = QString("请选择一个序号");
                    discardscene->ui->label->setText(labelText);
                });
                discardscene->show();
            }
        }
    }
}


void Fight::battleRound(Pokemon* mypokemon, Pokemon* syspokemon, int &myHp, int &sysHp,int &round,int randnumber1,int randnumber2,int randHit1,int randHit2)
{
    if (myHp <= 0 || sysHp <= 0)
        return;

    ui->listWidget->addItem("Round " + QString::number(round));
    qDebug() << "Round" << round;
    srand(time(0));

    int damage1 = mypokemon->attack(syspokemon,randnumber1);
    int CriHit1 = 0;
    if(mypokemon->level*2.5<randHit1)
    {
        CriHit1 = mypokemon->attackPower/10;
    }
    sysHp -= (damage1+CriHit1);
    if (sysHp <= 0)
    {
        if(CriHit1==0)
            ui->listWidget->addItem("我方对敌方造成" + QString::number(damage1) + "的"+mypokemon->attackWay+"伤害,敌方剩余血量：" + QString::number(sysHp));
        else
            ui->listWidget->addItem("我方对敌方造成" + QString::number(damage1+CriHit1) + "的"+mypokemon->attackWay+"暴击伤害,敌方剩余血量：" + QString::number(sysHp));
        ui->listWidget->addItem(syspokemon->name + "已阵亡,恭喜胜利");
        return;
    }
    if (damage1 == 0)
        ui->listWidget->addItem("miss 未命中");
    else
        ui->listWidget->addItem("我方对敌方造成" + QString::number(damage1) + "的"+mypokemon->attackWay+"伤害,敌方剩余血量：" + QString::number(sysHp));


    int damage2 = syspokemon->attack(mypokemon,randnumber2);
    int CriHit2 = 0;
    if(syspokemon->level*2.5<randHit1)
    {
        CriHit2 = syspokemon->attackPower/10;
    }
    myHp -= (damage2+CriHit2);
    if (myHp <= 0)
    {
        if(CriHit2 == 0)
            ui->listWidget->addItem("敌方对我方造成" + QString::number(damage2) + "的"+syspokemon->attackWay+"伤害,我方剩余血量：" + QString::number(myHp));
        else
            ui->listWidget->addItem("敌方对我方造成" + QString::number(damage2+CriHit2) + "的"+syspokemon->attackWay+"暴击伤害,我方剩余血量：" + QString::number(myHp));
        ui->listWidget->addItem(mypokemon->name + "已阵亡,挑战失败");
        return;
    }
    if (damage2 == 0)
        ui->listWidget->addItem("miss 未命中");
    else
        ui->listWidget->addItem("敌方对我方造成" + QString::number(damage2) + "的"+syspokemon->attackWay+"伤害,我方剩余血量：" + QString::number(myHp));

    ui->listWidget->addItem("loading ....");
}


void Fight::loadingPokemon(Pokemon* &mypokemon,Pokemon* &syspokemon)
{
    qDebug()<<"start";
    static Pokemon* pokemonList[] = {
        new XiaoHuoLong(),
        new JieNiGui(),
        new PiKaQiu(),
        new MiaoWaZhongZi(),
        new KaBiShou(),
        new GangQiu(),
        new DaYanShe(),
        new JiLiDan(),
        new KeDaYa(),
        new ShanDianNiao(),
        new KuaiLong(),
        new XiaoDaLa()
    };
    QString name[]={"小火龙","杰尼龟","皮卡丘","妙蛙种子","卡比兽","钢球","大岩蛇","吉利蛋","可达鸭","闪电鸟","快龙","小达拉"};
    int index;
    for( index =0 ;index<12;index++)
    {
        if(Pokemon_my["精灵名字"].toString().compare(name[index])==0)
        {
            break;
        }
    }
    int index2;
    for( index2 =0 ;index2<12;index2++)
    {
        if(Pokemon_sys["精灵名字"].toString().compare(name[index2])==0)
        {
            break;
        }
    }
    mypokemon =pokemonList[index];
    syspokemon = pokemonList[index2];
    mypokemon->name = Pokemon_my["精灵名字"].toString();
    mypokemon->level= Pokemon_my["等级"].toString().toInt();
    mypokemon->exp = Pokemon_my["经验"].toString().toInt();
    mypokemon->attackPower = Pokemon_my["攻击力"].toString().toInt();
    mypokemon->defensePower = Pokemon_my["防御力"].toString().toInt();
    mypokemon->healthPoints= Pokemon_my["血量"].toString().toInt();
    mypokemon->speedValue = Pokemon_my["闪避率"].toString().toDouble();
    mypokemon->attackway();

    syspokemon->name = Pokemon_sys["精灵名字"].toString();
    syspokemon->level= Pokemon_sys["等级"].toString().toInt();
    syspokemon->exp = 0;
    syspokemon->attackPower = Pokemon_sys["攻击力"].toString().toInt();
    syspokemon->defensePower = Pokemon_sys["防御力"].toString().toInt();
    syspokemon->healthPoints=Pokemon_sys["血量"].toString().toInt();
    syspokemon->speedValue = Pokemon_sys["闪避率"].toString().toDouble();
    syspokemon->attackway();

    qDebug()<<mypokemon->name<<syspokemon->name;
}


