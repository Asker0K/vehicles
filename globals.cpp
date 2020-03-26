#include "globals.h"

QString username;//var for username

QJsonObject objectIndexOfCarSelected;//index of selected car
QJsonObject userJsonObject;
QJsonObject objectIndexOfGsmCarSelected;
QJsonObject objectIndexOfMileageCarSelected;
QJsonObject objectIndexOfOrderCarSelected;

QJsonTableModel* episodesCarsList;

bool isRowSelected; // bool check if row selected
bool isUserSelected; // check if user selected
bool isMileageSelected=false;//kartaTC---->mileageTable
bool isGsmSelected=false;// kartaTC------->gsmTable
bool isPut=false;
bool isPutTyre = true;

char putOrPost;//char u or o for get request put or post

QByteArray  jsonByteArrayId;//id of selected row in byte array in cars_list
QByteArray userId;//id of selected user
QByteArray  jsonByteArrayRezinaId;
QByteArray jsonByteArrayGsm;
QByteArray jsonByteArrayMileage;
QByteArray jsonByteArrayOrder;
QByteArray jsonIdSklad;

QString errId;//err id
QString dopogIdIs;
QString userRole;//user role
QString jsonByteArrayNewId;
QString gosNumberTC;
QString fileNameDopog;
QString idTO;

QVector <QString> tyreWithAlarmId;
QVector <int> errorTyreId;

QVector <QString> dopogWithAlarmId;
QVector <int> errorDopogId;

QVector <QString> toWithAlarmId;
QVector <int> errorToId;

QVector <QString> grmWithAlarmId;
QVector <int> errorGrmId;
