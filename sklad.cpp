#include "sklad.h"
#include "ui_sklad.h"

sklad::sklad(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sklad)
{
    ui->setupUi(this);
    imgIndicatorSklad();

    connectManagerSklad();
}

sklad::~sklad()
{
    delete ui;
}
void sklad::imgIndicatorSklad()
{
    QPixmap pix(":/resource/img/search.png");
    int w = ui->searchIconSklad->width();
    int h = ui->searchIconSklad->height();
    ui->searchIconSklad->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
}

void sklad::mousePressEvent(QMouseEvent *event)
{
  if(event->buttons()==Qt::LeftButton)
    {
        ui->searchQuerySklad->clear();
        ui->searchQuerySklad->setFocus();
        ui->searchIconSklad->show();
        on_btnSearch_clicked();
    }
}

void sklad::on_btnReturn_clicked()
{
    this->close();
}

void sklad::on_btnAdd_clicked()
{
        isPutTyre = false;
        setTyre *tyre = new setTyre();
        tyre->show();
}

void sklad::filterModel()
{
    /*
    QString searchValue = ui->searchQuerySklad->text();
    proxyModel->setFilterRegExp(QRegExp(searchValue, Qt::CaseInsensitive, QRegExp::FixedString));
    proxyModel->setFilterKeyColumn(1);
    ui->tableSklad->setModel(proxyModelOrder);
    */
}

void sklad::connectManagerSklad()
{
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/tyre/free?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &sklad::replyFinishedSklad);
}

void sklad::replyFinishedSklad(QNetworkReply* reply)
{
    /************************creating QJsonTable with filling data in it******************************/
            QJsonTableModel::Header headerSklad;
            headerSklad.push_back( QJsonTableModel::Heading( { {"title","id"},    {"index","id"} }) );
            headerSklad.push_back( QJsonTableModel::Heading( { {"title","Название"},   {"index","name"} }) );
            headerSklad.push_back( QJsonTableModel::Heading( { {"title","Типоразмер"},  {"index","type_size"} }) );
            headerSklad.push_back( QJsonTableModel::Heading( { {"title","Пробег"},  {"index","mileage"} }) );
            headerSklad.push_back( QJsonTableModel::Heading( { {"title","Дата установки"},  {"index","date_of_setting"} }) );

            tableSklad = new QJsonTableModel( headerSklad, this );

            proxyModel = new QSortFilterProxyModel(this);
            proxyModel->setSourceModel(tableSklad);

            ui->tableSklad->setModel( proxyModel );
            ui->tableSklad->setSortingEnabled(true);
            ui->tableSklad->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->tableSklad->setSelectionMode(QAbstractItemView::SingleSelection);
            ui->tableSklad->setColumnHidden(0,true);
            ui->tableSklad->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
/*****************************************************************************************/
            QByteArray json = reply->readAll();
            QJsonDocument jsonDocument = QJsonDocument::fromJson( json );
            tableSklad->setJson( jsonDocument );
            qDebug() <<"JsonDocument:"<<jsonDocument;
/*****************************************************************************************/
            reply->deleteLater();
            qDebug() << "model tableSklad created|\n ";
}

void sklad::on_btnDelete_clicked()
{
    if(isSkladRowSelected==true)
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

    on_btnRefresh_clicked();
}


void sklad::deleteRequest()
{
    manager = new QNetworkAccessManager();
    manager->deleteResource(QNetworkRequest(QUrl("http://localhost:7777/v1/tyre/"+jsonIdSklad+"?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &sklad::replyDeleteFinished);
}

void sklad::replyDeleteFinished(QNetworkReply* reply)
{
    qDebug()<<reply->readAll();
    reply->deleteLater();
}

void sklad::on_tableSklad_clicked(const QModelIndex &index)
{
    QModelIndex index2 = proxyModel->mapToSource(index);
    objectIndexOfSkladSelected = tableSklad->getJsonObject( index2 );
    qDebug() << objectIndexOfSkladSelected;
    qDebug() << objectIndexOfSkladSelected["id"];

    jsonIdSklad = objectIndexOfSkladSelected["id"].toString().toUtf8();//convert QJsonObject into QbyteArray
    isSkladRowSelected = true;
}

void sklad::on_btnRefresh_clicked()
{
    ui->searchQuerySklad->clear();
    ui->searchQuerySklad->setFocus();
    ui->searchIconSklad->show();
    isSkladRowSelected = false;
    connectManagerSklad();
    qDebug()<<"\nsklad refreshed!\n";
}

void sklad::on_btnSearch_clicked()
{
    filterModel();
}

void sklad::on_searchQuerySklad_textChanged(const QString &arg1)
{
    if(arg1!='\0')
        ui->searchIconSklad->hide();
}

void sklad::on_searchQuerySklad_editingFinished()
{
    on_btnSearch_clicked();
}

void sklad::on_btnEdit_clicked()
{
    isPutTyre = true;
    setTyre *tyre = new setTyre();
    tyre->show();
}
