#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QPixmap>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariantMap>

#include "cars_list.h"
#include "cars_list.h"
#include "menu.h"
#include "globals.h"
#include "alarm.h"
//#include "user.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    char *getUser();
    void putUser(char *a);

private slots:
    void closeEvent(QCloseEvent *event);

    void on_btnAuth_clicked();
    void cascadStyleSheet();
    void imgLogoAuth();
    void replyFinished(QNetworkReply *);
    void slotReadyRead();
    void slotError(QNetworkReply::NetworkError err);
private:
    Ui::MainWindow *ui;
    QString *errString;



};

#endif // MAINWINDOW_H
