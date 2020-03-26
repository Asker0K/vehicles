#include "cars_list.h"
#include "ui_cars_list.h"


cars_list::cars_list(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cars_list)
{
        ui->setupUi(this);
        imgSearch();
        imgBtnSettings();
        isUserAdmin();
        setDisabledcarsList(true);//disable lineEdits with short info
        connectManagerVehicle();//get request
        tyreAlarm();
}
cars_list::~cars_list()
{
    delete ui;
}

void cars_list::tyreAlarm()
{
    for(int i =0;i<errorTyreId.length();i++)
    {
        if(errorTyreId[i]==0)
        {
            qDebug()<<errorTyreId[i]<< "its OK_TYRE"<<tyreWithAlarmId[i];
        }
        if(errorTyreId[i]==1)
        {
            qDebug() <<errorTyreId[i]<< "close to be bad_TYRE"<<tyreWithAlarmId[i];
        }
        if(errorTyreId[i]==2)
            qDebug() <<errorTyreId[i]<<"WARNING!_TYRE"<<tyreWithAlarmId[i];
    }
    for(int i =0;i<errorDopogId.length();i++)
    {
        if(errorDopogId[i]==0)
        {
            qDebug()<<errorDopogId[i]<< "its OK_DOPOG"<<dopogWithAlarmId[i];
        }
        if(errorDopogId[i]==1)
        {
            qDebug() <<errorDopogId[i]<< "close to be bad_DOPOG"<<dopogWithAlarmId[i];
        }
        if(errorDopogId[i]==2)
            qDebug() <<errorDopogId[i]<<"WARNING!_DOPOG"<<dopogWithAlarmId[i];
    }
    for(int i =0;i<errorToId.length();i++)
    {
        if(errorToId[i]==0)
        {
            qDebug()<<errorToId[i]<< "its OK_TO"<<toWithAlarmId[i];
        }
        if(errorToId[i]==1)
        {
            qDebug() <<errorToId[i]<< "close to be bad_TO"<<toWithAlarmId[i];
        }
        if(errorToId[i]==2)
            qDebug() <<errorToId[i]<<"WARNING!_TO"<<toWithAlarmId[i];
    }
    for(int i =0;i<errorGrmId.length();i++)
    {
        if(errorGrmId[i]==0)
        {
            qDebug()<<errorGrmId[i]<< "its OK_GRM"<<grmWithAlarmId[i];
        }
        if(errorGrmId[i]==1)
        {
            qDebug() <<errorGrmId[i]<< "close to be bad_GRM"<<grmWithAlarmId[i];
        }
        if(errorGrmId[i]==2)
            qDebug() <<errorGrmId[i]<<"WARNING!_GRM"<<grmWithAlarmId[i];
    }
    if(errorTyreId.contains(1)||errorTyreId.contains(2)||errorDopogId.contains(1)||errorDopogId.contains(2)||errorToId.contains(1)||
       errorToId.contains(2)||errorGrmId.contains(1)||errorGrmId.contains(2))
    {
        if(errorTyreId.contains(2)||errorDopogId.contains(2)||errorToId.contains(2)||errorGrmId.contains(2))
        {
            imgIndicator( ":/resource/img/warningIcon.png");
        }
        else  imgIndicator(":/resource/img/attention.png" );
    }
    else  imgIndicator(":/resource/img/everythingOK.png");

}


void cars_list::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox;
          msgBox.setText("Внимание!");
          msgBox.setInformativeText("Выход из приложения!\nВы уверены?");
          msgBox.setIcon(QMessageBox::Warning);
          QPushButton *yes = msgBox.addButton(tr("Да"),QMessageBox::ActionRole);
          QPushButton *no = msgBox.addButton(tr("Нет"),QMessageBox::ActionRole);
          msgBox.exec();
          if(msgBox.clickedButton()==yes) event->accept();
          else if(msgBox.clickedButton()==no) event->ignore();
}

void cars_list::imgIndicator(QString path)
{
    qDebug() <<path;
    QPixmap pix(path);
    int w = ui->imgProblemIndicator->width();
    int h = ui->imgProblemIndicator->height();
    ui->imgProblemIndicator->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
}

void cars_list::imgSearch()
{
    QPixmap pix(":/resource/img/search.png");
    int w = ui->search_icon->width();
    int h = ui->search_icon->height();
    ui->search_icon->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
}

void cars_list::isUserAdmin()
{
    if(userRole=="User")
    {
        ui->btnSettings->setVisible(false);
        ui->labelSettings->setVisible(false);
        ui->btnAdd->setVisible(false);
        ui->btnRefresh->setVisible(false);
        ui->btnDelete->setVisible(false);
    }
}


void cars_list::imgBtnSettings()
{
    QPixmap pix(":/resource/img/user.png");
    QIcon icon(pix);
    int w =15;
    int h = 15;
    ui->btnSettings->setIcon(icon);
    ui->btnSettings->setIconSize(QSize(w,h));
}

void cars_list::on_btnSettings_clicked()
{
        userinfo *userIn = new userinfo();
        userIn->connectManagerUser();
        userIn->show();
}

void cars_list::on_btnRefresh_clicked()
{
    isRowSelected = false;
    clearDataJson();
    connectManagerVehicle();
    ui->search_query->clear();
    ui->search_query->setFocus();
    ui->search_icon->show();
    qDebug()<<"cars_list::refreshed";
}

void cars_list::on_btnOpen_clicked()
{
    if(isRowSelected==true)//if row is selected
    {
        menu *m = new menu();
        m->getDataFromDB();//fill data in lineEdits in menu
        m->connectManagerGSM();
        m->connectManagerTo();
        m->connectManagerHandbookTo();
        this->hide();
        m->show();
        putOrPost ='u';//give the value put
    }
    else
    {
         qDebug()<<"no row selected"<<QMessageBox::information(this,"Не выбрана машина","Выберите машину!");
    }

}

void cars_list::on_btnSearch_clicked()
{
    filterModel();
}

void cars_list::on_btnAdd_clicked()
{
    putOrPost ='o';//give param for post request
    menu *m = new menu;
    this->hide();
    m->show();
}

void cars_list::on_btnDelete_clicked()
{
    if(isRowSelected==true)
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
                      deleteRequest();
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
    menu* m = new menu();
    m->menu::clearDataJson();
    on_btnRefresh_clicked();
}

void cars_list::connectManagerVehicle()
{
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/vehicles?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &cars_list::replyFinished);
}

void cars_list::deleteRequest()
{
    manager = new QNetworkAccessManager();
    manager->deleteResource(QNetworkRequest(QUrl("http://localhost:7777/v1/vehicles/"+jsonByteArrayId+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &cars_list::replyDeleteFinished);
}

void cars_list::replyDeleteFinished(QNetworkReply* reply)
{
    qDebug()<<reply->readAll();
    reply->deleteLater();
}

void cars_list::replyFinished(QNetworkReply* reply)
{
    /************************creating QJsonTable with filling data in it******************************/
            QJsonTableModel::Header headerCarsList;
            headerCarsList.push_back( QJsonTableModel::Heading( { {"title","id"},    {"index","id"} }) );
            headerCarsList.push_back( QJsonTableModel::Heading( { {"title","Марка"},   {"index","brand"} }) );
            headerCarsList.push_back( QJsonTableModel::Heading( { {"title","Модель"},  {"index","model"} }) );
            headerCarsList.push_back( QJsonTableModel::Heading( { {"title","Тип"}, {"index","vehicle_type"} }) );

            episodesCarsList = new QJsonTableModel( headerCarsList, this );

            proxyModel = new QSortFilterProxyModel(this);
            proxyModel->setSourceModel(episodesCarsList);
            ui->db_cars_list->setModel(proxyModel);
            ui->db_cars_list->setSortingEnabled(true);
            ui->db_cars_list->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->db_cars_list->setSelectionMode(QAbstractItemView::SingleSelection);
            ui->db_cars_list->setColumnHidden(0,true);
            ui->db_cars_list->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
/*****************************************************************************************/
            QByteArray json = reply->readAll();
            QJsonDocument jsonDocument = QJsonDocument::fromJson( json );
            episodesCarsList->setJson( jsonDocument );
            qDebug() << "getVehicleFinished=" << json;
/*****************************************************************************************/
            reply->deleteLater();

}

void cars_list::setDisabledcarsList(bool isEnabled)
{
    /*************disable lineEdits***************/
    ui->lineMarka->setReadOnly(isEnabled);
    ui->lineModel->setReadOnly(isEnabled);
    ui->lineType->setReadOnly(isEnabled);
    ui->lineVIN->setReadOnly(isEnabled);
    ui->lineNumber->setReadOnly(isEnabled);
    ui->lineProbeg->setReadOnly(isEnabled);
}

void cars_list::on_db_cars_list_clicked(const QModelIndex &index)
{
    QModelIndex index2 = proxyModel->mapToSource(index);
    objectIndexOfCarSelected = episodesCarsList->getJsonObject( index2 );

    jsonByteArrayId = objectIndexOfCarSelected["id"].toString().toUtf8();//convert QJsonObject into QbyteArray


/*******convert QJsonObject into QJsonDocument*****************/
    QJsonDocument doc(objectIndexOfCarSelected);
    QByteArray jsonByteArray(doc.toJson(QJsonDocument::Compact)) ;
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
/*********************************************************/
    isRowSelected = true;
/***********fill the lines with data from json**********************/
    QJsonObject json_obj=docObj.object();
    QVariantMap json_map = json_obj.toVariantMap();
/*********************************************************/
    ui->lineType->setText(json_map["vehicle_type"].toString());
    ui->lineNumber->setText(json_map["gos_number"].toString());
    ui->lineMarka->setText(json_map["brand"].toString());
    ui->lineModel->setText(json_map["model"].toString());
    ui->lineVIN->setText(json_map["vin"].toString());
    ui->lineProbeg->setText(QString::number(json_map["mileage"].toInt()));
/*********************************************************/
}

void cars_list::on_search_query_textChanged(const QString &arg1)
{
    if(arg1!='\0')
        ui->search_icon->hide();
}

void cars_list::on_search_query_editingFinished()
{
    on_btnSearch_clicked();
}

void cars_list::mousePressEvent(QMouseEvent *event)
{
  if(event->buttons()==Qt::LeftButton)
    {
        ui->search_query->clear();
        ui->search_query->setFocus();
        ui->search_icon->show();
        on_btnSearch_clicked();
    }
}


void cars_list::clearDataJson()
{
/***************clear QJsonObject********************/
        ui->lineMarka->clear();
        ui->lineModel->clear();
        ui->lineNumber->clear();
        ui->lineProbeg->clear();
        ui->lineType->clear();
        ui->lineVIN->clear();
/**************************************************/
        isRowSelected = false;
}


void cars_list::filterModel()
{
    QString searchValue = ui->search_query->text();
    proxyModel->setFilterRegExp(QRegExp(searchValue, Qt::CaseInsensitive,
                                                QRegExp::FixedString));
   proxyModel->setFilterKeyColumn(1);
    ui->db_cars_list->setModel(proxyModel);
}
