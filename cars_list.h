#ifndef CARS_LIST_H
#define CARS_LIST_H

#include <QDialog>
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
#include <QMouseEvent>
#include <QColor>
#include <QSortFilterProxyModel>

#include "qjsontable.h"
#include "menu.h"
#include "userinfo.h"



namespace Ui {
class cars_list;
}

class cars_list : public QWidget
{
    Q_OBJECT

public:
    explicit cars_list(QWidget *parent = 0);
    ~cars_list();

    void tyreAlarm();
private slots:
    void closeEvent(QCloseEvent *event);

    void on_btnOpen_clicked();
    void on_btnSearch_clicked();
    void on_btnAdd_clicked();
    void on_btnDelete_clicked();

    void setDisabledcarsList(bool isEnabled);

    void connectManagerVehicle();
    void replyFinished(QNetworkReply* );

    void deleteRequest();
    void replyDeleteFinished(QNetworkReply*);
    void on_db_cars_list_clicked(const QModelIndex &index);


    void mousePressEvent(QMouseEvent *event);

    void on_btnRefresh_clicked();
    void on_search_query_textChanged(const QString &arg1);
    void on_search_query_editingFinished();

    void on_btnSettings_clicked();

    void isUserAdmin();


    void clearDataJson();

    void filterModel();

private:
    Ui::cars_list *ui;
    void imgIndicator(QString path);
    void imgSearch();
    void imgBtnSettings();
    QNetworkAccessManager *manager;
    QSortFilterProxyModel *proxyModel;

};

#endif // CARS_LIST_H
