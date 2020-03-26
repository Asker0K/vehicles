#ifndef EDITUSER_H
#define EDITUSER_H

#include <QWidget>
#include "userinfo.h"
#include"globals.h"

namespace Ui {
class editUser;
}

class editUser : public QWidget
{
    Q_OBJECT

public:
    explicit editUser(QWidget *parent = 0);
    ~editUser();
    void connectManagerUser();

private slots:
    void replyFinished();
    void putDataToDB();
    void putFinished(QNetworkReply *reply);
    void on_btnOk_clicked();

    void on_btnCancel_clicked();

private:
    Ui::editUser *ui;
    QNetworkAccessManager *manager;
    QModelIndex index;
    QVariant v;
};

#endif // EDITUSER_H
