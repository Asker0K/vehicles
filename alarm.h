#ifndef ALARM_H
#define ALARM_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QRegularExpression>
#include <QVector>
#include <QString>
#include <QDebug>

#include "globals.h"
#include "cars_list.h"

class alarm : public QObject
{
    Q_OBJECT
public:
    explicit alarm(QObject *parent = 0);

signals:

public slots:
    void connectManagerGetVehicles();
    void connectManagerGetTO();
    void connectManagerGetTyres();
    void alarmLevelsMileageTyre();
    void alarmLevelsToGrm();
    void connectManagerGetCertificateDopog();
    void connectManagerHandbook();
private slots:
    //remember data func's
    void rememberVehicleData(QString jsonDoc, QString key1, QString key2);
    void rememberTyreData(QString jsonDoc, QString key1, QString key2);
    void rememberDopogData(QString jsonDoc);
    void rememberHandbookData(QString jsonDoc);
    void rememberToData(QString jsonDoc);
    //main func's
    void appendMatrix();
    void checkPeriodOfValidityDopog();
    void compareTO();
    void compareGrm();
    //reply func's
    void replyGetTOFinished(QNetworkReply* reply);
    void replyGetVehiclesFinished(QNetworkReply* reply);
    void replyGetTyresFinished(QNetworkReply* reply);
    void replyGetCertificateDopogFinished(QNetworkReply* reply);
    void replyGetHandbookFinished(QNetworkReply* reply);

private:
    //pointer
    QNetworkAccessManager* manager;
    //tyre arrays
    QVector <QString> vehicle_id;
    QVector <QString> tyre_vehicle_id;
    QVector <QString> tyre_id;
    QVector <QString> tyreId;
    //vehicle array
    QVector <QString> vehicle_mileage;
    QVector <QString> tyre_mileage;
    //diff between mileages vehicle and tyre arrays
    QVector <QString> idDiff;
    QVector <int> mileageDiff;
    //dopog arrays
    QVector <QString> idDopog;
    QVector <QString> periodDopog;
    //to arrays
    QVector <QString> idTo_s;
    QVector <QString> mileage_for_next_to;
    //handbook arrays
    QVector <QString> idHandbook;
    QVector <QString> period_between_to;
    QVector <QString> period_between_grm;
    //diff between period_between_to and mileage_for_next_to
    QVector <QString> to_sDiffId;
    QVector <int> to_sDiffMileage;
    //diff between period_between_grm_service and mileage_for_next_to
    QVector <int> grmDiffMileage;
};

#endif // ALARM_H
