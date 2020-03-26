#include "opendopogfile.h"
#include "ui_opendopogfile.h"

openDopogFile::openDopogFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::openDopogFile)
{
    ui->setupUi(this);
        ui->btnCancel->setVisible(false);
        ui->btnOK->setVisible(false);
        ui->lineDate_of_validity->setReadOnly(true);
        ui->lineGos_number->setReadOnly(true);
    if(userRole=="User")
    {
        ui->btnChange->setDisabled(true);
        ui->btnUploadDopog->setDisabled(true);
    }
}

openDopogFile::~openDopogFile()
{
    delete ui;
}

void openDopogFile::on_btnChange_clicked()
{
    ui->lineDate_of_validity->setReadOnly(false);
    ui->lineDate_of_validity->setFocus();
    ui->btnCancel->setVisible(true);
    ui->btnOK->setVisible(true);
}

void openDopogFile::on_btnCancel_clicked()
{
    ui->btnOK->setVisible(false);
    ui->btnCancel->setVisible(false);
    ui->lineDate_of_validity->setReadOnly(true);
    ui->lineGos_number->setReadOnly(true);
}

void openDopogFile::on_btnOK_clicked()
{
    ui->lineDate_of_validity->setReadOnly(true);
    ui->lineGos_number->setReadOnly(true);
    ui->btnChange->setVisible(true);
    ui->btnOK->setVisible(false);
    ui->btnCancel->setVisible(false);
    if(putOrPost==1)
    {
        putData();
    }
    else if(putOrPost==2)
    {
        postData();
        putOrPost= 1;
    }
    this->close();
}

void openDopogFile::on_btnUploadDopog_clicked()
{
    QString extensionReversed;
    QString filePath=QFileDialog::getOpenFileName(this,QString::fromUtf8("Открыть файл"),QDir::currentPath());
    for(int i = filePath.length()-1;filePath[i]!='.';i--)
    {
        extensionReversed.append(filePath[i]);
    }
    extensionReversed.append('.');
    QString extension;
    for(int i =extensionReversed.length()-1;i>=0;i--)
    {
        extension.append(extensionReversed[i]);
    }
    qDebug() << "path:=" <<filePath;
    qDebug() << "extensionReversed:="<<extensionReversed;
    qDebug() << "extension:="<<extension;
    uploadDopogFile(extension,filePath);
}


void openDopogFile::uploadDopogFile(QString extension,QString filePath)
{
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart extensionPart;
    extensionPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"extension\""));
    extensionPart.setBody(extension.toLatin1());

    QHttpPart fileBodyPart;
    fileBodyPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\""+extension+"\""));
    QFile *file = new QFile(filePath);
    if (!file->open(QIODevice::ReadOnly))
    {
                qDebug() << "QFile Error: File not found!";
                return ;
    }
    else
    {
        qDebug() << "File found, proceed as planned";
     }
    file->close();
    file->open(QFile::ReadOnly);
    fileBodyPart.setBodyDevice(file);
    file->setParent(multiPart);

    multiPart->append(extensionPart);
    multiPart->append(fileBodyPart);

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyLoadDopogFinished(QNetworkReply*)));
    QNetworkRequest request;
    request.setUrl(QUrl("http://localhost:7777/v1/certificates/fileupload/"+dopogIdIs+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a"));
    qDebug() << "dopogid"<<dopogIdIs;
    QNetworkReply* reply = manager->post(request, multiPart);
    multiPart->setParent(reply);
}

void openDopogFile::replyLoadDopogFinished(QNetworkReply*reply)
{
    qDebug()<<"ReplyDOPOG: "<<reply->readAll();
    reply->deleteLater();
}

void openDopogFile::connectManagerGet()
{
    QString id(jsonByteArrayId);
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/certificates/veh/"+id+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &openDopogFile::replyGetFinished);
}

void openDopogFile::replyGetFinished(QNetworkReply* reply)
{
    QByteArray json = reply->readAll();
    if(reply->error())
    {
        qDebug() << "reply::error" << json;
        putOrPost= 2;
        ui->lineGos_number->setText(gosNumberTC);
        return;
    }
    QJsonDocument jsonDocument = QJsonDocument::fromJson( json );

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
    dopogIdIs=json_map["id"].toString();
    QString dateStr = json_map["period_of_validity"].toString();
    QDate date =QDate::fromString((dateStr),"dd.MM.yyyy");
    ui->lineDate_of_validity->setDate(date);
    ui->lineGos_number->setText(json_map["gos_number"].toString());
    fileNameDopog = json_map["file_name"].toString();
    /*********************************************************/
    putOrPost= 1;
    reply->deleteLater();
}

void openDopogFile::postData()
{
    /********************************************************************/
            QString period_of_validity=ui->lineDate_of_validity->text();
            QString gos_number=ui->lineGos_number->text();
            QString id(jsonByteArrayId);
    /***********************put from lines to json****************************/
            QJsonObject dopog;
            dopog["vehicle_id"] = id;
            dopog["period_of_validity"] = period_of_validity;
            dopog["gos_number"] = gos_number;
    /***************************post request*******************************/
            QJsonDocument doc( dopog);
            QByteArray strJson(doc.toJson());

            QNetworkAccessManager* manager = new QNetworkAccessManager(this);
            connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyPostDataFinished(QNetworkReply*)));
            QNetworkRequest request;
            request.setUrl(QUrl("http://localhost:7777/v1/certificates?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a"));
            request.setRawHeader("Content-Type", "application/json");
            manager->post(request, strJson);
    /********************************************************************/
}

void openDopogFile::replyPostDataFinished(QNetworkReply* reply)
{
    QByteArray json = reply->readAll();
    qDebug() <<"postDataFinishedDopog="<<json;
    reply->deleteLater();
}

void openDopogFile::putData()
{
    /********************************************************************/
        QString period_of_validity=ui->lineDate_of_validity->text();
        QString gos_number=ui->lineGos_number->text();
    /********************************************************************/
        QJsonObject dopog;
        dopog["id"] = dopogIdIs;
        dopog["period_of_validity"]=period_of_validity;
        dopog["gos_number"]=gos_number;
    /********************************************************************/
        QJsonDocument doc(dopog);
        QByteArray strJson(doc.toJson());

        QNetworkAccessManager* manager = new QNetworkAccessManager(this);
        connect(manager, &QNetworkAccessManager::finished, this, &openDopogFile::replyPutDataFinished);
        QNetworkRequest request;

        request.setUrl(QUrl("http://localhost:7777/v1/certificates?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
        manager->put(request, strJson);
}

void openDopogFile::replyPutDataFinished(QNetworkReply* reply)
{
    qDebug() <<"putDataFinishedDopog="<<reply->readAll();
    reply->deleteLater();
}
