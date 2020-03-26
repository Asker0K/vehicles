#include "addgsm.h"
#include "ui_addgsm.h"

addGsm::addGsm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addGsm)
{
    ui->setupUi(this);

    ui->btnOk->setVisible(false);
}

addGsm::~addGsm()
{
    delete ui;
}

void addGsm::isBtnsOkOrSave()
{
    ui->btnSave->setVisible(false);
    ui->btnOk->setVisible(true);
}



void addGsm::on_btnOk_clicked()
{

    if(isMileageSelected==true)
        {
            putDataToDbMileage();
        }
    else if(isGsmSelected==true)
        {
            putDataToDbGsm();
         }


    this->close();
}

void addGsm::on_btnCancel_clicked()
{
    this->close();
}

void addGsm::on_btnSave_clicked()
{
    postDataToDB();
    this->close();
}

void addGsm::postDataToDB()
{
    /********************************************************************/
            QString date = ui->lineDate->text();
            QString track_number=ui->lineNumber->text();
            QString mileage_before_out=ui->lineStartMileage->text();
            QString on_city=ui->linePoGorodu->text();
            QString on_trace=ui->linePoTrasse->text();
            QString mileage_after_in=ui->lineProbegReturned->text();
            QString ostatok_gsm_before_out=ui->lineStartGsm->text();
            QString fuel_consumption_in_city=ui->linePoGoroduGsm->text();
            QString fuel_consumption_on_trace=ui->linePoTrasseGsm->text();
            QString given_gsm= ui->lineGsmGiven->text();
            QString ostatok_gsm_after_in=ui->lineGsmBack->text();
            QString normal_fuel_consumption_in_city=ui->linePoGoroduGsmNorm->text();
            QString normal_fuel_consumption_on_trace=ui->linePoTrasseGsmNorm->text();

    /***********************put from lines to json****************************/
            QJsonObject kartaGSM;
            QString selectedCarForGsm(jsonByteArrayId);
            kartaGSM["vehicle_id"]=selectedCarForGsm;
            kartaGSM["date"]=date;
            kartaGSM[ "track_number" ] = track_number ;
            kartaGSM[ "mileage_before_out" ] = mileage_before_out;
            kartaGSM[ "on_city" ] = on_city;
            kartaGSM[ "on_trace" ] = on_trace;
            kartaGSM[ "mileage_after_in" ] = mileage_after_in;
            kartaGSM[ "ostatok_gsm_before_out" ] = ostatok_gsm_before_out;
            kartaGSM[ "fuel_consumption_in_city" ] = fuel_consumption_in_city;
            kartaGSM[ "fuel_consumption_on_trace" ] = fuel_consumption_on_trace;
            kartaGSM[ "given_gsm" ] = given_gsm;
            kartaGSM[ "ostatok_gsm_after_in" ] = ostatok_gsm_after_in;
            kartaGSM[ "normal_fuel_consumption_in_city" ] =normal_fuel_consumption_in_city ;
            kartaGSM[ "normal_fuel_consumption_on_trace" ] = normal_fuel_consumption_on_trace ;
    /***************************post request*******************************/
            QJsonDocument doc( kartaGSM);
            QByteArray strJson(doc.toJson());

            QNetworkAccessManager* manager = new QNetworkAccessManager(this);
            connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
            QNetworkRequest request;
            request.setUrl(QUrl("http://localhost:7777/v1/mileagelists?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a"));
            request.setRawHeader("Content-Type", "application/json");
            QNetworkReply *reply = manager->post(request, strJson);
    /********************************************************************/
}

void addGsm::replyFinished(QNetworkReply * reply)
{
           qDebug()<<"Reply: "<<  reply->readAll();
           reply->deleteLater();
}




void addGsm::connectManagerGetMileageData()
{
    QString id(jsonByteArrayMileage);
    qDebug() << id;
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/mileagelists/own/"+id+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &addGsm::replygetDataFinished);
}

void addGsm::connectManagerGetGsmData()
{
    QString id(jsonByteArrayGsm);
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/mileagelists/own/"+id+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &addGsm::replygetDataFinished);
}




void addGsm::replygetDataFinished(QNetworkReply *reply)
{
    QByteArray json = reply->readAll();
    QJsonDocument jsonDocument = QJsonDocument::fromJson( json );
    qDebug() <<jsonDocument;
/*********************************************************/
    if(jsonDocument.isNull())
    {
        qDebug()<<"Failed to create JSON doc.";
    }
    if(!jsonDocument.isObject())
    {
        qDebug()<<"JSON is not an object.";
        exit(3);
    }
/***********fill the lines with data from json**********************/
    QJsonObject json_obj=jsonDocument.object();
    QVariantMap json_map = json_obj.toVariantMap();
    /*********************************************************/
    QString dateStr = json_map["date"].toString();
    QDate date =QDate::fromString((dateStr),"dd.MM.yyyy");
    ui->lineDate->setDate(date);
    ui->lineNumber->setText(json_map["track_number"].toString());
    ui->lineStartMileage->setText(json_map["mileage_before_out"].toString());
    ui->linePoGorodu->setText(json_map["on_city"].toString());
    ui->linePoTrasse->setText(json_map["on_trace"].toString());
     ui->lineProbegReturned->setText(json_map["mileage_after_in"].toString());
    ui->lineStartGsm->setText(json_map["ostatok_gsm_before_out"].toString());
    ui->linePoGoroduGsm->setText(json_map["fuel_consumption_in_city"].toString());
    ui->linePoTrasseGsm->setText(json_map["fuel_consumption_on_trace"].toString());
    ui->lineGsmGiven->setText(json_map["given_gsm"].toString());
    ui->lineGsmBack->setText(json_map["ostatok_gsm_after_in"].toString());
    ui->linePoGoroduGsmNorm->setText(json_map["normal_fuel_consumption_in_city"].toString());
    ui->linePoTrasseGsmNorm->setText(json_map["normal_fuel_consumption_on_trace"].toString());
    /*********************************************************/
    qDebug() <<reply->readAll();
    reply->deleteLater();
}





void addGsm::putDataToDbMileage()
{
/********************************************************************/
    QString date = ui->lineDate->text();
    QString track_number=ui->lineNumber->text();
    QString mileage_before_out=ui->lineStartMileage->text();
    QString on_city=ui->linePoGorodu->text();
    QString on_trace=ui->linePoTrasse->text();
    QString mileage_after_in=ui->lineProbegReturned->text();
    QString ostatok_gsm_before_out=ui->lineStartGsm->text();
    QString fuel_consumption_in_city=ui->linePoGoroduGsm->text();
    QString fuel_consumption_on_trace=ui->linePoTrasseGsm->text();
    QString given_gsm= ui->lineGsmGiven->text();
    QString ostatok_gsm_after_in=ui->lineGsmBack->text();
    QString normal_fuel_consumption_in_city=ui->linePoGoroduGsmNorm->text();
    QString normal_fuel_consumption_on_trace=ui->linePoTrasseGsmNorm->text();
    QString id(idMileage);
/********************************************************************/
    QJsonObject putMileagelists;
    putMileagelists["id"]=id;
    putMileagelists["date"]=date;
    putMileagelists[ "track_number" ] = track_number ;
    putMileagelists[ "mileage_before_out" ] = mileage_before_out;
    putMileagelists[ "on_city" ] = on_city;
    putMileagelists[ "on_trace" ] = on_trace;
    putMileagelists[ "mileage_after_in" ] = mileage_after_in;
    putMileagelists[ "ostatok_gsm_before_out" ] = ostatok_gsm_before_out;
    putMileagelists[ "fuel_consumption_in_city" ] = fuel_consumption_in_city;
    putMileagelists[ "fuel_consumption_on_trace" ] = fuel_consumption_on_trace;
    putMileagelists[ "given_gsm" ] = given_gsm;
    putMileagelists[ "ostatok_gsm_after_in" ] = ostatok_gsm_after_in;
    putMileagelists[ "normal_fuel_consumption_in_city" ] =normal_fuel_consumption_in_city ;
    putMileagelists[ "normal_fuel_consumption_on_trace" ] = normal_fuel_consumption_on_trace ;
/********************************************************************/
    QJsonDocument doc( putMileagelists);
    QByteArray strJson(doc.toJson());

    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &addGsm::replyFinishedPutMileagelists);
    QNetworkRequest request;

    request.setUrl(QUrl("http://localhost:7777/v1/mileagelists?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    QNetworkReply *reply = manager->put(request, strJson);
/********************************************************************/
}


void addGsm::putDataToDbGsm()
{
/********************************************************************/
    QString date = ui->lineDate->text();
    QString track_number=ui->lineNumber->text();
    QString mileage_before_out=ui->lineStartMileage->text();
    QString on_city=ui->linePoGorodu->text();
    QString on_trace=ui->linePoTrasse->text();
    QString mileage_after_in=ui->lineProbegReturned->text();
    QString ostatok_gsm_before_out=ui->lineStartGsm->text();
    QString fuel_consumption_in_city=ui->linePoGoroduGsm->text();
    QString fuel_consumption_on_trace=ui->linePoTrasseGsm->text();
    QString given_gsm= ui->lineGsmGiven->text();
    QString ostatok_gsm_after_in=ui->lineGsmBack->text();
    QString normal_fuel_consumption_in_city=ui->linePoGoroduGsmNorm->text();
    QString normal_fuel_consumption_on_trace=ui->linePoTrasseGsmNorm->text();
    QString id(idGsm);
/********************************************************************/
    QJsonObject putMileagelists;
    putMileagelists["id"]=id;
    putMileagelists["date"]=date;
    putMileagelists[ "track_number" ] = track_number ;
    putMileagelists[ "mileage_before_out" ] = mileage_before_out;
    putMileagelists[ "on_city" ] = on_city;
    putMileagelists[ "on_trace" ] = on_trace;
    putMileagelists[ "mileage_after_in" ] = mileage_after_in;
    putMileagelists[ "ostatok_gsm_before_out" ] = ostatok_gsm_before_out;
    putMileagelists[ "fuel_consumption_in_city" ] = fuel_consumption_in_city;
    putMileagelists[ "fuel_consumption_on_trace" ] = fuel_consumption_on_trace;
    putMileagelists[ "given_gsm" ] = given_gsm;
    putMileagelists[ "ostatok_gsm_after_in" ] = ostatok_gsm_after_in;
    putMileagelists[ "normal_fuel_consumption_in_city" ] =normal_fuel_consumption_in_city ;
    putMileagelists[ "normal_fuel_consumption_on_trace" ] = normal_fuel_consumption_on_trace ;
/********************************************************************/
    QJsonDocument doc( putMileagelists);
    QByteArray strJson(doc.toJson());

    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &addGsm::replyFinishedPutMileagelists);
    QNetworkRequest request;

    request.setUrl(QUrl("http://localhost:7777/v1/mileagelists?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    QNetworkReply *reply = manager->put(request, strJson);
/********************************************************************/
}


void addGsm::replyFinishedPutMileagelists(QNetworkReply * reply)
{
     qDebug()<<"Reply: "<<  reply->readAll();
     reply->deleteLater();
}
