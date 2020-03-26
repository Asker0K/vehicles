#ifndef ZAKAZNARYAD_H
#define ZAKAZNARYAD_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include "cars_list.h"

namespace Ui {
class zakaznaryad;
}

class zakaznaryad : public QWidget
{
    Q_OBJECT

public:
    explicit zakaznaryad(QWidget *parent = 0);
    ~zakaznaryad();

    void disableButtons();
    void isUserAdmin();
    void disableDownload();
    void connectManagerGetOrder();

private slots:
    void loadZakazNaryad(QString extension, QString filePath);

    void on_btnReturnToOrderForRepair_clicked();
    void on_btnOk_clicked();
    void on_btnChange_clicked();
    void on_btnLoad_clicked();
    void on_btnDwnld_clicked();


    void disableLines(bool isEnabled);
    void postOrderForRepair();

    void replyFinished(QNetworkReply * reply);
    void replyGetDataFromOrderFinished(QNetworkReply* reply);
    void replyLoadFinished(QNetworkReply* reply);
    void onDownloadStsFinished(QNetworkReply *reply);
    void replyPutOrderFinished(QNetworkReply* reply);

    void putOrderForRepair();
private:
    Ui::zakaznaryad *ui;
    QNetworkAccessManager* manager;
    QString fileNameZakaznaryad;
    QString pathToZakaznaryadFile;
};

#endif // ZAKAZNARYAD_H
