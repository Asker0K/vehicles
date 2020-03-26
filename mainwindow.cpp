#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cascadStyleSheet();
    imgLogoAuth();
    setWindowIcon(QIcon(":/resource/img/znak(transperent).png"));
    errString = new QString;
    alarm *a=new alarm();
    a->connectManagerGetVehicles();
    a->connectManagerGetCertificateDopog();
    a->connectManagerGetTO();
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
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

void MainWindow::imgLogoAuth()
{
    QPixmap pix(":/resource/img/znak(transperent).png");
    int w = ui->auth_image->width();
    int h = ui->auth_image->height();
    ui->auth_image->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
}
void MainWindow::cascadStyleSheet()
{
    QString cssMain;
    QFile fileMain(":/resource/css/style.css");
    if(fileMain.open(QIODevice::ReadOnly))
      {
        cssMain = fileMain.readAll();
        fileMain.close();
      }
    qApp->setStyleSheet(cssMain);
}

void MainWindow::replyFinished(QNetworkReply * reply)
{
        QByteArray json = reply->readAll();
        QJsonDocument jsonDocument = QJsonDocument::fromJson( json );

         qDebug()<<"Reply:"<< json;

        QJsonObject json_obj=jsonDocument.object();
        QVariantMap json_map = json_obj.toVariantMap();
    if(reply->error())
    {
        errId = json_map["status"].toString();
        qDebug()<<"json_map:"<< errId;
    }
    else
    {
            QMessageBox::information(this,"Успешно!","Добро пожаловать,\n"+username+"") ;
            qDebug() <<"success";
            userRole = json_map["Role"].toString();
            qDebug() << userRole;
                cars_list *cL = new cars_list();
                this->hide();
                cL->show();
    }
    reply->deleteLater();
}

void MainWindow::on_btnAuth_clicked()
{

    username=ui->loginLineEdit->text();
    QString pass=ui->passwdLineEdit->text();
    QJsonObject currentUser;

    currentUser[ "username" ] = username;
    currentUser[ "password" ] =  pass;

    QJsonDocument doc( currentUser);

    QByteArray strJson(doc.toJson());
    qDebug() <<"JSON:"<<strJson;

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
    QNetworkRequest request;
    request.setUrl(QUrl("http://localhost:7777/login"));
    request.setRawHeader("Content-Type", "application/json");
    QNetworkReply *reply = manager->post(request, strJson);

    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(slotError(QNetworkReply::NetworkError)));


}

void MainWindow::slotReadyRead()
{

}

void MainWindow::slotError(QNetworkReply::NetworkError err)
{
        QMessageBox::warning(this,"Неверный логин/пароль","Повторите попытку") ;
                             return;
}








