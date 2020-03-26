#ifndef TYREFORCAR_H
#define TYREFORCAR_H

#include <QWidget>
#include "menu.h"

namespace Ui {
class tyreForCar;
}

class tyreForCar : public QWidget
{
    Q_OBJECT

public:
    explicit tyreForCar(QWidget *parent = 0);
    ~tyreForCar();

private slots:
    void connectManagerSetTyre();

    void replyFinishedSetTyre(QNetworkReply* reply);
    void replyDeleteFinished(QNetworkReply* reply);
    void replyFinishedSet(QNetworkReply* reply);

    void on_btnSet_clicked();

    void setTyreForCurrentCar();
    void deleteRequest();

    void on_tableTyres_clicked(const QModelIndex &index);

    void on_btnCancel_clicked();

private:
    Ui::tyreForCar *ui;
    QSortFilterProxyModel *proxyModel;
    QJsonTableModel *tableShowTyres;
    QNetworkAccessManager *manager;
    QJsonObject objectIndexOfSelectedTyre;
    QByteArray byteArrayIdOfTyreSelected;

    bool  isTyreRowSelected=false;

    QString name;
    QString type_size;
    QString mileage;
    QString date_of_setting;
};

#endif // TYREFORCAR_H
