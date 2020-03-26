#ifndef MENU_H
#define MENU_H
//include libraries
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QStandardItemModel>
#include <QMouseEvent>
#include <QHttpMultiPart>
#include <QFileDialog>
#include <QRegularExpression>
// include files
#include "sklad.h"
#include "zakaznaryad.h"
#include "cars_list.h"
#include "mainwindow.h"
#include "qjsontable.h"
#include "globals.h"
#include "settyre.h"
#include "addgsm.h"
#include "tyreforcar.h"
#include "opendopogfile.h"
#include "alarm.h"


namespace Ui {
class menu;
}

class menu : public QWidget
{
    Q_OBJECT

public:
    explicit menu(QWidget *parent = nullptr);
    ~menu();
    void setDataInLineEdits();
    void getDataFromDB();
    void clearDataJson();
    void connectManagerTo();
    void connectManagerHandbookTo();
    void connectManagerGSM();
private slots:
    //events
    void closeEvent(QCloseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    //remember func's
    void rememberVehiclesTyresData(QString jsonDoc);
    void rememberDopogAlarmData(QString jsonDoc);
    //load files
    void loadFileSTS(QString extension, QString filePath);
    void loadFilePTS(QString extension, QString filePath);
    //export jsons to CSV
    void exportJsonToCSVProbeg_Gsm(int jsArrayCount, QString pathToSaveCsvFile, QString jsonDoc);
    void exportJsonToCSVTyre(int jsArrayCount, QString pathToSaveCsvFile, QString jsonDoc);
    void exportJsonToCSVRemont(int jsArrayCount,QString pathToSaveCsvFile, QString jsonDoc);
    // check func's
    int getPutOrPost();
    bool isKartaTcFull();
    void isUserAdmin();
    //filter func's for search
    void filterModelTyres();
    void filterModelRemont();
    //set images func's
    void imgSearchTyres();
    void imgSearchRemont();
    void imgSearchTO();
    //click events
    void on_btnSklad_clicked();
    void on_zakazNaryad_clicked();
    void on_btnReturn_clicked();
    void on_btnChange_clicked();
    void on_btnOk_clicked();
    void on_tcForRezina_clicked(const QModelIndex &index);
    void on_setRezinaForTc_clicked();
    void on_btnAddOrderForRepair_clicked();
    void on_btnAddGsm_clicked();
    void on_btnRefreshRemontTc_clicked();
    void on_btnRefreshRezina_clicked();
    void on_btnSearchRezina_clicked();
    void on_tableMileageKartaTC_clicked(const QModelIndex &index);
    void on_tableGsmKartaTC_clicked(const QModelIndex &index);
    void on_btnChangeGsm_clicked();
    void on_btnKartaTCRefresh_clicked();
    void on_btnDeleteGsm_clicked();
    void on_btnSearch_remont_clicked();
    void on_btnDwnldDopog_clicked();
    void on_btnLoadSTS_clicked();
    void on_btnOpenDopog_clicked();
    void on_btnLoadPTS_clicked();
    void on_btnDwnldPTS_clicked();
    void on_btnDwnldCTC_clicked();
    void on_btnSave_Probeg_clicked();
    void on_btnSave_tyres_clicked();
    void on_btnDeleteRemont_TC_clicked();
    void on_btnChangeRemont_TC_clicked();
    void on_btnSave_remont_clicked();
    void on_btnCancel_clicked();
    void on_remont_tc_clicked(const QModelIndex &index);
    void on_btnDeleteTyre_clicked();
    void on_tableRezinaShow_clicked(const QModelIndex &index);
    void on_tableAlarmTyres_doubleClicked(const QModelIndex &index);
    void on_remont_tabBarClicked(int index);
    //text_finished events
    void on_search_query_remont_textChanged(const QString &arg1);
    void on_searchTcRezina_textChanged(const QString &arg1);
    void on_searchTcRezina_editingFinished();
    void on_search_query_remont_editingFinished();
    //set visable/disable func's
    void setVisableButtons(bool isVisible);
    void setDisabledKartaTC(bool isEnabled);
    //connect manager func's
    void connectManagerCarsRezina();
    void connectManagerOrderForRepair();
    void connectManagerRezina();
    void connectManagerVehiclesAlarmTyres();
    void connectManagerCertificateDopogAlarm();
    //reply finished func's
    void replyFinished(QNetworkReply * reply);
    void replyRezinaFinished(QNetworkReply* reply);
    void replyCarsRezinaFinished(QNetworkReply* reply);
    void replyOrderForRepair(QNetworkReply* reply);
    void replyGsmFinished(QNetworkReply* reply);
    void replyDeleteMileagelistsFinished(QNetworkReply *reply);
    void replyLoadFinished(QNetworkReply*reply);
    void onDownloadStsFinished(QNetworkReply *reply);
    void onDownloadPtsFinished(QNetworkReply *reply);
    void onDownloadDopogFinished(QNetworkReply* reply);
    void replyGetProbegFromDbFinished(QNetworkReply * reply);
    void replyGetTyreFromDbFinished(QNetworkReply * reply);
    void replyDeleteOrderFinished(QNetworkReply * reply);
    void replyToFinished(QNetworkReply* reply);
    void replyHandbookToFinished(QNetworkReply* reply);
    void replyPostToFinished(QNetworkReply* reply);
    void replyPostHandbookToFinished(QNetworkReply* reply);
    void replyPutToFinished(QNetworkReply* reply);
    void replyPutToHandbookFinished(QNetworkReply* reply);
    void replyFinishedPostDataToDB(QNetworkReply* reply);
    void replyPostTyreToSkaldFinished(QNetworkReply* reply);
    void replyDeleteTyreShowFinished(QNetworkReply *reply);
    void replyGetRemontFromDbFinished(QNetworkReply * reply);
    void replyAlarmVehiclesTyresFinished(QNetworkReply* reply);
    void replyAlarmDopogFinished(QNetworkReply* reply);
    //put func's
    void putToHandbook();
    void putTo();
    void putDataToDB();
    //post func's
    void postTO();
    void postHandbookTo();
    void postTyreToSklad();
    void postDataToDB();
    //get data from db
    void getProbegFromDb();
    void getTyreFromDb();
    void getRemontFromDb();
    //delete data from db
    void deleteMileageRequest();
    void deleteGsmRequest();
    void deleteOrderRequest();
    void deleteTyreShowRequest();
    //set tables
    void alarmTyresSetTable();
    void alarmDopogSetTable();
    //transform data to byte array
    QJsonObject transformDataToByteArrayTyreShow();
    //slots
     void slotReadyRead();
private:
    Ui::menu *ui;
    //pointers
    QNetworkAccessManager * manager;
    QJsonTableModel * rezinaCarsList;
    QJsonTableModel * rezinaCarsListShow;
    QJsonTableModel * orderForRepair;
    QJsonTableModel * gsmTable;
    QJsonTableModel * gsmMileageTable;
    QJsonTableModel * alarmTyres;
    QJsonTableModel * alarmDopog;
    QSortFilterProxyModel * proxyModel;
    QSortFilterProxyModel * proxyModelGsm;
    QSortFilterProxyModel * proxyModelSubTyre;
    QSortFilterProxyModel * proxyModelOrder;
    QSortFilterProxyModel * proxyModelMileage;
    QSortFilterProxyModel * proxyModelAlarmTyres;
    QSortFilterProxyModel * proxyModelAlarmDopog;
    //strings
    QString type_size ;
    QString name;
    QString date_of_setting ;
    QString mileage;
    QString pathToStsFile;
    QString pathToPtsFile;
    QString pathToDopogFile;
    QString fileNameSts;
    QString fileNamePts;
    //byte arrays
    QByteArray jsonByteArrayTyreShowId;
    //json objects;
    QJsonObject indexSelectedCarTyreShow;
    QJsonObject indexSelectedCarRezina;
    //boolean func's
    bool isResinaTcSelected=false;
    bool isRezinaShow = false;
    bool isOrderSelected=false;
    bool isTyreShowSelected = false;
    int whichTableSelected;
    //save probeg CSV
    QString pathToSaveCsvFileProbeg;
    QString arrayNameOfProbeg[13] = {" Дата ","№ Путевого листа","Показания спидометра при выезде,км","Пройдено по городу,км","Пройдено по трассе,км","Показания спидометра при возвращении,км","Остаток ГСМ при выезде,л","Расход топлива по городу,л","Расход по трассе,л","Выдано ГСМ,л","Остаток ГСМ при возвращении","Расход топлива по городу(норма),л","Расход топлива по трассе(норма)"};
    QVector <QString> probegDateArray;
    QVector <QString> probegtrack_numberArray;
    QVector <QString> probegmileage_before_outArray;
    QVector <QString> probegon_cityArray;
    QVector <QString> probegon_traceArray;
    QVector <QString> probegmileage_after_inArray;
    QVector <QString> probegostatok_gsm_before_outArray;
    QVector <QString> probegfuel_consumption_in_cityArray;
    QVector <QString> probegfuel_consumption_on_traceArray;
    QVector <QString> probeggiven_gsmArray;
    QVector <QString> probegostatok_gsm_after_inArray;
    QVector <QString> probegnormal_fuel_consumption_in_cityArray;
    QVector <QString> probegnormal_fuel_consumption_on_traceArray;
    //save TYRE to CSV
    QString pathToSaveCsvFileTyre;
    QString tyre_headerArray[4] = {"Дата установки","Название","Типоразмер","Пробег на момент установки"};
    QVector <QString> tyre_type_sizeArray;
    QVector <QString> tyre_date_of_settingArray;
    QVector <QString> tyre_mileageArray;
    QVector <QString> tyre_nameArray;
    // save remont to CSV
    QString pathToSaveCsvFileRemont;
    QString remont_headerArray[6] = {"Дата документа","Номер заказ-наряда","Марка ТС","Гос. номер ТС ","Название контрагента ","Итоговая сумма по документу"};
    QVector <QString> remont_date_of_conclusion;
    QVector <QString> remont_order_number;
    QVector <QString> remont_contractor;
    QVector <QString> remont_sum;
    QVector <QString> remont_gos_number;
    QVector <QString> remont_brand;
    //array to create object vehicle
    QJsonArray array_tyreDataWithVehicleId;
    QJsonObject itemData;
    QJsonObject itemDataVehicle;
    //array to creat object dopog
    QJsonArray array_dopogDataWithId;
    QJsonObject dopogItemData;
    QJsonObject itemDataDopog;
signals:
    void carsList();
protected:
};

#endif // MENU_H
