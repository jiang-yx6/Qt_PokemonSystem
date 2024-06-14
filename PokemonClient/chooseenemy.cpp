#include "chooseenemy.h"
#include "ui_chooseenemy.h"
#include "ui_fight.h"
// #include "mainscene.h"
// #include"startgame.h"
ChooseEnemy::ChooseEnemy(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChooseEnemy)
{
    fightscene = new Fight;
    connect(this->fightscene,&Fight::back,this,[=](){
        fightscene->close();
        emit back();
    });
    ui->setupUi(this);
    ui->systemTab->clearContents(); // 清空表格内容
    ui->systemTab->setRowCount(48); // 设置行数
    ui->systemTab->setColumnCount(7);
    ui->systemTab->setHorizontalHeaderLabels(QStringList()<<"精灵名字"<<"等级"<<"类型"<<"血量"<<"攻击力"<<"防御力"<<"闪避率");
    ui->myTab->setColumnCount(8);
    ui->myTab->setHorizontalHeaderLabels(QStringList()<<"精灵名字"<<"等级"<<"经验"<<"血量"<<"攻击力"<<"防御力"<<"闪避率"<<"ID");
    QString name[]={"小火龙","皮卡丘","杰尼龟","妙蛙种子","卡比兽","钢球","大岩蛇","吉利蛋","可达鸭","闪电鸟","快龙","小达拉"};
    int row =0;
    for (int i=0;i<12;i++) {
        int attackPower=8;
        int healthPoints=45;
        int defensePower=4;
        float speedValue=0.07;
        int level =1;

        for(int j =0;j<4;j++)
        {
            if(j!=0)
                level=j*5;
            QString type;
            if(i>=0&&i<=2)
            {
                type="攻击型";
                for(int i=1;i<level;i++)
                {
                    attackPower+=i*4;
                    healthPoints+=i*7;
                    defensePower+=i*3;
                    speedValue+=0.02;
                }
            }
            else if(i>=3&&i<=5)
            {
                type="防御型";
                for(int i=1;i<level;i++)
                {
                    attackPower+=i*3;
                    healthPoints+=i*9;
                    defensePower+=i*5;
                    speedValue+=0.012;
                }
            }
            else if(i>=6&&i<=8)
            {
                type="肉盾型";
                for(int i=1;i<level;i++)
                {
                    attackPower+=i*3;
                    healthPoints+=i*11;
                    defensePower+=i*4;
                    speedValue+=0.01;
                }
            }
            else if(i>=9&&i<=11)
            {
                type="敏捷型";
                for(int i=1;i<level;i++)
                {
                    attackPower+=i*4;
                    healthPoints+=i*6;
                    defensePower+=i*3;
                    speedValue+=0.024;
                }
            }
            ui->systemTab->setItem(row, 0, new QTableWidgetItem(name[i]));
            ui->systemTab->setItem(row, 1, new QTableWidgetItem(QString::number(level)));
            ui->systemTab->setItem(row, 2, new QTableWidgetItem(type));
            ui->systemTab->setItem(row, 3, new QTableWidgetItem(QString::number(healthPoints)));
            ui->systemTab->setItem(row, 4, new QTableWidgetItem(QString::number(attackPower)));
            ui->systemTab->setItem(row, 5, new QTableWidgetItem(QString::number(defensePower)));
            ui->systemTab->setItem(row, 6, new QTableWidgetItem(QString::number(speedValue)));
            row++;
        }
    }

}

void ChooseEnemy::setLabelText(const QString& text)
{
    ui->label_2->setText(text);
}

void ChooseEnemy::closeEvent(QCloseEvent *event) {
    emit back();
}

ChooseEnemy::~ChooseEnemy()
{
    delete ui;
}

void ChooseEnemy::on_confirmBtn_clicked()
{
    if(ui->label_2->text().compare("升级赛"))
        fightscene->battleType = "升级赛";
    else
        fightscene->battleType = "决斗赛";

    int inputSysRow = ui->sysEdit->text().toInt()-1;
    int inputUserRow = ui->userEdit->text().toInt()-1;

    if(inputSysRow >= 0 && inputSysRow < ui->systemTab->rowCount()&&inputUserRow >= 0 && inputUserRow < ui->myTab->rowCount()){ // 检查行号是否有效
        for(int column=0;column<ui->systemTab->columnCount();column++)
        {
            QTableWidgetItem *item = ui->systemTab->item(inputSysRow, column);
            fightscene->Pokemon_sys[ui->systemTab->horizontalHeaderItem(column)->text()] = item->text();
        }
        for(int column=0;column<ui->myTab->columnCount();column++)
        {
            QTableWidgetItem *item = ui->myTab->item(inputUserRow, column);
            fightscene->Pokemon_my[ui->myTab->horizontalHeaderItem(column)->text()] = item->text();
        }
        fightscene->show();
        fightscene->ui->myname->setText(fightscene->Pokemon_my["精灵名字"].toString());
        fightscene->ui->sysname->setText(fightscene->Pokemon_sys["精灵名字"].toString());
        qDebug()<<fightscene->Pokemon_sys;
        qDebug()<<fightscene->Pokemon_my;
        this->hide();
    }
    else {
        qDebug() << "Invalid row number";
        ui->warning->setText("输入有误，请重新输入");
    }

}

