#include "settyre.h"
#include "ui_settyre.h"

setTyre::setTyre(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setTyre)
{
    ui->setupUi(this);
    if(isPutTyre==true)
    {
        connectManagerGetData();
    }
}

setTyre::~setTyre()
{
    delete ui;
}


void setTyre::postTyre()
{
    /********************************************************************/
            QString tyreSize=ui->lineTyreSize->text();
            QString tyreName = ui->lineName->text();
            QString dateOfSetting=ui->dateOfSetting->text();
            QString mileage = ui->lineMileage->text();
    /***********************put from lines to json****************************/
            QJsonObject setTyre;
            //QString stringIndexSelectedCarRezina(jsonByteArrayRezinaId);
            //setTyre["vehicle_id"]=stringIndexSelectedCarRezina;
            setTyre[ "type_size" ] = tyreSize;
            setTyre[ "name" ] = tyreName;
            setTyre[ "date_of_setting" ] = dateOfSetting;
            setTyre[ "mileage" ] = mileage;
    /***************************post request*******************************/
            QJsonDocument doc( setTyre);
            QByteArray strJson(doc.toJson());

            manager = new QNetworkAccessManager(this);
            connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
            QNetworkRequest request;
            request.setUrl(QUrl("http://localhost:7777/v1/tyre?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a"));
            request.setRawHeader("Content-Type", "application/json");
            QNetworkReply *reply = manager->post(request, strJson);
    /********************************************************************/
}

void setTyre::replyFinished(QNetworkReply * reply)
{
           qDebug()<<"Reply: "<<  reply->readAll();
           reply->deleteLater();
}

void setTyre::on_btnOk_clicked()
{
    if(isPutTyre ==false)
        postTyre();
    else if(isPutTyre == true)
        putTyre();
    this->close();
}

void setTyre::on_btnCancel_clicked()
{
    this->close();
}

void setTyre::connectManagerGetData()
{
    QString id(jsonIdSklad);
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/tyre/own/"+id+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &setTyre::replyGetDataFinished);
}

void setTyre::replyGetDataFinished(QNetworkReply* reply)
{
    QByteArray json = reply->readAll();
    QJsonDocument docObj = QJsonDocument::fromJson(json);
/*********************************************************/
    if(docObj.isNull())
    {
        qDebug()<<"Failed to create JSON doc.";
        isRowSelected = false;
    }
    if(!docObj.isObject())
    {
        qDebug()<<"JSON is not an object.";
        exit(3);
    }
/*********************************************************/
/***********fill the lines with data from json**********************/
    QJsonObject json_obj=docObj.object();
    QVariantMap json_map = json_obj.toVariantMap();
/*********************************************************/
    ui->lineTyreSize->setText(json_map["type_size"].toString());
    ui->lineName->setText(json_map["name"].toString());
    QString dateStr = json_map["date_of_setting"].toString();
    QDate date =QDate::fromString((dateStr),"dd.MM.yyyy");
    ui->dateOfSetting->setDate(date);
    ui->lineMileage->setText(json_map["mileage"].toString());
/*********************************************************/

}




void setTyre::putTyre()
{
        /********************************************************************/
            QString type_size=ui->lineTyreSize->text();
            QString name=ui->lineName->text();
            QString date_of_setting=ui->dateOfSetting->text();
            QString mileage=ui->lineMileage->text();
            QString id(jsonIdSklad);
        /***********************put from lines to json****************************/
            QJsonObject Tyre;
            Tyre["id"]=id;
            Tyre["type_size"]=type_size;
            Tyre[ "name" ] = name ;
            Tyre[ "date_of_setting" ] = date_of_setting ;
            Tyre[ "mileage" ] = mileage ;
        /********************************************************************/
            QJsonDocument doc( Tyre);
            QByteArray strJson(doc.toJson());

            QNetworkAccessManager* manager = new QNetworkAccessManager(this);
            connect(manager, &QNetworkAccessManager::finished, this, &setTyre::replyPutDataFinished);
            QNetworkRequest request;

            request.setUrl(QUrl("http://localhost:7777/v1/tyre?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a"));
            request.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
            QNetworkReply *reply = manager->put(request, strJson);
        /********************************************************************/
}


void setTyre::replyPutDataFinished(QNetworkReply* reply)
{
    qDebug() <<"replyPutData=" << reply->readAll();
    reply->deleteLater();
}
