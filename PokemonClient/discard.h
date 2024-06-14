#ifndef DISCARD_H
#define DISCARD_H

#include <QWidget>
#include <QNetworkReply>

namespace Ui {
class Discard;
}

class Discard : public QWidget
{
    Q_OBJECT

public:
    explicit Discard(QWidget *parent = nullptr);
    ~Discard();
    QNetworkAccessManager * d_manager;
    Ui::Discard *ui;
};

#endif // DISCARD_H
