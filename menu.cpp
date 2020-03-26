#include "menu.h"
#include "ui_menu.h"

menu::menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::menu)
{
    ui->setupUi(this);
    openDopogFile *dopog = new openDopogFile();
    dopog->connectManagerGet();

    imgSearchTyres();
    imgSearchRemont();

    setDisabledKartaTC(true);
    setVisableButtons(true);

    ui->btnOk->setVisible(false);
    ui->btnCancel->setVisible(false);

    isUserAdmin();

    connectManagerRezina();
    connectManagerOrderForRepair();
    connectManagerVehiclesAlarmTyres();
    connectManagerCertificateDopogAlarm();

}

menu::~menu()
{
    delete ui;
}

void menu::imgSearchTyres()
{
    QPixmap pix(":/resource/img/search.png");
    int w = ui->searchIconTyres->width();
    int h = ui->searchIconTyres->height();
    ui->searchIconTyres->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
}

void menu::imgSearchRemont()
{
    QPixmap pix(":/resource/img/search.png");
    int w = ui->searchIconRemont->width();
    int h = ui->searchIconRemont->height();
    ui->searchIconRemont->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
}

void menu::imgSearchTO()
{
    QPixmap pix(":/resource/img/search.png");
    int w = ui->searchIconTO->width();
    int h = ui->searchIconTO->height();
    ui->searchIconTO->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
}

void menu::isUserAdmin()
{
    if(userRole=="User")
    {
        ui->btnChangeGsm->setVisible(false);
        ui->btnAddGsm->setVisible(false);
        ui->btnChange->setVisible(false);
        ui->btnDeleteGsm->setVisible(false);
        ui->btnKartaTCRefresh->setVisible(false);
        ui->btnLoadPTS->setDisabled(true);
        ui->btnLoadSTS->setDisabled(true);

        ui->setRezinaForTc->setVisible(false);
        ui->btnRefreshRezina->setVisible(false);

        ui->btnAddOrderForRepair->setVisible(false);
        ui->btnDeleteRemont_TC->setVisible(false);
        ui->btnChangeRemont_TC->setVisible(false);
        ui->btnRefreshRemontTc->setVisible(false);
    }
}

void menu::on_btnSklad_clicked()
{
    sklad *skl = new sklad;
    skl->show();
}

int menu::getPutOrPost()
{
/**************function checks putOrPost request****************/
    if(putOrPost=='u')
    {
        qDebug()<<"putOrPost: "<<putOrPost;
        return 1;
    }
    else if(putOrPost=='o')
    {
        qDebug()<<"putOrPost: "<<putOrPost;
        return 2;
    }
/*********************************************************/
    return 0;
}

void menu::on_zakazNaryad_clicked()
{
    if(isOrderSelected==true)
    {
        zakaznaryad *zkn = new zakaznaryad;
        zkn->show();
        zkn->disableButtons();
        zkn->connectManagerGetOrder();
    }
    else qDebug()<<" no Row selected"<<QMessageBox::information(this,"Не выбрана строка","Выберите строку!");
}

void menu::on_btnReturn_clicked()
{
    clearDataJson();//clear Json
    isRowSelected = false;
    cars_list *cL = new cars_list;
    this->hide();
    cL->show();
}

void menu::on_btnChange_clicked()
{
    setDisabledKartaTC(false);
    ui->btnOk->setVisible(true);
    ui->btnCancel->setVisible(true);
    ui->btnChange->setVisible(false);

}

void menu::on_btnOk_clicked()
{
    setDisabledKartaTC(true);
    ui->btnChange->setVisible(true);
    ui->btnCancel->setVisible(false);
    ui->btnOk->setVisible(false);
    /******check putOrPost****/
    if(getPutOrPost()==1)
    {
            putDataToDB();
            putToHandbook();
            putTo();
    }
    else if(getPutOrPost()==2)
    {
            if(isKartaTcFull()==false)
            {
                qDebug()<<"dopog is empty"<<QMessageBox::information(this,"Пустые поля","Не все поля заполнены,\nЗаполните все поля");
                return;
            }
            postDataToDB();
            QMessageBox msgBox;
                  msgBox.setText("Вы уверены?");
                  msgBox.setInformativeText("Создать?");
                  msgBox.setIcon(QMessageBox::Question);
                  QPushButton *yes = msgBox.addButton(tr("Да"),QMessageBox::ActionRole);
                  QPushButton *no = msgBox.addButton(tr("Нет"),QMessageBox::ActionRole);
                  msgBox.exec();
                  if(msgBox.clickedButton()==yes)
                   {
                      postTO();
                      postHandbookTo();
                      cars_list *cL = new cars_list();
                      this->hide();
                      cL->show();
                   }
                  else if(msgBox.clickedButton()==no)
                  {
                          return;
                  }
    }
}

bool menu::isKartaTcFull()
{
    QString marka=ui->markaTC->text();
    QString model=ui->modelTC->text();
    QString type=ui->typeTC->text();
    QString godVipuska=ui->godVipuska->text();
    QString gosNo=ui->gosNo->text();
    QString probeg=ui->probeg->text();
    QString vinNo=ui->vinNo->text();
    QString stsNumber=ui->stsNumber->text();
    QString ptsNumber=ui->ptsNumber->text();

    QString date_of_lastTO=ui->lineDateTo->text();
    QString date_of_lastGrm=ui->lineDateGrm->text();
    QString mileageTO=ui->lineMileageTo->text();
    QString periodTO=ui->linePeriodBetweenTo->text();
    QString periodGrm=ui->linePeriodBetweenGrm->text();


    if(marka=='\0'||model=='\0'||type=='\0'||godVipuska=='\0'||gosNo=='\0'||
            probeg=='\0'||vinNo=='\0'||stsNumber=='\0'||ptsNumber=='\0'||date_of_lastTO=='\0'||
            date_of_lastGrm=='\0'||mileageTO=='\0'||periodTO=='\0'||periodGrm=='\0')
    {
        return false;
    }
    else
    {
         return true;
    }
}

void menu::on_tcForRezina_clicked(const QModelIndex &index)
{
    QModelIndex index2 = proxyModel->mapToSource(index);
    indexSelectedCarRezina = rezinaCarsList->getJsonObject( index2 );

    jsonByteArrayRezinaId = indexSelectedCarRezina["id"].toString().toUtf8();//convert QJsonObject into QbyteArray

    /*******convert QJsonObject into QJsonDocument*****************/
    QJsonDocument doc(indexSelectedCarRezina);
    QByteArray jsonByteArray(doc.toJson(QJsonDocument::Compact)) ;
    QJsonDocument docObj = QJsonDocument::fromJson(jsonByteArray);
    /*********************************************************/
    if(docObj.isNull())
    {
        qDebug()<<"Failed to create JSON doc.";
        isResinaTcSelected = false;
    }
    if(!docObj.isObject())
    {
        qDebug()<<"JSON is not an object.";
        exit(3);
    }
    /*********************************************************/
    isResinaTcSelected = true;
    connectManagerCarsRezina();
}

void menu::setDisabledKartaTC(bool isEnabled)
{
/************setReadOnly lines**********************/
    ui->markaTC->setReadOnly(isEnabled);
    ui->modelTC->setReadOnly(isEnabled);
    ui->typeTC->setReadOnly(isEnabled);
    ui->godVipuska->setReadOnly(isEnabled);
    ui->gosNo->setReadOnly(isEnabled);
    ui->probeg->setReadOnly(isEnabled);
    ui->vinNo->setReadOnly(isEnabled);
    ui->stsNumber->setReadOnly(isEnabled);
    ui->ptsNumber->setReadOnly(isEnabled);
    ui->lineDateGrm->setReadOnly(isEnabled);
    ui->lineDateTo->setReadOnly(isEnabled);
    ui->linePeriodBetweenGrm->setReadOnly(isEnabled);
    ui->linePeriodBetweenTo->setReadOnly(isEnabled);
    ui->lineMileageTo->setReadOnly(isEnabled);
/************************************************/
}


void menu::setVisableButtons(bool isVisible)
{
    ui->btnChange->setVisible(isVisible);
}


void menu::getDataFromDB()
{
    /*************convert QJsonObject into QJsonDocument*******************/
        QJsonDocument doc(objectIndexOfCarSelected);
        QByteArray jsonByteArray(doc.toJson(QJsonDocument::Compact)) ;
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
/********************************************************************/
        ui->typeTC->setText(json_map["vehicle_type"].toString());
        ui->gosNo->setText(json_map["gos_number"].toString());
        ui->markaTC->setText(json_map["brand"].toString());
        ui->modelTC->setText(json_map["model"].toString());
        ui->godVipuska->setText(json_map["year_of_manufacture"].toString());
        ui->vinNo->setText(json_map["vin"].toString());
        ui->probeg->setText(QString::number(json_map["mileage"].toInt()));
        ui->stsNumber->setText(json_map["sts"].toString());
        ui->ptsNumber->setText(json_map["pts"].toString());
        fileNameSts = json_map["sts_filename"].toString();
        fileNamePts = json_map["pts_filename"].toString();
        ui->authorOfChangedData->setText(json_map["author"].toString());
        ui->dateOfLastChanging->setText(json_map["date_of_creating"].toString());

/********************************************************************/
        gosNumberTC=ui->gosNo->text();
}

void menu::connectManagerRezina()
{
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/vehicles?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &menu::replyRezinaFinished);
}

void menu::replyRezinaFinished(QNetworkReply* reply)
{
    /************************creating QJsonTable with filling data in it******************************/
            QJsonTableModel::Header headerRezina;
            headerRezina.push_back( QJsonTableModel::Heading( { {"title","id"},    {"index","id"} }) );
            headerRezina.push_back( QJsonTableModel::Heading( { {"title","Марка"},   {"index","brand"} }) );
            headerRezina.push_back( QJsonTableModel::Heading( { {"title","Модель"},  {"index","model"} }) );
            headerRezina.push_back( QJsonTableModel::Heading( { {"title","Тип"}, {"index","vehicle_type"} }) );

            rezinaCarsList = new QJsonTableModel(headerRezina, this);

            proxyModel = new QSortFilterProxyModel(this);
            proxyModel->setSourceModel(rezinaCarsList);

            ui->tcForRezina->setModel( proxyModel );
            ui->tcForRezina->setSortingEnabled(true);
            ui->tcForRezina->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->tcForRezina->setSelectionMode(QAbstractItemView::SingleSelection);
            ui->tcForRezina->setColumnHidden(0,true);
            ui->tcForRezina->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
/*****************************************************************************************/
            QByteArray json = reply->readAll();
            QJsonDocument jsonDocument = QJsonDocument::fromJson( json );
            rezinaCarsList->setJson( jsonDocument );
/*****************************************************************************************/
            reply->deleteLater();

}

void menu::connectManagerCarsRezina()
{
    QString stringIndexSelectedCarRezina(jsonByteArrayRezinaId);
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/tyre/veh/"+stringIndexSelectedCarRezina+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &menu::replyCarsRezinaFinished);
}

void menu::replyCarsRezinaFinished(QNetworkReply* reply)
{
    /************************creating QJsonTable with filling data in it******************************/
            QJsonTableModel::Header headerRezinaShow;
            headerRezinaShow.push_back( QJsonTableModel::Heading( { {"title","id"},    {"index","id"} }) );
            headerRezinaShow.push_back( QJsonTableModel::Heading( { {"title","Название"},   {"index","name"} }) );
            headerRezinaShow.push_back( QJsonTableModel::Heading( { {"title","Типоразмер"},  {"index","type_size"} }) );
            headerRezinaShow.push_back( QJsonTableModel::Heading( { {"title","Пробег"}, {"index","mileage"} }) );
            headerRezinaShow.push_back( QJsonTableModel::Heading( { {"title","Дата"}, {"index","date_of_setting"} }) );

            rezinaCarsListShow = new QJsonTableModel(headerRezinaShow, this);

            proxyModelSubTyre = new QSortFilterProxyModel(this);
            proxyModelSubTyre->setSourceModel(rezinaCarsListShow);

            ui->tableRezinaShow->setModel(  proxyModelSubTyre );
            ui->tableRezinaShow->setSortingEnabled(true);
            ui->tableRezinaShow->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->tableRezinaShow->setSelectionMode(QAbstractItemView::SingleSelection);
            ui->tableRezinaShow->setColumnHidden(0,true);
            ui->tableRezinaShow->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
/*****************************************************************************************/
            QByteArray json = reply->readAll();
            QJsonDocument jsonDocument = QJsonDocument::fromJson( json );
            qDebug() << "jsonDocument o get request="<<jsonDocument;
            rezinaCarsListShow->setJson( jsonDocument );
/*****************************************************************************************/
            reply->deleteLater();
            isRezinaShow = true;
}


void menu::putDataToDB()
{
/********************************************************************/
    QString dt = QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss");
    QString marka=ui->markaTC->text();
    QString model=ui->modelTC->text();
    QString type=ui->typeTC->text();
    QString godVipuska=ui->godVipuska->text();
    QString gosNo=ui->gosNo->text();
    QString probeg=ui->probeg->text();
    QString vinNo=ui->vinNo->text();
    QString stsNumber=ui->stsNumber->text();
    QString ptsNumber=ui->ptsNumber->text();
    QString idToPutData (jsonByteArrayId);
/********************************************************************/
    QJsonObject kartaTC;
    kartaTC["id"]= idToPutData;
    kartaTC["vehicle_type"]=type;
    kartaTC[ "gos_number" ] = gosNo ;
    kartaTC[ "brand" ] = marka;
    kartaTC[ "model" ] = model;
    kartaTC[ "year_of_manufacture" ] = godVipuska;
    kartaTC[ "vin" ] = vinNo;
    kartaTC[ "mileage" ] = probeg;
    kartaTC[ "sts" ] = stsNumber;
    kartaTC[ "pts" ] = ptsNumber;
    kartaTC[ "author" ] = username ;
    kartaTC[ "date_of_creating" ] = dt ;
/********************************************************************/
    QJsonDocument doc( kartaTC);
    QByteArray strJson(doc.toJson());

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &menu::replyFinished);
    QNetworkRequest request;

    request.setUrl(QUrl("http://localhost:7777/v1/vehicles?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    QNetworkReply *reply = manager->put(request, strJson);
    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)),this, SLOT(slotSslErrors(QList<QSslError>)));
/********************************************************************/
}

void menu::replyFinished(QNetworkReply * reply)
{
           qDebug()<<"replyFinished="<<reply->readAll();
           reply->deleteLater();
}

void menu::clearDataJson()
{
/***************clear QJsonObject********************/
        objectIndexOfCarSelected.remove("vehicle_type");
        objectIndexOfCarSelected.remove("gos_number");
        objectIndexOfCarSelected.remove("brand");
        objectIndexOfCarSelected.remove("model");
        objectIndexOfCarSelected.remove("year_of_manufacture");
        objectIndexOfCarSelected.remove("vin");
        objectIndexOfCarSelected.remove("mileage");
        objectIndexOfCarSelected.remove("sts");
        objectIndexOfCarSelected.remove("pts");
        objectIndexOfCarSelected.remove("StsFileName");
        objectIndexOfCarSelected.remove("PtsFileName");
        objectIndexOfCarSelected.remove("author");
        objectIndexOfCarSelected.remove("date_of_creating");
/**************************************************/
        isRowSelected = false;
}

void menu::on_setRezinaForTc_clicked()
{
    if( isResinaTcSelected == true)
    {
        tyreForCar *tyre = new tyreForCar();
        tyre->show();
    }
    else
        qDebug()<<"no row selected"<<QMessageBox::information(this,"Не выбрана машина","Выберите машину!");
}


void menu::connectManagerOrderForRepair()
{
    QString id(jsonByteArrayId);
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/ordersforrepair/veh/"+id+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &menu::replyOrderForRepair);
}

void menu::replyOrderForRepair(QNetworkReply* reply)
{
    /************************creating QJsonTable with filling data in it******************************/
            QJsonTableModel::Header headerOrderForRepair;
            headerOrderForRepair.push_back( QJsonTableModel::Heading( { {"title","id"},{"index","id"} }) );
            headerOrderForRepair.push_back( QJsonTableModel::Heading( { {"title","Дата док-та"},{"index","date_of_conclusion"} }) );
            headerOrderForRepair.push_back( QJsonTableModel::Heading( { {"title","№ Заказ наряда"},{"index","order_number"} }) );
            headerOrderForRepair.push_back( QJsonTableModel::Heading( { {"title","Марка ТС"},{"index","brand"} }) );
            headerOrderForRepair.push_back( QJsonTableModel::Heading( { {"title","Гос.номер ТС"},{"index","gos_number"} }) );
            headerOrderForRepair.push_back( QJsonTableModel::Heading( { {"title","Контрагент"},{"index","contractor"} }) );
            headerOrderForRepair.push_back( QJsonTableModel::Heading( { {"title","Итог"},{"index","sum"} }) );

            orderForRepair = new QJsonTableModel(headerOrderForRepair, this);

            proxyModelOrder = new QSortFilterProxyModel(this);
            proxyModelOrder->setSourceModel(orderForRepair);


            ui->remont_tc->setModel( proxyModelOrder );
            ui->remont_tc->setSortingEnabled(true);
            ui->remont_tc->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->remont_tc->setSelectionMode(QAbstractItemView::SingleSelection);
            ui->remont_tc->setColumnHidden(0,true);
            ui->remont_tc->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
/*****************************************************************************************/
            QByteArray json = reply->readAll();
            QJsonDocument jsonDocument = QJsonDocument::fromJson( json );
            orderForRepair->setJson( jsonDocument );


/*****************************************************************************************/
            reply->deleteLater();
}

void menu::on_btnAddOrderForRepair_clicked()
{
    zakaznaryad *zkzNaryad = new zakaznaryad();
    isPut=false;
    zkzNaryad->show();
    zkzNaryad->disableDownload();
}


void menu::connectManagerGSM()
{
    QString id(jsonByteArrayId);
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/mileagelists/veh/"+id+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &menu::replyGsmFinished);
}

void menu::replyGsmFinished(QNetworkReply* reply)
{
    /************************creating QJsonTable with filling data in it******************************/
            QJsonTableModel::Header headerGSM;
            headerGSM.push_back( QJsonTableModel::Heading( { {"title","id"},    {"index","id"} }) );
            headerGSM.push_back( QJsonTableModel::Heading( { {"title","Дата"},   {"index","date"} }) );
            headerGSM.push_back( QJsonTableModel::Heading( { {"title","№\nПутевого\nлиста"},  {"index","track_number"} }) );
            headerGSM.push_back( QJsonTableModel::Heading( { {"title","Пробег\nна\nмомент\nвыезда,км"}, {"index","mileage_before_out"} }) );
            headerGSM.push_back( QJsonTableModel::Heading( { {"title","Пройдено\nпо\nгороду,\nкм"}, {"index","on_city"} }) );
            headerGSM.push_back( QJsonTableModel::Heading( { {"title","Пройдено\nпо\nтрассе,\nкм"}, {"index","on_trace"} }) );
            headerGSM.push_back( QJsonTableModel::Heading( { {"title","Пробег на\nмомент\nвозвра-\nщения,\nкм"}, {"index","mileage_after_in"} }) );

            gsmTable = new QJsonTableModel(headerGSM, this);

            proxyModelMileage = new QSortFilterProxyModel(this);
            proxyModelMileage->setSourceModel(gsmTable);

            ui->tableMileageKartaTC->setModel(  proxyModelMileage );
            ui->tableMileageKartaTC->setSortingEnabled(true);
            ui->tableMileageKartaTC->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->tableMileageKartaTC->setSelectionMode(QAbstractItemView::SingleSelection);
            ui->tableMileageKartaTC->setColumnHidden(0,true);
            ui->tableMileageKartaTC->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
/*****************************************************************************************/
            QJsonTableModel::Header headerGsmMileage;
            headerGsmMileage.push_back( QJsonTableModel::Heading( { {"title","id"},    {"index","id"} }) );
            headerGsmMileage.push_back( QJsonTableModel::Heading( { {"title","Дата"},   {"index","date"} }) );
            headerGsmMileage.push_back( QJsonTableModel::Heading( { {"title","№\nПутевого\nлиста"},  {"index","track_number"} }) );
            headerGsmMileage.push_back( QJsonTableModel::Heading( { {"title","Остаток ГСМ\nпри выезде,\nл"}, {"index","ostatok_gsm_before_out"} }) );
            headerGsmMileage.push_back( QJsonTableModel::Heading( { {"title","Расход топливаn\nпо городу\n(факт),\nл"}, {"index","fuel_consumption_in_city"} }) );
            headerGsmMileage.push_back( QJsonTableModel::Heading( { {"title","Расход топлива\nпо трассе\n(факт),\nл"}, {"index","fuel_consumption_on_trace"} }) );
            headerGsmMileage.push_back( QJsonTableModel::Heading( { {"title","Выдано\nГСМ"}, {"index","given_gsm"} }) );
            headerGsmMileage.push_back( QJsonTableModel::Heading( { {"title","Остаток ГСМ\nпри возвращении,\nл"}, {"index","ostatok_gsm_after_in"} }) );
            headerGsmMileage.push_back( QJsonTableModel::Heading( { {"title","Расход\nтоплива\nпо городу\n(норма),\nл"}, {"index","normal_fuel_consumption_in_city"} }) );
            headerGsmMileage.push_back( QJsonTableModel::Heading( { {"title","Расход\nтоплива\nпо трассе\n(норма),\nл"}, {"index","normal_fuel_consumption_on_trace"} }) );

            gsmMileageTable = new QJsonTableModel(headerGsmMileage, this);

            proxyModelGsm = new QSortFilterProxyModel(this);
            proxyModelGsm->setSourceModel(gsmMileageTable);

            ui->tableGsmKartaTC->setModel(  proxyModelGsm );
            ui->tableGsmKartaTC->setSortingEnabled(true);
            ui->tableGsmKartaTC->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->tableGsmKartaTC->setSelectionMode(QAbstractItemView::SingleSelection);
            ui->tableGsmKartaTC->setColumnHidden(0,true);
           // ui->tableGsmKartaTC->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
/*****************************************************************************************/
            QByteArray json = reply->readAll();
            QJsonDocument jsonDocument = QJsonDocument::fromJson( json );
            gsmTable->setJson( jsonDocument );
            gsmMileageTable->setJson( jsonDocument );
/*****************************************************************************************/
            reply->deleteLater();

}




void menu::on_btnAddGsm_clicked()
{
    if(getPutOrPost()!=2)
    {
        addGsm *GSM = new addGsm();
        GSM->show();
    }
    else
        qDebug()<<"Dopog doesnt exist"<<QMessageBox::information(this,"Новое транспортное средство!","Данное меню будет доступно\nпосле создания ТС.");

}

void menu::on_btnRefreshRemontTc_clicked()
{
  //  isSkladRowSelected = false;
    ui->search_query_remont->clear();
    ui->search_query_remont->setFocus();
    ui->searchIconRemont->show();
    if(isOrderSelected==true)
    {
        orderForRepair->deleteLater();
        isOrderSelected=false;
    }
    connectManagerOrderForRepair();
    qDebug()<<"\nRemont_tc refreshed!\n";
}

void menu::on_btnRefreshRezina_clicked()
{
    ui->searchTcRezina->clear();
    ui->searchTcRezina->setFocus();
    ui->searchIconTyres->show();

    connectManagerRezina();
    if(isRezinaShow==true)
    {
        proxyModelSubTyre->deleteLater();
        isRezinaShow=false;
    }
        isResinaTcSelected=false;
        isTyreShowSelected=false;

}

void menu::on_tableMileageKartaTC_clicked(const QModelIndex &index)
{
    QModelIndex index2 = proxyModelMileage->mapToSource(index);
    objectIndexOfMileageCarSelected = gsmMileageTable->getJsonObject( index2 );

    jsonByteArrayMileage = objectIndexOfMileageCarSelected["id"].toString().toUtf8();//convert QJsonObject into QbyteArray
    whichTableSelected = 0;
    isMileageSelected = true;
}

void menu::on_tableGsmKartaTC_clicked(const QModelIndex &index)
{
    QModelIndex index2 = proxyModelGsm->mapToSource(index);
    objectIndexOfGsmCarSelected = gsmTable->getJsonObject( index2 );

    jsonByteArrayGsm = objectIndexOfGsmCarSelected["id"].toString().toUtf8();//convert QJsonObject into QbyteArray
    whichTableSelected = 1;
    isGsmSelected = true;
}

void menu::on_btnChangeGsm_clicked()
{
    if(getPutOrPost()!=2)
    {
        addGsm *GSM = new addGsm();
        GSM->show();
    }
    else
        qDebug()<<"Dopog doesnt exist"<<QMessageBox::information(this,"Новое транспортное средство!","Данное меню будет доступно\nпосле создания ТС.");

}

void menu::on_btnKartaTCRefresh_clicked()
{
    if(getPutOrPost()!=2)
    {
        if((whichTableSelected == 0)&&(isMileageSelected==true))
        {
            addGsm *gsm = new addGsm();
            gsm->show();
            gsm->addGsm::connectManagerGetMileageData();
            gsm->addGsm::isBtnsOkOrSave();
        }
        else if(isGsmSelected == false)
        {
           qDebug()<<" no Row selected"<<QMessageBox::information(this,"Не выбрана строка","Выберите строку!");
           return;
        }

        if((whichTableSelected==1)&&(isGsmSelected==true))
        {
            addGsm *gsm = new addGsm();
            gsm->show();
            gsm->addGsm::connectManagerGetGsmData();
            gsm->addGsm::isBtnsOkOrSave();
        }
        else if(isMileageSelected==false)
           qDebug()<<" no Row selected"<<QMessageBox::information(this,"Не выбрана строка","Выберите строку!");
    }
    else
        qDebug()<<"Dopog doesnt exist"<<QMessageBox::information(this,"Новое транспортное средство!","Данное меню будет доступно\nпосле создания ТС.");

}

void menu::on_btnDeleteGsm_clicked()
{
    if((whichTableSelected == 0)&&(isMileageSelected==true))
    {
        QMessageBox msgBox;
              msgBox.setText("Вы уверены?");
              msgBox.setInformativeText("Удалить?");
              msgBox.setIcon(QMessageBox::Question);
              QPushButton *yes = msgBox.addButton(tr("Да"),QMessageBox::ActionRole);
              QPushButton *no = msgBox.addButton(tr("Нет"),QMessageBox::ActionRole);
              msgBox.exec();
              if(msgBox.clickedButton()==yes)
               {
                      deleteMileageRequest();
               }
              else if(msgBox.clickedButton()==no)
              {
                      return;
              }
    }
     if((whichTableSelected==1)&&(isGsmSelected==true))
     {
         QMessageBox msgBox;
               msgBox.setText("Вы уверены?");
               msgBox.setInformativeText("Удалить?");
               msgBox.setIcon(QMessageBox::Question);
               QPushButton *yes = msgBox.addButton(tr("Да"),QMessageBox::ActionRole);
               QPushButton *no = msgBox.addButton(tr("Нет"),QMessageBox::ActionRole);
               msgBox.exec();
               if(msgBox.clickedButton()==yes)
                {
                       deleteGsmRequest();
                }
               else if(msgBox.clickedButton()==no)
               {
                       return;
               }
     }
     else if(isGsmSelected==false&&isMileageSelected==false)
     {
        qDebug()<<" no Row selected"<<QMessageBox::information(this,"Не выбрана строка","Выберите строку!");
     }
}



void menu::deleteMileageRequest()
{
    QString id(jsonByteArrayMileage);
    manager = new QNetworkAccessManager();
    manager->deleteResource(QNetworkRequest(QUrl("http://localhost:7777/v1/mileagelists/"+id+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &menu::replyDeleteMileagelistsFinished);
}

void menu::deleteGsmRequest()
{
    QString id(jsonByteArrayGsm);
    manager = new QNetworkAccessManager();
    manager->deleteResource(QNetworkRequest(QUrl("http://localhost:7777/v1/mileagelists/"+id+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &menu::replyDeleteMileagelistsFinished);
}

void menu::replyDeleteMileagelistsFinished(QNetworkReply *reply)
{
    qDebug()<<reply->readAll();
    reply->deleteLater();
}


void menu::filterModelTyres()
{
    QString searchValue = ui->searchTcRezina->text();
    proxyModel->setFilterRegExp(QRegExp(searchValue, Qt::CaseInsensitive, QRegExp::FixedString));
    proxyModel->setFilterKeyColumn(1);
    ui->tcForRezina->setModel(proxyModel);
}

void menu::filterModelRemont()
{
    QString searchValue = ui->search_query_remont->text();
    proxyModelOrder->setFilterRegExp(QRegExp(searchValue, Qt::CaseInsensitive, QRegExp::FixedString));
   proxyModelOrder->setFilterKeyColumn(1);
    ui->remont_tc->setModel(proxyModelOrder);
}

void menu::on_btnSearchRezina_clicked()
{
    filterModelTyres();
}

void menu::on_searchTcRezina_textChanged(const QString &arg1)
{
    if(arg1!='\0')
        ui->searchIconTyres->hide();
}

void menu::on_search_query_remont_textChanged(const QString &arg1)
{
    if(arg1!='\0')
        ui->searchIconRemont->hide();
}


void menu::mousePressEvent(QMouseEvent *event)
{
  if(event->buttons()==Qt::LeftButton)
    {
        ui->searchTcRezina->clear();
        ui->searchTcRezina->setFocus();
        ui->searchIconTyres->show();
        on_btnSearchRezina_clicked();

        ui->search_query_remont->clear();
        ui->search_query_remont->setFocus();
        ui->searchIconRemont->show();
        on_btnSearch_remont_clicked();
    }
}

void menu::on_btnSearch_remont_clicked()
{
    filterModelRemont();
}




void menu::on_searchTcRezina_editingFinished()
{
    on_btnSearchRezina_clicked();
}

void menu::on_search_query_remont_editingFinished()
{
    on_btnSearch_remont_clicked();
}

void menu::replyLoadFinished(QNetworkReply*reply)
{
    qDebug() <<"load reply:"<<reply->readAll();
    reply->deleteLater();
}

void menu::on_btnLoadSTS_clicked()
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
    for(int i =extensionReversed.length()-1;i>=0;i--)
    {
        extension.append(extensionReversed[i]);
    }
    qDebug() << "path:=" <<path;
    qDebug() << "extensionReversed:="<<extensionReversed;
    qDebug() << "extension:="<<extension;
    loadFileSTS(extension,path);
}

void menu::on_btnOpenDopog_clicked()
{
    openDopogFile *openDopog = new openDopogFile();
    if(getPutOrPost()!=2)
    {
        openDopog->connectManagerGet();
        openDopog->show();
    }
    else
        qDebug()<<"Dopog doesnt exist"<<QMessageBox::information(this,"Новое транспортное средство!","Данное меню будет доступно\nпосле создания ТС.");
}

void menu::loadFileSTS(QString extension, QString filePath)
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
    request.setUrl(QUrl("http://localhost:7777/v1/vehicles/stsfile/"+jsonByteArrayId+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a"));
    qDebug() << "car_id="<<jsonByteArrayId;
    QNetworkReply *reply = manager->post(request, multiPart);
    multiPart->setParent(reply);
}

void menu::loadFilePTS(QString extension, QString filePath)
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
    request.setUrl(QUrl("http://localhost:7777/v1/vehicles/ptsfile/"+jsonByteArrayId+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a"));
    //request.setRawHeader("Content-Type", "extension/file");
    qDebug() << "vehicle_id="<<jsonByteArrayId;
    QNetworkReply *reply = manager->post(request, multiPart);
    multiPart->setParent(reply);
}

void menu::on_btnLoadPTS_clicked()
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
    loadFilePTS(extension,path);
    on_btnKartaTCRefresh_clicked();
}

void menu::on_btnDwnldDopog_clicked()
{
    if(getPutOrPost()!=2)
    {
        pathToDopogFile = QFileDialog::getExistingDirectory(0, "Выберите папку", "");
        if(pathToDopogFile.isEmpty())
            return;
        manager = new QNetworkAccessManager();
        manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/certificates/file/"+dopogIdIs+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
        connect(manager, &QNetworkAccessManager::finished, this, &menu::onDownloadDopogFinished);
    }
    else qDebug()<<"Dopog doesnt exist"<<QMessageBox::information(this,"Новое транспортное средство!","Данное меню будет доступно\nпосле создания ТС.");
}

void menu::onDownloadDopogFinished(QNetworkReply* reply)
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
   qDebug() << "path+extension="<<pathToDopogFile+"/"+fileNameDopog;
    QFile *file = new QFile(pathToDopogFile+"/"+fileNameDopog);
    if(file->open(QFile::WriteOnly))
    {
        file->write(json);
        file->close();
    }
    reply->deleteLater();
}

void menu::on_btnDwnldPTS_clicked()
{
    pathToPtsFile = QFileDialog::getExistingDirectory(0, "Выберите папку", "");
    if(pathToPtsFile.isEmpty())
        return;
    QString id(jsonByteArrayId);
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/vehicles/ptsfile/"+id+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &menu::onDownloadPtsFinished);
}

void menu::on_btnDwnldCTC_clicked()
{
    pathToStsFile = QFileDialog::getExistingDirectory(0, "Выберите папку", "");
    if(pathToStsFile.isEmpty())
        return;
    qDebug() <<"fileNameSts="<<fileNameSts;
    QString id(jsonByteArrayId);
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/vehicles/stsfile/"+id+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &menu::onDownloadStsFinished);
}

void menu::onDownloadStsFinished(QNetworkReply *reply)
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
   qDebug() << "path+extension="<<pathToStsFile+"/"+fileNameSts;
    QFile *file = new QFile(pathToStsFile+"/"+fileNameSts);
    if(file->open(QFile::WriteOnly))
    {
        file->write(json);
        file->close();
    }
    reply->deleteLater();
}

void menu::onDownloadPtsFinished(QNetworkReply *reply)
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

    QFile *file = new QFile(pathToPtsFile+"/"+fileNamePts);
    if(file->open(QFile::WriteOnly))
    {
        file->write(json);
        file->close();
    }
     reply->deleteLater();
}

void menu::on_btnSave_Probeg_clicked()
{
    pathToSaveCsvFileProbeg = QFileDialog::getSaveFileName(0, "Выберите папку", "",".csv");
    getProbegFromDb();
}

void menu::getProbegFromDb()
{
        QString id(jsonByteArrayId);
        manager = new QNetworkAccessManager();
        manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/mileagelists/veh/"+id+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
        connect(manager, &QNetworkAccessManager::finished, this, &menu::replyGetProbegFromDbFinished);
}

void menu::replyGetProbegFromDbFinished(QNetworkReply * reply)
{
    QByteArray json = reply->readAll();
    QString jsonDoc(json);
    QJsonDocument jsonDocument = QJsonDocument::fromJson( json );
    QJsonArray jsArray = jsonDocument.array();
    int jsArrayCount=jsArray.count();
    qDebug() << "jsonLength="<<jsArray.count();
    exportJsonToCSVProbeg_Gsm(jsArrayCount,pathToSaveCsvFileProbeg,jsonDoc);
    reply->deleteLater();
}

void menu::exportJsonToCSVProbeg_Gsm(int jsArrayCount,QString pathToSaveCsvFile,QString jsonDoc)
{
    const auto& data = jsonDoc;
    QRegularExpression regex("\\[|\\]");
    const auto& jsons = data.split(regex);

        for(const auto& json : jsons)
        {
            if(!json.trimmed().isEmpty())
            {
                const auto& formattedJson = QString("[%1]").arg(json);
                const auto& doc = QJsonDocument::fromJson(formattedJson.toUtf8());

                if(doc.isArray())
                    for(const auto& item : doc.array())
                    {
                        const auto& obj = item.toObject();
                        const auto& keys = obj.keys();

                        for(const auto& key : keys)
                        {
                           if(key=="date")
                                probegDateArray.append(obj[key].toString());
                            if(key=="track_number")
                                probegtrack_numberArray.append(obj[key].toString());
                            if(key=="mileage_before_out")
                                probegmileage_before_outArray.append(obj[key].toString());
                            if(key=="on_city")
                                probegon_cityArray.append(obj[key].toString());
                            if(key=="on_trace")
                                probegon_traceArray.append(obj[key].toString());
                            if(key=="mileage_after_in")
                                probegmileage_after_inArray.append(obj[key].toString());
                            if(key=="ostatok_gsm_before_out")
                                probegostatok_gsm_before_outArray.append(obj[key].toString());
                            if(key=="fuel_consumption_in_city")
                                probegfuel_consumption_in_cityArray.append(obj[key].toString());
                            if(key=="fuel_consumption_on_trace")
                                probegfuel_consumption_on_traceArray.append(obj[key].toString());
                            if(key=="given_gsm")
                                probeggiven_gsmArray.append(obj[key].toString());
                            if(key=="ostatok_gsm_after_in")
                                probegostatok_gsm_after_inArray.append(obj[key].toString());
                            if(key=="normal_fuel_consumption_in_city")
                                probegnormal_fuel_consumption_in_cityArray.append(obj[key].toString());
                            if(key=="normal_fuel_consumption_on_trace")
                                probegnormal_fuel_consumption_on_traceArray.append(obj[key].toString());
                        }
                    }
            }
        }

    QFile *csvFile = new QFile(pathToSaveCsvFile+".csv");

        if(csvFile->open( QIODevice::WriteOnly ))
        {
            QTextStream textStream( csvFile );
            QStringList stringList;
            for(int row=0;row<jsArrayCount+1;row++)
            {
                    stringList.clear();
                    if(row==0)
                    {
                            for(int col=0;col<13;col++)
                            {
                                stringList << arrayNameOfProbeg[col];
                            }
                            textStream << stringList.join( ';' )+"\n";
                    }
                    else
                    {
                            for(int col=0;col<13;col++)
                            {
                                    switch(col)
                                    {
                                        case 0:  stringList << probegDateArray[row-1];
                                        break;
                                            case 1:stringList << probegtrack_numberArray[row-1];
                                        break;
                                            case 2:stringList << probegmileage_before_outArray[row-1];
                                        break;
                                            case 3:stringList << probegon_cityArray[row-1];
                                        break;
                                            case 4:stringList << probegon_traceArray[row-1];
                                        break;
                                            case 5:stringList << probegmileage_after_inArray[row-1];
                                        break;
                                            case 6:stringList << probegostatok_gsm_before_outArray[row-1];
                                        break;
                                            case 7:stringList << probegfuel_consumption_in_cityArray[row-1];
                                        break;
                                            case 8:stringList << probegfuel_consumption_on_traceArray[row-1];
                                        break;
                                            case 9:stringList << probeggiven_gsmArray[row-1];
                                        break;
                                            case 10:stringList << probegostatok_gsm_after_inArray[row-1];
                                        break;
                                            case 11:stringList << probegnormal_fuel_consumption_in_cityArray[row-1];
                                        break;
                                            case 12:stringList << probegnormal_fuel_consumption_on_traceArray[row-1];
                                        break;
                                    }

                            }
                                                textStream << stringList.join( ';' )+"\n";
                    }
              }
                csvFile->close();
        }

}

void menu::on_btnSave_tyres_clicked()
{
    pathToSaveCsvFileTyre = QFileDialog::getSaveFileName(0, "Выберите папку", "",".csv");
    getTyreFromDb();
}

void menu::getTyreFromDb()
{
    QString id(jsonByteArrayId);
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/tyre/veh/"+id+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &menu::replyGetTyreFromDbFinished);
}

void menu::replyGetTyreFromDbFinished(QNetworkReply * reply)
{
    QByteArray json = reply->readAll();
    QString jsonDoc(json);
    QJsonDocument jsonDocument = QJsonDocument::fromJson( json );
    QJsonArray jsArray = jsonDocument.array();
    int jsArrayCount=jsArray.count();
    exportJsonToCSVTyre(jsArrayCount,pathToSaveCsvFileTyre,jsonDoc);
    reply->deleteLater();
}

void menu::exportJsonToCSVTyre(int jsArrayCount,QString pathToSaveCsvFile, QString jsonDoc)
{
    const auto& data = jsonDoc;
    QRegularExpression regex("\\[|\\]");
    const auto& jsons = data.split(regex);

        for(const auto& json : jsons)
        {
            if(!json.trimmed().isEmpty())
            {
                const auto& formattedJson = QString("[%1]").arg(json);
                const auto& doc = QJsonDocument::fromJson(formattedJson.toUtf8());

                if(doc.isArray())
                    for(const auto& item : doc.array())
                    {
                        const auto& obj = item.toObject();
                        const auto& keys = obj.keys();

                        for(const auto& key : keys)
                        {
                           if(key=="type_size")
                                tyre_type_sizeArray.append(obj[key].toString());
                            if(key=="name")
                                tyre_nameArray.append(obj[key].toString());
                            if(key=="date_of_setting")
                                tyre_date_of_settingArray.append(obj[key].toString());
                            if(key=="mileage")
                                tyre_mileageArray.append(obj[key].toString());
                        }
                    }
            }
        }

    QFile *csvFile = new QFile(pathToSaveCsvFile+".csv");

        if(csvFile->open( QIODevice::WriteOnly ))
        {
            QTextStream textStream( csvFile );
            QStringList stringList;
            for(int row=0;row<jsArrayCount+1;row++)
            {
                    stringList.clear();
                    if(row==0)
                    {
                            for(int col=0;col<4;col++)
                            {
                                stringList << tyre_headerArray[col];
                            }
                            textStream << stringList.join( ';' )+"\n";
                    }
                    else
                    {
                            for(int col=0;col<4;col++)
                            {
                                    switch(col)
                                    {
                                        case 0:  stringList << tyre_date_of_settingArray[row-1];
                                        break;
                                            case 1:stringList << tyre_nameArray[row-1];
                                        break;
                                            case 2:stringList << tyre_type_sizeArray[row-1];
                                        break;
                                            case 3:stringList << tyre_mileageArray[row-1];
                                        break;
                                    }

                            }
                           textStream << stringList.join( ';' )+"\n";
                    }
              }
                csvFile->close();
        }

}



void menu::on_btnDeleteRemont_TC_clicked()
{
    if(isOrderSelected == true)
    {
        QMessageBox msgBox;
              msgBox.setText("Вы уверены?");
              msgBox.setInformativeText("Удалить?");
              msgBox.setIcon(QMessageBox::Question);
              QPushButton *yes = msgBox.addButton(tr("Да"),QMessageBox::ActionRole);
              QPushButton *no = msgBox.addButton(tr("Нет"),QMessageBox::ActionRole);
              msgBox.exec();
              if(msgBox.clickedButton()==yes)
               {
                      deleteOrderRequest();
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

void menu::on_btnChangeRemont_TC_clicked()
{
     if(isOrderSelected == true)
     {
    zakaznaryad *zakaz = new zakaznaryad();
    zakaz->show();
    zakaz->zakaznaryad::isUserAdmin();
    zakaz->zakaznaryad::connectManagerGetOrder();
    isPut=true;
     }
     else qDebug()<<" no Row selected"<<QMessageBox::information(this,"Не выбрана строка","Выберите строку!");
}

void menu::on_remont_tc_clicked(const QModelIndex &index)
{
    QModelIndex index2 = proxyModelOrder->mapToSource(index);
    objectIndexOfOrderCarSelected = orderForRepair->getJsonObject(index2);

    jsonByteArrayOrder = objectIndexOfOrderCarSelected["id"].toString().toUtf8();//convert QJsonObject into QbyteArray
    qDebug() <<"jsonByteArrayOrder="<<jsonByteArrayOrder;
    isOrderSelected = true;
}

void menu::deleteOrderRequest()
{
    QString id(jsonByteArrayOrder);
    qDebug() <<"jsonByteArrayOrder="<<id;
    manager = new QNetworkAccessManager();
    manager->deleteResource(QNetworkRequest(QUrl("http://localhost:7777/v1/ordersforrepair/"+id+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &menu::replyDeleteOrderFinished);
}

void menu::replyDeleteOrderFinished(QNetworkReply * reply)
{
    qDebug() << "ReplyDeleteOrder="<<reply->readAll();
    reply->deleteLater();
}

void menu::connectManagerTo()
{
    QString id(jsonByteArrayId);
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/to_s/veh/"+id+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &menu::replyToFinished);
}

void menu::replyToFinished(QNetworkReply* reply)
{
    QByteArray json = reply->readAll();
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
    idTO = json_map["id"].toString();
    QString dateStr1 = json_map["date_of_last_to"].toString();
    QDate date1 =QDate::fromString((dateStr1),"dd.MM.yyyy");
    ui->lineDateTo->setDate(date1);
    QString dateStr2 = json_map["date_of_last_grm_service"].toString();
    QDate date2 =QDate::fromString((dateStr2),"dd.MM.yyyy");
    ui->lineDateGrm->setDate(date2);
    ui->lineMileageTo->setText(json_map["mileage_for_next_to"].toString());


    /*********************************************************/
    reply->deleteLater();
}

void menu::connectManagerHandbookTo()
{
    QString id(jsonByteArrayId);
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/handbook/own/"+id+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &menu::replyHandbookToFinished);
}

void menu::replyHandbookToFinished(QNetworkReply* reply)
{
    QByteArray json = reply->readAll();
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
    ui->linePeriodBetweenTo->setText(json_map["period_between_to"].toString());
    ui->linePeriodBetweenGrm->setText(json_map["period_between_grm_service"].toString());
    /*********************************************************/
    reply->deleteLater();
}

void menu::postTO()
{
    /********************************************************************/
            QString date_of_last_to=ui->lineDateTo->text();
            QString date_of_last_grm_service=ui->lineDateGrm->text();
            QString mileage_for_next_to=ui->lineMileageTo->text();
    /***********************put from lines to json****************************/
            QJsonObject TO;
            TO["vehicle_id"]=jsonByteArrayNewId;
            qDebug() <<"postToNewId="<<jsonByteArrayNewId;
            TO["date_of_last_to"]=date_of_last_to;
            TO[ "date_of_last_grm_service" ] = date_of_last_grm_service ;
            TO[ "mileage_for_next_to" ] = mileage_for_next_to;
    /***************************post request*******************************/
            QJsonDocument doc(TO);
            QByteArray strJson(doc.toJson());

            QNetworkAccessManager* manager = new QNetworkAccessManager(this);
            connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyPostToFinished(QNetworkReply*)));
            QNetworkRequest request;
            request.setUrl(QUrl("http://localhost:7777/v1/to_s?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a"));
            request.setRawHeader("Content-Type", "application/json");
            QNetworkReply *reply = manager->post(request, strJson);
    /********************************************************************/
}

void menu::replyPostToFinished(QNetworkReply* reply)
{
    qDebug() <<"postTO="<<reply->readAll();
    reply->deleteLater();
}

void menu::postHandbookTo()
{
    /********************************************************************/
            QString period_between_to=ui->linePeriodBetweenTo->text();
            QString period_between_grm_service=ui->linePeriodBetweenGrm->text();
    /***********************put from lines to json****************************/
            QJsonObject Handbook;
            Handbook["id"]=jsonByteArrayNewId;
            qDebug() <<"postToNewId="<<jsonByteArrayNewId;
            Handbook["period_between_to"]=period_between_to;
            Handbook[ "period_between_grm_service" ] = period_between_grm_service ;
    /***************************post request*******************************/
            QJsonDocument doc( Handbook);
            QByteArray strJson(doc.toJson());

            QNetworkAccessManager* manager = new QNetworkAccessManager(this);
            connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyPostHandbookToFinished(QNetworkReply*)));
            QNetworkRequest request;
            request.setUrl(QUrl("http://localhost:7777/v1/handbook?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a"));
            request.setRawHeader("Content-Type", "application/json");
            QNetworkReply *reply = manager->post(request, strJson);
    /********************************************************************/
}

void menu::replyPostHandbookToFinished(QNetworkReply* reply)
{
    qDebug() <<"postHandbookTo="<<reply->readAll();
    reply->deleteLater();
}

void menu::putTo()
{
    /********************************************************************/
        QString date_of_last_to=ui->lineDateTo->text();
        QString date_of_last_grm_service=ui->lineDateGrm->text();
        QString mileage_for_next_to=ui->lineMileageTo->text();
    /********************************************************************/
        QJsonObject TO;
        TO["id"]=idTO;
        TO["date_of_last_to"]=date_of_last_to;
        TO[ "date_of_last_grm_service" ] = date_of_last_grm_service ;
        TO[ "mileage_for_next_to" ] = mileage_for_next_to;
    /********************************************************************/
        QJsonDocument doc( TO);
        QByteArray strJson(doc.toJson());

        QNetworkAccessManager* manager = new QNetworkAccessManager(this);
        connect(manager, &QNetworkAccessManager::finished, this, &menu::replyPutToFinished);
        QNetworkRequest request;

        request.setUrl(QUrl("http://localhost:7777/v1/to_s?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
        QNetworkReply *reply = manager->put(request, strJson);
    /********************************************************************/
}

void menu::replyPutToFinished(QNetworkReply* reply)
{
    qDebug()<<"ReplyPutTo: "<<  reply->readAll();
    reply->deleteLater();
}

void menu::putToHandbook()
{
    /********************************************************************/
        QString period_between_to=ui->linePeriodBetweenTo->text();
        QString period_between_grm_service=ui->linePeriodBetweenGrm->text();
        QString vehicle_id(jsonByteArrayId);
    /***********************put from lines to json****************************/
        QJsonObject Handbook;
        Handbook["id"]=vehicle_id;
        Handbook["period_between_to"]=period_between_to;
        Handbook[ "period_between_grm_service" ] = period_between_grm_service ;
    /********************************************************************/
        QJsonDocument doc( Handbook);
        QByteArray strJson(doc.toJson());

        QNetworkAccessManager* manager = new QNetworkAccessManager(this);
        connect(manager, &QNetworkAccessManager::finished, this, &menu::replyPutToHandbookFinished);
        QNetworkRequest request;

        request.setUrl(QUrl("http://localhost:7777/v1/handbook?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
        QNetworkReply *reply = manager->put(request, strJson);
    /********************************************************************/
}

void menu::replyPutToHandbookFinished(QNetworkReply* reply)
{
    qDebug()<<"ReplyPutTOHandbook: "<<  reply->readAll();
    reply->deleteLater();
}


void menu::postDataToDB()
{
    /********************************************************************/
            QString dt = QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss");
            QString marka=ui->markaTC->text();
            QString model=ui->modelTC->text();
            QString type=ui->typeTC->text();
            QString godVipuska=ui->godVipuska->text();
            QString gosNo=ui->gosNo->text();
            QString probeg=ui->probeg->text();
            QString vinNo=ui->vinNo->text();
            QString stsNumber=ui->stsNumber->text();
            QString ptsNumber=ui->ptsNumber->text();
    /***********************put from lines to json****************************/
            QJsonObject kartaTC;
            kartaTC["vehicle_type"]=type;
            kartaTC[ "gos_number" ] = gosNo ;
            kartaTC[ "brand" ] = marka;
            kartaTC[ "model" ] = model;
            kartaTC[ "year_of_manufacture" ] = godVipuska;
            kartaTC[ "vin" ] = vinNo;
            kartaTC[ "mileage" ] = probeg;
            kartaTC[ "sts" ] = stsNumber;
            kartaTC[ "pts" ] = ptsNumber;
            kartaTC[ "author" ] = username ;
            kartaTC[ "date_of_creating" ] = dt ;
    /***************************post request*******************************/
            QJsonDocument doc( kartaTC);
            QByteArray strJson(doc.toJson());

            QNetworkAccessManager* manager = new QNetworkAccessManager(this);
            connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinishedPostDataToDB(QNetworkReply*)));
            QNetworkRequest request;
            request.setUrl(QUrl("http://localhost:7777/v1/vehicles?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a"));
            request.setRawHeader("Content-Type", "application/json");
            QNetworkReply *reply = manager->post(request, strJson);
            connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    /********************************************************************/
}

void menu::replyFinishedPostDataToDB(QNetworkReply* reply)
{

    QByteArray json = reply->readAll();
    QJsonDocument jsonDocument = QJsonDocument::fromJson( json );
    qDebug()<<"ReplyNewID: "<<json;
    QJsonObject json_obj=jsonDocument.object();
    QVariantMap json_map = json_obj.toVariantMap();
    jsonByteArrayNewId=json_map["id"].toString();
    qDebug() <<"new ID="<<jsonByteArrayNewId;
    reply->deleteLater();
}

void menu::slotReadyRead()
{
    qDebug() <<"111111111111";
}


void menu::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox;
          msgBox.setText("Внимание!");
          msgBox.setInformativeText("Выход из приложения!\nВы уверены?");
          msgBox.setIcon(QMessageBox::Warning);
          QPushButton *yes = msgBox.addButton(tr("Да"),QMessageBox::ActionRole);
          QPushButton *no = msgBox.addButton(tr("Нет"),QMessageBox::ActionRole);
          msgBox.exec();
          if(msgBox.clickedButton()==yes){
              event->accept();
              qApp->closeAllWindows();
          }
          else if(msgBox.clickedButton()==no) event->ignore();
}

void menu::on_btnDeleteTyre_clicked()
{
    if(isTyreShowSelected==true)
    {
        QMessageBox msgBox;
              msgBox.setText("Вы уверены?");
              msgBox.setInformativeText("Списать на склад?");
              msgBox.setIcon(QMessageBox::Question);
              QPushButton *yes = msgBox.addButton(tr("Да"),QMessageBox::ActionRole);
              QPushButton *no = msgBox.addButton(tr("Нет"),QMessageBox::ActionRole);
              msgBox.exec();
              if(msgBox.clickedButton()==yes)
               {
                  postTyreToSklad();
                  deleteTyreShowRequest();
               }
              else if(msgBox.clickedButton()==no)
              {
                      return;
              }
    }
    else qDebug()<<" no Row selected"<<QMessageBox::information(this,"Не выбрана Резина","Выберите Резину\nдля списания!");

    alarm *a=new alarm();
    a->connectManagerGetVehicles();
}


void menu::postTyreToSklad()
{
            QJsonObject json = transformDataToByteArrayTyreShow();
            qDebug() <<"json="<<json;
            QJsonDocument doc(json );
            QByteArray strJson(doc.toJson());
            QNetworkAccessManager* manager = new QNetworkAccessManager(this);
            connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyPostTyreToSkaldFinished(QNetworkReply*)));
            QNetworkRequest request;
            request.setUrl(QUrl("http://localhost:7777/v1/tyre?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a"));
            request.setRawHeader("Content-Type", "application/json");
            manager->post(request, strJson);
}

void menu::replyPostTyreToSkaldFinished(QNetworkReply* reply)
{
   QByteArray json = reply->readAll();
   qDebug() << "replyPostTyreToSkladFinished=" << json;
}



void menu::on_tableRezinaShow_clicked(const QModelIndex &index)
{
    QModelIndex index2 = proxyModelSubTyre->mapToSource(index);
    indexSelectedCarTyreShow = rezinaCarsListShow->getJsonObject( index2 );
    QVariantMap json_map = indexSelectedCarTyreShow.toVariantMap();
    qDebug() << "json_map" <<json_map ;
    jsonByteArrayTyreShowId = json_map["id"].toString().toUtf8();//convert QJsonObject into QbyteArray
    type_size=json_map["type_size"].toString().toUtf8();
    name=json_map["name"].toString().toUtf8();
    date_of_setting=json_map["date_of_setting"].toString().toUtf8();
    mileage=json_map["mileage"].toString().toUtf8();
    isTyreShowSelected = true;
}

QJsonObject menu::transformDataToByteArrayTyreShow()
{
    QJsonObject tyreShowData;
    tyreShowData["type_size"]=type_size;
    tyreShowData["name"]=name;
    tyreShowData["date_of_setting"]=date_of_setting;
    tyreShowData["mileage"]=mileage;
    return tyreShowData;
}


void menu::deleteTyreShowRequest()
{
    QString id(jsonByteArrayTyreShowId);
    manager = new QNetworkAccessManager();
    manager->deleteResource(QNetworkRequest(QUrl("http://localhost:7777/v1/tyre/"+id+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &menu::replyDeleteTyreShowFinished);
}

void menu::replyDeleteTyreShowFinished(QNetworkReply *reply)
{
    qDebug()<<reply->readAll();
    reply->deleteLater();
}

void menu::on_btnSave_remont_clicked()
{
    pathToSaveCsvFileRemont = QFileDialog::getSaveFileName(0, "Выберите папку", "",".csv");
    getRemontFromDb();
}


void menu::getRemontFromDb()
{
        manager = new QNetworkAccessManager();
        manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/ordersforrepair?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
        connect(manager, &QNetworkAccessManager::finished, this, &menu::replyGetRemontFromDbFinished);
}

void menu::replyGetRemontFromDbFinished(QNetworkReply * reply)
{
    QByteArray json = reply->readAll();
    QString jsonDoc(json);
    QJsonDocument jsonDocument = QJsonDocument::fromJson( json );
    QJsonArray jsArray = jsonDocument.array();
    qDebug() << "jsonLength="<<jsArray.count();
    int jsArrayCount=jsArray.count();
    exportJsonToCSVRemont(jsArrayCount,pathToSaveCsvFileRemont,jsonDoc);
    reply->deleteLater();
}

void menu::exportJsonToCSVRemont(int jsArrayCount,QString pathToSaveCsvFile, QString jsonDoc)
{
    const auto& data = jsonDoc;
    QRegularExpression regex("\\[|\\]");
    const auto& jsons = data.split(regex);

        for(const auto& json : jsons)
        {
            if(!json.trimmed().isEmpty())
            {
                const auto& formattedJson = QString("[%1]").arg(json);
                const auto& doc = QJsonDocument::fromJson(formattedJson.toUtf8());

                if(doc.isArray())
                    for(const auto& item : doc.array())
                    {
                        const auto& obj = item.toObject();
                        const auto& keys = obj.keys();

                        for(const auto& key : keys)
                        {
                           if(key=="date_of_conclusion")
                                remont_date_of_conclusion.append(obj[key].toString());
                            if(key=="order_number")
                                remont_order_number.append(obj[key].toString());
                            if(key=="brand")
                                remont_brand.append(obj[key].toString());
                            if(key=="gos_number")
                                remont_gos_number.append(obj[key].toString());
                            if(key=="contractor")
                                remont_contractor.append(obj[key].toString());
                            if(key=="sum")
                                remont_sum.append(obj[key].toString());
                        }
                    }
            }
        }

    QFile *csvFile = new QFile(pathToSaveCsvFile+".csv");

        if(csvFile->open( QIODevice::WriteOnly ))
        {
            QTextStream textStream( csvFile );
            QStringList stringList;
            for(int row=0;row<jsArrayCount+1;row++)
            {
                    stringList.clear();
                    if(row==0)
                    {
                            for(int col=0;col<6;col++)
                            {
                                stringList << remont_headerArray[col];
                            }
                            textStream << stringList.join( ';' )+"\n";
                    }
                    else
                    {
                            for(int col=0;col<6;col++)
                            {
                                    switch(col)
                                    {
                                        case 0:  stringList << remont_date_of_conclusion[row-1];
                                        break;
                                            case 1:stringList << remont_order_number[row-1];
                                        break;
                                            case 2:stringList << remont_brand[row-1];
                                        break;
                                            case 3:stringList << remont_gos_number[row-1];
                                        break;
                                        case 4:stringList << remont_contractor[row-1];
                                             break;
                                        case 5:stringList << remont_sum[row-1];
                                             break;
                                    }

                            }
                           textStream << stringList.join( ';' )+"\n";
                    }
              }
                csvFile->close();
        }

}





void menu::connectManagerVehiclesAlarmTyres()
{
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/tyre?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &menu::replyAlarmVehiclesTyresFinished);
}

void menu::replyAlarmVehiclesTyresFinished(QNetworkReply* reply)
{
/*****************************************************************************************/
            QByteArray json = reply->readAll();
            if(reply->error())
            {
                qDebug() << "json_error=" <<json;
                return;
            }
            QString jsonDocument(json);
            qDebug() <<jsonDocument;
            rememberVehiclesTyresData(jsonDocument);
            qDebug() << "array_tyreDataWithVehicleId" << array_tyreDataWithVehicleId;
            QJsonDocument doc(array_tyreDataWithVehicleId);
            qDebug() << "doc=" <<doc.toJson(QJsonDocument::Compact);
            if(doc.isEmpty()==false)
            alarmTyresSetTable();
/*****************************************************************************************/
            reply->deleteLater();
}

void menu::rememberVehiclesTyresData(QString jsonDoc)
{
        const auto& data = jsonDoc;
        QRegularExpression regex("\\[|\\]");
        const auto& jsons = data.split(regex);
            for(const auto& json : jsons)
            {
                if(!json.trimmed().isEmpty())
                {
                    const auto& formattedJson = QString("[%1]").arg(json);
                    const auto& doc = QJsonDocument::fromJson(formattedJson.toUtf8());

                    if(doc.isArray()){
                         int i=0;
                                for(const auto& item : doc.array()) {
                                    const auto& obj = item.toObject();

                                    if(obj.contains("vehicle_id") && (obj["vehicle_id"].toString().isEmpty()==false) ){
                                        if(errorTyreId[i]!=0){
                                        if(i<errorTyreId.length()){
                                           itemData.insert("vehicle_id",obj["vehicle_id"].toString());
                                           itemData.insert("name",obj["name"].toString());
                                           itemData.insert("mileage",obj["mileage"].toString());
                                           array_tyreDataWithVehicleId.push_back(QJsonValue(itemData));
                                           i++;
                                          }
                                          else if(i<errorTyreId.length())i++;
                                         }

                                        }
                                    }
                                 }
                    }
                }
}

void menu::on_btnCancel_clicked()
{
    setDisabledKartaTC(true);
    ui->btnOk->setVisible(false);
    ui->btnCancel->setVisible(false);
    ui->btnChange->setVisible(true);
}

void menu::alarmTyresSetTable()
{
    /************************creating QJsonTable with filling data in it******************************/
            QJsonTableModel::Header headerAlarmTyres;
            headerAlarmTyres.push_back( QJsonTableModel::Heading( { {"title","Название"},{"index","name"} }) );
            headerAlarmTyres.push_back( QJsonTableModel::Heading( { {"title","Пробег на момент\nустановки"},{"index","mileage"} }) );

            alarmTyres = new QJsonTableModel(headerAlarmTyres, this);

            proxyModelAlarmTyres = new QSortFilterProxyModel(this);
            proxyModelAlarmTyres->setSourceModel(alarmTyres);

            ui->tableAlarmTyres->setModel( proxyModelAlarmTyres );
            ui->tableAlarmTyres->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->tableAlarmTyres->setSelectionMode(QAbstractItemView::SingleSelection);
            ui->tableAlarmTyres->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
/*****************************************************************************************/
            QJsonDocument doc(array_tyreDataWithVehicleId);
            alarmTyres->setJson( doc );
/*****************************************************************************************/
}



void menu::on_tableAlarmTyres_doubleClicked(const QModelIndex &index)
{
    QModelIndex index2 = proxyModelAlarmTyres->mapToSource(index);
    QJsonObject objectIndexOfAlarmTyres = alarmTyres->getJsonObject( index2 );
    QByteArray jsonByteArrayAlarmTyres = objectIndexOfAlarmTyres["vehicle_id"].toString().toUtf8();//convert QJsonObject into QbyteArray
    QString id(jsonByteArrayAlarmTyres);
    qDebug() <<"id=" <<id;
    ui->remont->setCurrentIndex(1);
    proxyModel->setFilterRegExp(QRegExp(id, Qt::CaseInsensitive,
                                                    QRegExp::FixedString));
   proxyModel->setFilterKeyColumn(0);
    ui->tcForRezina->setModel(proxyModel);
}

void menu::on_remont_tabBarClicked(int index)
{

}


void menu::connectManagerCertificateDopogAlarm()
{
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/certificates?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &menu::replyAlarmDopogFinished);
}

void menu::replyAlarmDopogFinished(QNetworkReply* reply)
{
/*****************************************************************************************/
            QByteArray json = reply->readAll();
            if(reply->error())
            {
                qDebug() << "json_error=" <<json;
                return;
            }
            QString jsonDocument(json);
            qDebug() <<  "DOPOG=\n" <<jsonDocument;
            rememberDopogAlarmData(jsonDocument);
            QJsonDocument doc(array_dopogDataWithId);
            qDebug() << "docDOPOG=" <<doc.toJson(QJsonDocument::Compact);
            if(doc.isEmpty()==false)
            alarmDopogSetTable();
/*****************************************************************************************/
            reply->deleteLater();
}

void menu::rememberDopogAlarmData(QString jsonDoc)
{
        const auto& data = jsonDoc;
        QRegularExpression regex("\\[|\\]");
        const auto& jsons = data.split(regex);
            for(const auto& json : jsons)
            {
                if(!json.trimmed().isEmpty())
                {
                    const auto& formattedJson = QString("[%1]").arg(json);
                    const auto& doc = QJsonDocument::fromJson(formattedJson.toUtf8());

                    if(doc.isArray())
                    {
                         int i=0;
                                for(const auto& item : doc.array())
                                {
                                    qDebug () << "dopog_i=" <<i;
                                    const auto& obj = item.toObject();
                                        if(obj["id"].toString().contains(dopogWithAlarmId[i]))
                                        {
                                            qDebug() << "WOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOW=" <<i;
                                            if(errorDopogId[i]!=0)
                                            {
                                                if(i<errorDopogId.length())
                                                {
                                                   dopogItemData.insert("id",obj["id"].toString());
                                                   dopogItemData.insert("period_of_validity",obj["period_of_validity"].toString());
                                                   dopogItemData.insert("gos_number",obj["gos_number"].toString());
                                                   array_dopogDataWithId.push_back(QJsonValue(dopogItemData));
                                                   i++;
                                                 }
                                                 else if(i<errorDopogId.length())i++;
                                             }
                                            else if(i<errorDopogId.length())i++;
                                        }
                                        else if(i<errorDopogId.length())i++;
                                }
                        }
                    }
                }
}

void menu::alarmDopogSetTable()
{
    /************************creating QJsonTable with filling data in it******************************/
            QJsonTableModel::Header headerAlarmDopog;
            headerAlarmDopog.push_back( QJsonTableModel::Heading( { {"title","Гос. номер ТС"},{"index","gos_number"} }) );
            headerAlarmDopog.push_back( QJsonTableModel::Heading( { {"title","Срок действия"},{"index","period_of_validity"} }) );

            alarmDopog = new QJsonTableModel(headerAlarmDopog, this);

            proxyModelAlarmDopog = new QSortFilterProxyModel(this);
            proxyModelAlarmDopog->setSourceModel(alarmDopog);

            ui->tableAlarmDopog->setModel( proxyModelAlarmDopog );
            ui->tableAlarmDopog->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->tableAlarmDopog->setSelectionMode(QAbstractItemView::SingleSelection);
            ui->tableAlarmDopog->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
/*****************************************************************************************/
            QJsonDocument doc(array_dopogDataWithId);
            alarmDopog->setJson( doc );
/*****************************************************************************************/
}
