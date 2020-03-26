#ifndef SETTYRE_H
#define SETTYRE_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include "menu.h"

namespace Ui {
class setTyre;
}

class setTyre : public QDialog
{
    Q_OBJECT

public:
    explicit setTyre(QWidget *parent = 0);
    ~setTyre();

private slots:
    void postTyre();
    void connectManagerGetData();
    void putTyre();

    void on_btnOk_clicked();
    void on_btnCancel_clicked();

    void replyFinished(QNetworkReply * reply);
    void replyGetDataFinished(QNetworkReply* reply);
    void replyPutDataFinished(QNetworkReply* reply);

private:
    Ui::setTyre *ui;
    QNetworkAccessManager* manager;
};

#endif // SETTYRE_H
