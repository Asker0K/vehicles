#ifndef USERINFO_H
#define USERINFO_H

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
#include "QSortFilterProxyModel"
#include <QMessageBox>
#include "qjsontable.h"
#include "adduser.h"
#include "edituser.h"
#include "globals.h"

namespace Ui {
class userinfo;
}

class userinfo : public QWidget
{
    Q_OBJECT

public:
    explicit userinfo(QWidget *parent = 0);
    ~userinfo();
    void connectManagerUser();

private slots:
    void on_addUserBtn_clicked();
    void on_delUserBtn_clicked();

    void replyFinished(QNetworkReply* );
    void deleteRequest();
    void replyDeleteFinished(QNetworkReply* reply);

    void on_usersList_clicked(const QModelIndex &index);
    void on_btnRefresh_clicked();
    void on_btnChangeInfo_clicked();

    void findAdmin();

private:
    Ui::userinfo *ui;
    QNetworkAccessManager *manager;
    QJsonTableModel* tableUserInfo;
    bool isUserSelected = false;
    QSortFilterProxyModel *proxyModel;
};

#endif // USERINFO_H
