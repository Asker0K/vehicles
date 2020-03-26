#ifndef ADDGSM_H
#define ADDGSM_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QUrl>
#include "menu.h"
#include "cars_list.h"

namespace Ui {
class addGsm;
}

class addGsm : public QWidget
{
    Q_OBJECT

public:
    explicit addGsm(QWidget *parent = 0);
    ~addGsm();
    void connectManagerGetMileageData();
    void connectManagerGetGsmData();

    void isBtnsOkOrSave();

private slots:
    void on_btnCancel_clicked();
    void on_btnSave_clicked();

    void postDataToDB();

    void putDataToDbGsm();
    void putDataToDbMileage();

    void replyFinished(QNetworkReply * reply);
    void replygetDataFinished(QNetworkReply *reply);
    void replyFinishedPutMileagelists(QNetworkReply * reply);

    void on_btnOk_clicked();

private:
    Ui::addGsm *ui;
    QNetworkAccessManager *manager;
    QString id;
    QByteArray idMileage= jsonByteArrayMileage;
    QByteArray idGsm= jsonByteArrayGsm;

};

#endif // ADDGSM_H
