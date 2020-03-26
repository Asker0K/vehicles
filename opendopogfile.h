#ifndef OPENDOPOGFILE_H
#define OPENDOPOGFILE_H

#include <QWidget>

#include "globals.h"
#include "menu.h"

namespace Ui {
class openDopogFile;
}

class openDopogFile : public QWidget
{
    Q_OBJECT

public:
    explicit openDopogFile(QWidget *parent = 0);
    ~openDopogFile();
    void connectManagerGet();

private slots:
    void on_btnChange_clicked();
    void on_btnCancel_clicked();
    void on_btnOK_clicked();
    void on_btnUploadDopog_clicked();

    void postData();
    void putData();

    void uploadDopogFile(QString extension, QString filePath);

    void replyLoadDopogFinished(QNetworkReply*reply);
    void replyGetFinished(QNetworkReply* reply);
    void replyPostDataFinished(QNetworkReply* reply);
    void replyPutDataFinished(QNetworkReply* reply);
private:
    Ui::openDopogFile *ui;
    QNetworkAccessManager *manager;
    int putOrPost;
    QString dopogId;
};

#endif // OPENDOPOGFILE_H
