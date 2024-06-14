#ifndef CHOOSEENEMY_H
#define CHOOSEENEMY_H

#include <QWidget>
#include <QNetworkAccessManager>
#include"fight.h"

namespace Ui {
class ChooseEnemy;
}

class ChooseEnemy : public QWidget
{
    Q_OBJECT

public:
    explicit ChooseEnemy(QWidget *parent = nullptr);
    ~ChooseEnemy();
    Fight* fightscene;
signals:
    void back();
public slots:
    void setLabelText(const QString& text); // 添加这个槽函数声明
protected:
    void closeEvent(QCloseEvent *event) override;
private slots:
    void on_confirmBtn_clicked();
public:
    Ui::ChooseEnemy *ui;

};

#endif // CHOOSEENEMY_H
