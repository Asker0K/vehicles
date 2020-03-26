#include "tyreforcar.h"
#include "ui_tyreforcar.h"

tyreForCar::tyreForCar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tyreForCar)
{
    ui->setupUi(this);

    connectManagerSetTyre();
}

tyreForCar::~tyreForCar()
{
    delete ui;
}

void tyreForCar::connectManagerSetTyre()
{
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/tyre/free?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &tyreForCar::replyFinishedSetTyre);
}

void tyreForCar::replyFinishedSetTyre(QNetworkReply* reply)
{
    /************************creating QJsonTable with filling data in it******************************/
            QJsonTableModel::Header headerSklad;
            headerSklad.push_back( QJsonTableModel::Heading( { {"title","id"},    {"index","id"} }) );
            headerSklad.push_back( QJsonTableModel::Heading( { {"title","Название"},   {"index","name"} }) );
            headerSklad.push_back( QJsonTableModel::Heading( { {"title","Типоразмер"},  {"index","type_size"} }) );
            headerSklad.push_back( QJsonTableModel::Heading( { {"title","Пробег"},  {"index","mileage"} }) );
            headerSklad.push_back( QJsonTableModel::Heading( { {"title","Дата установки"},  {"index","date_of_setting"} }) );

            tableShowTyres = new QJsonTableModel( headerSklad, this );

            proxyModel = new QSortFilterProxyModel(this);
            proxyModel->setSourceModel( tableShowTyres);

            ui->tableTyres->setModel( proxyModel );
            ui->tableTyres->setSortingEnabled(true);
            ui->tableTyres->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->tableTyres->setSelectionMode(QAbstractItemView::SingleSelection);
            ui->tableTyres->setColumnHidden(0,true);
            ui->tableTyres->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
/*****************************************************************************************/
            QByteArray json = reply->readAll();
            QJsonDocument jsonDocument = QJsonDocument::fromJson( json );
            tableShowTyres->setJson( jsonDocument );
            qDebug() <<"JsonDocument:"<<jsonDocument;
/*****************************************************************************************/
            reply->deleteLater();
            qDebug() << "model tableSklad created|\n ";
}

void tyreForCar::on_tableTyres_clicked(const QModelIndex &index)
{
    QModelIndex index2 = proxyModel->mapToSource(index);
    objectIndexOfSelectedTyre = tableShowTyres->getJsonObject( index2 );
    qDebug() << objectIndexOfSelectedTyre;
    qDebug() << objectIndexOfSelectedTyre["id"];

    byteArrayIdOfTyreSelected = objectIndexOfSelectedTyre["id"].toString().toUtf8();//convert QJsonObject into QbyteArray
    isTyreRowSelected = true;
    /*******convert QJsonObject into QJsonDocument*****************/
        QJsonDocument doc(objectIndexOfSelectedTyre);
        QByteArray jsonByteArray(doc.toJson(QJsonDocument::Compact)) ;
        qDebug() <<"success!" <<jsonByteArray;
        QJsonDocument docObj = QJsonDocument::fromJson(jsonByteArray);
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
    /***********fill the lines with data from json**********************/
        QJsonObject json_obj=docObj.object();
        QVariantMap json_map = json_obj.toVariantMap();
    /*********************************************************/
        name = json_map["name"].toString();
        type_size = json_map["type_size"].toString();
        mileage = (QString::number(json_map["mileage"].toInt()));
        date_of_setting = json_map["date_of_setting"].toString();
    /*********************************************************/
}

void tyreForCar::setTyreForCurrentCar()
{
    /***********************put from lines to json****************************/
            QJsonObject setTyre;
            QString id(jsonByteArrayRezinaId);
            setTyre[ "vehicle_id" ] = id;
            setTyre[ "name" ] = name;
            setTyre[ "type_size" ] = type_size;
            setTyre[ "mileage" ] = mileage;
            setTyre[ "date_of_setting" ] = date_of_setting;
    /***************************post request*******************************/
            QJsonDocument doc( setTyre);
            QByteArray strJson(doc.toJson());

            manager = new QNetworkAccessManager(this);
            connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinishedSet(QNetworkReply*)));
            QNetworkRequest request;
            request.setUrl(QUrl("http://localhost:7777/v1/tyre?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a"));
            request.setRawHeader("Content-Type", "application/json");
            QNetworkReply *reply = manager->post(request, strJson);
    /********************************************************************/
}

void tyreForCar::replyFinishedSet(QNetworkReply* reply)
{
    qDebug()<<"Reply: "<<  reply->readAll();
    reply->deleteLater();
}

void tyreForCar::deleteRequest()
{
    manager = new QNetworkAccessManager();
    manager->deleteResource(QNetworkRequest(QUrl("http://localhost:7777/v1/tyre/"+ byteArrayIdOfTyreSelected+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &tyreForCar::replyDeleteFinished);
}

void tyreForCar::replyDeleteFinished(QNetworkReply* reply)
{
    qDebug() << "\ntyre deleted!\n";
    qDebug()<<reply->readAll();
    reply->deleteLater();
}

void tyreForCar::on_btnSet_clicked()
{
    if(isTyreRowSelected==true)
    {
        QMessageBox msgBox;
              msgBox.setText("Вы уверены?");
              msgBox.setInformativeText("Установить?");
              msgBox.setIcon(QMessageBox::Question);
              QPushButton *yes = msgBox.addButton(tr("Да"),QMessageBox::ActionRole);
              QPushButton *no = msgBox.addButton(tr("Нет"),QMessageBox::ActionRole);
              msgBox.exec();
              if(msgBox.clickedButton()==yes)
               {
                      deleteRequest();
                      setTyreForCurrentCar();
                      this->close();
               }
              else if(msgBox.clickedButton()==no)
              {
                      return;
              }
    }
    else
        {
          qDebug()<<" no Row selected"<<QMessageBox::information(this,"Не выбрана строка","Выберите строку!");
        }
}

void tyreForCar::on_btnCancel_clicked()
{
    this->close();
}
