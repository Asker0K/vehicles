#ifndef ADDUSER_H
#define ADDUSER_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QUrl>
#include <QStandardItemModel>
#include <QMessageBox>
#include "userinfo.h"
#include "globals.h"

namespace Ui {
class addUser;
}

class addUser : public QWidget
{
    Q_OBJECT

public:
    explicit addUser(QWidget *parent = 0);
    ~addUser();

private slots:
    void on_btnCancel_clicked();
    void on_btnOk_clicked();
    void postUser();
    void replyFinished(QNetworkReply *reply);

    void slotError(QNetworkReply::NetworkError err);

private:
    Ui::addUser *ui;
    QNetworkAccessManager *manager;
};

#endif // ADDUSER_H
