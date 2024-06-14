#ifndef REGISTERSCENE_H
#define REGISTERSCENE_H

#include <QWidget>
#include <QNetworkAccessManager>
namespace Ui {
class RegisterScene;
}

class RegisterScene : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterScene(QWidget *parent = nullptr);
    ~RegisterScene();

private slots:
    void on_register2Btn_released();

private:
    Ui::RegisterScene *ui;
    QNetworkAccessManager * r_manager;
};

#endif // REGISTERSCENE_H
