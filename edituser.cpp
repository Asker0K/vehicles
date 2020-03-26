#include "edituser.h"
#include "ui_edituser.h"

editUser::editUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::editUser)
{
    ui->setupUi(this);
    connectManagerUser();
    index =ui->comboRole->model()->index(0,0);
    QVariant v(0);
    ui->comboRole->model()->setData(index,v,Qt::UserRole - 1);

}

editUser::~editUser()
{
    delete ui;
}


void editUser::connectManagerUser()
{
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/users?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &editUser::replyFinished);
}

void editUser::replyFinished()
{
        /*************convert QJsonObject into QJsonDocument*******************/
            QJsonDocument doc(userJsonObject);
            QByteArray jsonByteArray(doc.toJson(QJsonDocument::Compact)) ;
            qDebug() <<"success!" <<jsonByteArray;
            QJsonDocument docObj = QJsonDocument::fromJson(jsonByteArray);
    /************************************/
            if(docObj.isNull())
            {
                qDebug() <<"create!";

            }
            if(!docObj.isObject())
            {
                qDebug()<<"JSON is not an object.";
                exit(3);
            }
    /***************get Json data*************************/
            QJsonObject json_obj=docObj.object();
            QVariantMap json_map = json_obj.toVariantMap();

            qDebug()<< json_map["id"].toString();
            qDebug()<< json_map["username"].toString();
            qDebug()<< json_map["password"].toString();
            qDebug()<< json_map["role"].toString();
    /********************************************************************/
            ui->lineName->setText(json_map["username"].toString());
            ui->linePass->setText(json_map["password"].toString());
            QVariant role =json_map["role"];
           if(role=="super_admin")
           {
               ui->comboRole->setCurrentIndex(0);
               ui->comboRole->setEnabled(false);
               if(userRole == "Admin")
               {
                  ui->comboRole->setEnabled(false);
                  ui->lineName->setEnabled(false);
                  ui->linePass->setEnabled(false);
               }
           }
           else if(role == "Admin")
           {
               ui->comboRole->setCurrentIndex(1);
               if(userRole == "Admin")
               {
                  ui->comboRole->setEnabled(false);
                  ui->lineName->setEnabled(false);
                  ui->linePass->setEnabled(false);
               }
           }
           else if(role== "User")
           {
               ui->comboRole->setCurrentIndex(2);
               if(userRole == "Admin")
               {
                   QModelIndex index =ui->comboRole->model()->index(1,0);
                   QVariant v(0);
                   ui->comboRole->model()->setData(index,v,Qt::UserRole - 1);
               }
           }

    /********************************************************************/
}

void editUser::putDataToDB()
{
/********************************************************************/
    QString username=ui->lineName->text();
    QString password=ui->linePass->text();
    QString idToPutData (userId);
    QString comboRole = ui->comboRole->currentText();
/********************************************************************/
    QJsonObject userChange;
    userChange["id"]=idToPutData ;
    userChange["username"] = username;
    userChange[ "password" ] = password ;
    userChange[ "role" ] = comboRole;
/********************************************************************/
    QJsonDocument doc( userChange);
    QByteArray strJson(doc.toJson());

    qDebug() << strJson;

    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &editUser::putFinished);
    QNetworkRequest request;

    request.setUrl(QUrl("http://localhost:7777/v1/users"
                                "?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    QNetworkReply *reply = manager->put(request, strJson);

/********************************************************************/
}

void editUser::putFinished(QNetworkReply*reply)
{
    qDebug()<<"Reply: "<<  reply->readAll();
    reply->deleteLater();
}

void editUser::on_btnOk_clicked()
{
    putDataToDB();
    this->close();
}

void editUser::on_btnCancel_clicked()
{
    this->close();
}
