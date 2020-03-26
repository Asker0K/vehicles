#include "adduser.h"
#include "ui_adduser.h"

addUser::addUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addUser)
{
    ui->setupUi(this);
    if(userRole == "Admin")
    {
        QModelIndex index =ui->comboRole->model()->index(0,0);
        QVariant v(0);
        ui->comboRole->model()->setData(index,v,Qt::UserRole - 1);
        ui->comboRole->setCurrentIndex(1);
    }
    else
    {
        ui->comboRole->setCurrentIndex(0);
    }
}

addUser::~addUser()
{
    delete ui;
}

void addUser::on_btnCancel_clicked()
{
    this->close();
}

void addUser::on_btnOk_clicked()
{
    postUser();
}
void addUser::postUser()
{
/********************************************************************/
    QString username=ui->lineName->text();
    QString password=ui->linePass->text();
    QString comboRole = ui->comboRole->currentText();
/********************************************************************/
    QJsonObject userAdd;
    userAdd["username"] = username;
    userAdd[ "password" ] = password ;
    userAdd[ "role" ] = comboRole;
/***************************post request*******************************/
        QJsonDocument doc(userAdd);
        QByteArray strJson(doc.toJson());

        manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
        QNetworkRequest request;
        request.setUrl(QUrl("http://localhost:7777/v1/users?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a"));
        request.setRawHeader("Content-Type", "application/json");
        QNetworkReply *reply = manager->post(request, strJson);
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(slotError(QNetworkReply::NetworkError)));
/********************************************************************/
}

void addUser::replyFinished(QNetworkReply *reply)
{
    QByteArray json = reply->readAll();
    QJsonDocument jsonDocument = QJsonDocument::fromJson( json );

     qDebug()<<"Reply:"<< json;

    QJsonObject json_obj=jsonDocument.object();
    QVariantMap json_map = json_obj.toVariantMap();
    if(reply->error())
    {
        QString errStatus = json_map["status"].toString();
        qDebug()<<"json_map:"<<errStatus ;
    }
    else
    {
        this->close();
        qDebug()<<"Reply: "<<  reply->readAll();
        reply->deleteLater();
    }
}


void addUser::slotError(QNetworkReply::NetworkError err)
{
    QMessageBox::warning(this,"Ошибка!","Данный пользователь уже существует!") ;
                         return;
}

