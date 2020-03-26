#ifndef GLBALS_H
#define GLBALS_H
#include <QString>
#include <QJsonObject>
#include "qjsontable.h"
#include <QSortFilterProxyModel>

extern QString username;

extern QJsonObject objectIndexOfCarSelected;
extern QJsonObject objectIndexOfGsmCarSelected;
extern QJsonObject objectIndexOfMileageCarSelected;
extern QJsonObject objectIndexOfOrderCarSelected;
extern QJsonObject userJsonObject;

extern QJsonTableModel* episodesCarsList;
extern QSortFilterProxyModel *proxyModel;


extern bool isRowSelected;
extern bool isUserSelected;
extern bool isMileageSelected;
extern bool isGsmSelected;
extern bool isPut;
extern bool isPutTyre;

extern char putOrPost;
extern QByteArray  jsonByteArrayRezinaId;
extern QByteArray  jsonByteArrayId;
extern QByteArray userId;
extern QByteArray jsonByteArrayGsm;
extern QByteArray jsonByteArrayMileage;
extern QByteArray jsonByteArrayOrder;
extern QByteArray jsonIdSklad;

extern QString errId;
extern QString userRole;
extern QString dopogIdIs;
extern QString jsonByteArrayNewId;
extern QString gosNumberTC;
extern QString fileNameDopog;
extern QString idTO;

extern QVector <QString> tyreWithAlarmId;
extern QVector <int> errorTyreId;

extern QVector <QString> dopogWithAlarmId;
extern QVector <int> errorDopogId;

extern QVector <QString> toWithAlarmId;
extern QVector <int> errorToId;

extern QVector <QString> grmWithAlarmId;
extern QVector <int> errorGrmId;

#endif // GLBALS_H
