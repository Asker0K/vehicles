#include "zakaznaryad.h"
#include "ui_zakaznaryad.h"

zakaznaryad::zakaznaryad(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::zakaznaryad)
{
    ui->setupUi(this);
    disableLines(true);

    ui->btnOk->setVisible(false);
    ui->btnReturnToOrderForRepair->setVisible(false);
}

zakaznaryad::~zakaznaryad()
{
    delete ui;
}

void zakaznaryad::isUserAdmin()
{
        if(userRole=="User")
        {
            ui->btnChange->setVisible(false);
            ui->btnLoad->setDisabled(true);
        }
        else return;
}


void zakaznaryad::disableLines(bool isEnabled)
{
    ui->lineBrand->setReadOnly(isEnabled);
    ui->lineDate->setReadOnly(isEnabled);
    ui->lineGosNumber->setReadOnly(isEnabled);
    ui->lineNameContragent->setReadOnly(isEnabled);
    ui->lineNumber->setReadOnly(isEnabled);
    ui->lineSum->setReadOnly(isEnabled);
}

void zakaznaryad::on_btnReturnToOrderForRepair_clicked()
{
    ui->btnOk->setVisible(false);
    ui->btnReturnToOrderForRepair->setVisible(false);
    ui->btnChange->setVisible(true);
}

void zakaznaryad::on_btnOk_clicked()
{
    ui->btnChange->setVisible(true);
    ui->btnReturnToOrderForRepair->setVisible(false);
    ui->btnOk->setVisible(false);
    if(isPut==false)
         postOrderForRepair();
    if(isPut==true)
        putOrderForRepair();

}

void zakaznaryad::on_btnChange_clicked()
{
    ui->btnChange->setVisible(false);
    ui->btnOk->setVisible(true);
    ui->btnReturnToOrderForRepair->setVisible(true);
    disableLines(false);
}

void zakaznaryad::connectManagerGetOrder()
{
    QString id(jsonByteArrayOrder);
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/ordersforrepair/own/"+id+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &zakaznaryad::replyGetDataFromOrderFinished);
}

void zakaznaryad::replyGetDataFromOrderFinished(QNetworkReply* reply)
{
    /*************convert QJsonObject into QJsonDocument*******************/
    QByteArray json = reply->readAll();
    QJsonDocument jsonDocument = QJsonDocument::fromJson( json );
    qDebug() << "json="<<json;
/************************************/
        if(jsonDocument.isNull())
        {
            qDebug() <<"create!";
        }
        if(!jsonDocument.isObject())
        {
            qDebug()<<"JSON is not an object.";
            exit(3);
        }
/***************get Json data*************************/
        QJsonObject json_obj=jsonDocument.object();
        QVariantMap json_map = json_obj.toVariantMap();
        QString dateStr = json_map["date_of_conclusion"].toString();
        QDate date =QDate::fromString((dateStr),"dd.MM.yyyy");
/********************************************************************/
        ui->lineNameContragent->setText(json_map["contractor"].toString());
        ui->lineNumber->setText(json_map["order_number"].toString());
        ui->lineSum->setText(json_map["sum"].toString());
        ui->lineGosNumber->setText(json_map["gos_number"].toString());
        ui->lineBrand->setText(json_map["brand"].toString());
        ui->lineDate->setDate(date);
        fileNameZakaznaryad = json_map["filename"].toString();

/********************************************************************/
}

void zakaznaryad::postOrderForRepair()
{
    /********************************************************************/
            QString id(jsonByteArrayId);
            QString contractor = ui->lineNameContragent->text();
            QString orderNumber = ui->lineNumber->text();
            QString sum = ui->lineSum->text();
            QString gosNumber = ui->lineGosNumber->text();
            QString marka = ui->lineBrand->text();
            QString date = ui->lineDate->text();
    /***********************put from lines to json****************************/
            QJsonObject orderForRepair;
            orderForRepair[ "vehicle_id" ]=id;
            orderForRepair["order_number"]=orderNumber;
            orderForRepair[ "contractor" ] = contractor ;
            orderForRepair[ "sum" ] = sum;
            orderForRepair[ "gos_number" ] = gosNumber;
            orderForRepair[ "brand" ] = marka;
            orderForRepair[ "date_of_conclusion" ] =date;
    /***************************post request*******************************/
            QJsonDocument doc( orderForRepair );
            QByteArray strJson(doc.toJson());

            manager = new QNetworkAccessManager(this);
            connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
            QNetworkRequest request;
            request.setUrl(QUrl("http://localhost:7777/v1/ordersforrepair?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a"));
            request.setRawHeader("Content-Type", "application/json");
            manager->post(request, strJson);
    /********************************************************************/
}

void zakaznaryad::replyFinished(QNetworkReply * reply)
{
           qDebug()<<"Reply: "<<  reply->readAll();
           reply->deleteLater();
}

void zakaznaryad::putOrderForRepair()
{
    /********************************************************************/
        QString order_number=ui->lineNumber->text();
        QString sum=ui->lineSum->text();
        QString gos_number=ui->lineGosNumber->text();
        QString brand=ui->lineBrand->text();
        QString date_of_conclusion=ui->lineDate->text();
        QString id(jsonByteArrayOrder);
    /********************************************************************/
        QJsonObject Order;
        Order["id"]=id;
        Order["order_number"]=order_number;
        Order[ "sum" ] = sum ;
        Order[ "gos_number" ] = gos_number;
        Order[ "brand" ] = brand;
        Order[ "date_of_conclusion" ] = date_of_conclusion;
    /********************************************************************/
        QJsonDocument doc( Order);
        QByteArray strJson(doc.toJson());

        QNetworkAccessManager* manager = new QNetworkAccessManager(this);
        connect(manager, &QNetworkAccessManager::finished, this, &zakaznaryad::replyPutOrderFinished);
        QNetworkRequest request;

        request.setUrl(QUrl("http://localhost:7777/v1/ordersforrepair?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
        manager->put(request, strJson);
    /********************************************************************/
}

void zakaznaryad::replyPutOrderFinished(QNetworkReply* reply)
{
    qDebug()<<"ReplyPutTo: "<<  reply->readAll();
    reply->deleteLater();
}

void zakaznaryad::disableButtons()
{
    ui->btnChange->setVisible(false);
    if(userRole!="Admin"||userRole!="super_admin")
        ui->btnLoad->setVisible(false);
}

void zakaznaryad::disableDownload()
{

}


void zakaznaryad::loadZakazNaryad(QString extension, QString filePath)
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
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyLoadFinished(QNetworkReply*)));
    QNetworkRequest request;
    request.setUrl(QUrl("http://localhost:7777/v1/ordersforrepair/file/"+jsonByteArrayOrder+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a"));
    qDebug() << "idTO="<<idTO;
    QNetworkReply *reply = manager->post(request, multiPart);
    multiPart->setParent(reply);
}

void zakaznaryad::replyLoadFinished(QNetworkReply *reply)
{
    qDebug() <<"load reply:"<<reply->readAll();
    connectManagerGetOrder();
    reply->deleteLater();
}

void zakaznaryad::on_btnLoad_clicked()
{
    QString extensionReversed;
    QString path=QFileDialog::getOpenFileName(this,QString::fromUtf8("Открыть файл"),QDir::currentPath());
    if(path.isEmpty())
        return;
    for(int i = path.length()-1;path[i]!='.';i--)
    {
        extensionReversed.append(path[i]);
    }
    extensionReversed.append('.');
    QString extension;
    for(int i = extensionReversed.length()-1;i>=0;i--)
    {
        extension.append(extensionReversed[i]);
    }
    qDebug() << "path:=" <<path;
    qDebug() << "extensionReversed:="<<extensionReversed;
    qDebug() << "extension:="<<extension;
    loadZakazNaryad(extension,path);
}

void zakaznaryad::on_btnDwnld_clicked()
{
    pathToZakaznaryadFile = QFileDialog::getExistingDirectory(0, "Выберите папку", "");
    if(pathToZakaznaryadFile.isEmpty())
        return;
    qDebug() <<"fileNameSts="<<fileNameZakaznaryad;
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/ordersforrepair/file/"+jsonByteArrayOrder+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &zakaznaryad::onDownloadStsFinished);
}

void zakaznaryad::onDownloadStsFinished(QNetworkReply *reply)
{
    if(reply->error())
    {
            qDebug() << "ERROR";
            qDebug() << reply->errorString();
    }
    QByteArray json = reply->readAll();
    if(json.isEmpty())
    {
        if(userRole=="User")
            qDebug()<<" no Row selected"<<QMessageBox::information(this,"Файл не существует!","Обратитесь к Администратору.");
        else
            qDebug()<<" no Row selected"<<QMessageBox::information(this,"Файл не существует!","Загрузите файл!");
        reply->deleteLater();
        return;
    }
   qDebug() << "path+extension="<<pathToZakaznaryadFile+"/"+fileNameZakaznaryad;
    QFile *file = new QFile(pathToZakaznaryadFile+"/"+fileNameZakaznaryad);
    if(file->open(QFile::WriteOnly))
    {
        file->write(json);
        file->close();
    }
    reply->deleteLater();
}
