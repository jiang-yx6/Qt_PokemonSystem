#ifndef INFOFORM_H
#define INFOFORM_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
namespace Ui {
class InfoForm;
}

class InfoForm : public QWidget
{
    Q_OBJECT

public:
    explicit InfoForm(QWidget *parent = nullptr);
    ~InfoForm();
    QNetworkAccessManager * check_manager;
signals:
    void back();
private slots:
    void on_checkBtn_clicked();
    void on_userBtn_clicked();

protected:
    void closeEvent(QCloseEvent *event) override;
private:
    Ui::InfoForm *ui;
};

#endif // INFOFORM_H
