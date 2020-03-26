#include "userinfo.h"
#include "ui_userinfo.h"

userinfo::userinfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::userinfo)
{
    ui->setupUi(this);


}

userinfo::~userinfo()
{
    delete ui;
}

void userinfo::on_addUserBtn_clicked()
{
    addUser *add = new addUser();
    add->show();
}

void userinfo::on_delUserBtn_clicked()
{
    if(isUserSelected==true)
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
                      on_btnRefresh_clicked();
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


void userinfo::on_btnRefresh_clicked()
{
    isUserSelected=false;
    qDebug()<<"cars_list::refreshed";
    connectManagerUser();
}

void userinfo::on_btnChangeInfo_clicked()
{
    if(isUserSelected==true)
    {
        editUser *user = new editUser();

        user->show();
        user->connectManagerUser();
    }
    else
    {
      qDebug()<<" no Row selected"<<QMessageBox::information(this,"Не выбрана строка","Выберите строку!");
    }
}

void userinfo::connectManagerUser()
{
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/users?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &userinfo::replyFinished);
}

void userinfo::replyFinished(QNetworkReply* reply)
{
    /************************creating QJsonTable with filling data in it******************************/
            QJsonTableModel::Header headerUserInfo;
            headerUserInfo.push_back( QJsonTableModel::Heading( { {"title","id"},    {"index","id"} }) );
            headerUserInfo.push_back( QJsonTableModel::Heading( { {"title","Имя"},   {"index","username"} }) );
            headerUserInfo.push_back( QJsonTableModel::Heading( { {"title","Пароль"},  {"index","password"} }) );
            headerUserInfo.push_back( QJsonTableModel::Heading( { {"title","Роль"},  {"index","role"} }) );

            tableUserInfo = new QJsonTableModel( headerUserInfo, this );

            proxyModel = new QSortFilterProxyModel(this);
            proxyModel->setSourceModel( tableUserInfo);

            ui->usersList->setModel( proxyModel );
            ui->usersList->setSortingEnabled(true);
            ui->usersList->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->usersList->setSelectionMode(QAbstractItemView::SingleSelection);
            ui->usersList->setColumnHidden(0,true);
            ui->usersList->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
/*****************************************************************************************/
            QByteArray json = reply->readAll();
            QJsonDocument jsonDocument = QJsonDocument::fromJson( json );
            tableUserInfo->setJson( jsonDocument );
            findAdmin();

/*****************************************************************************************/
            reply->deleteLater();
}

void userinfo::on_usersList_clicked(const QModelIndex &index)
{
    QModelIndex index2 = proxyModel->mapToSource(index);
    userJsonObject = tableUserInfo->getJsonObject( index2 );
    qDebug() << userJsonObject;
    qDebug() << userJsonObject["id"];

    userId = userJsonObject["id"].toString().toUtf8();//convert QJsonObject into QbyteArray
    if(userJsonObject["role"].toString().toUtf8()=="super_admin")
    {
        ui->delUserBtn->setDisabled(true);
    }
    else
    {
        ui->delUserBtn->setDisabled(false);
    }
    /*******convert QJsonObject into QJsonDocument*****************/
    QJsonDocument doc(userJsonObject);
    QByteArray userId(doc.toJson(QJsonDocument::Compact)) ;
    qDebug() <<"success!" <<userId;
    QJsonDocument docObj = QJsonDocument::fromJson(userId);


    /*********************************************************/
    if(docObj.isNull())
    {
        qDebug()<<"Failed to create JSON doc.";
        isUserSelected = false;
    }
    if(!docObj.isObject())
    {
        qDebug()<<"JSON is not an object.";
        exit(3);
    }
    /*********************************************************/
     isUserSelected = true;
}

void userinfo::deleteRequest()
{
    manager = new QNetworkAccessManager();
    manager->deleteResource(QNetworkRequest(QUrl("http://localhost:7777/v1/users/byid/"+userId+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &userinfo::replyDeleteFinished);
}

void userinfo::replyDeleteFinished(QNetworkReply* reply)
{
    qDebug()<<reply->readAll();
    reply->deleteLater();
}


void userinfo::findAdmin()
{
    for(int row=0;row<proxyModel->rowCount();row++)
    {
            QModelIndex index =ui->usersList->model()->index(row,3);
            //QModelIndex index2 = proxyModel->mapToSource(index);
            QString text = index.data(Qt::DisplayRole).toString();
            if(userRole=="Admin")
            {
                if(text=="Admin"||text=="SuperAdmin" )
                {
                    qDebug() <<"ADMIN"<< text ;
                    ui->usersList->setRowHidden(row,true);

                }
            }
            qDebug() << "TEXT" << text ;
    }
}



