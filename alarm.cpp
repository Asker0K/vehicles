#include "alarm.h"

alarm::alarm(QObject *parent) : QObject(parent)
{

}

void alarm::connectManagerGetVehicles()
{
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/vehicles?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &alarm::replyGetVehiclesFinished);
}
void alarm::connectManagerGetTO()
{
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/to_s?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &alarm::replyGetTOFinished);
}
void alarm::connectManagerHandbook()
{
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/handbook?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &alarm::replyGetHandbookFinished);
}
void alarm::connectManagerGetTyres()
{
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/tyre?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &alarm::replyGetTyresFinished);
}
void alarm::connectManagerGetCertificateDopog()
{
    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://localhost:7777/v1/certificates?identitykey=66a726c41cdb93eaf1e1c8e1fd8338a9d6f353321a24255c48a0123bb0edf98a")));
    connect(manager, &QNetworkAccessManager::finished, this, &alarm::replyGetCertificateDopogFinished);
}




void alarm::replyGetVehiclesFinished(QNetworkReply* reply)
{
    if(reply->error())
    {
        qDebug() <<"errorALARM"<< reply->readAll();
        return;
    }
    QByteArray json = reply->readAll();
    qDebug() << "jsonALARM="<<json;
    QString jsonDoc(json);
    QString vehicleId = "id";
    QString vehicleMileage = "mileage";
    rememberVehicleData(jsonDoc,vehicleId,vehicleMileage);
    qDebug() <<"vehicle_id" <<vehicle_id;
    qDebug() <<"vehicle_mileage" <<vehicle_mileage;
    connectManagerGetTyres();
    reply->deleteLater();
}
void alarm::replyGetTyresFinished(QNetworkReply* reply)
{
    QByteArray json = reply->readAll();
    QString jsonDoc(json);
    QString tyreVehicleId = "vehicle_id";
    QString tyreMileage = "mileage";
    rememberTyreData(jsonDoc,tyreVehicleId,tyreMileage);
    appendMatrix();
    alarmLevelsMileageTyre();
    qDebug() << "idDiff=" <<idDiff;
    qDebug() << "mileageDiff=" <<mileageDiff;
    qDebug() << "tyreId="<<tyreId;
    reply->deleteLater();
}
void alarm::replyGetCertificateDopogFinished(QNetworkReply* reply)
{
    if(reply->error())
    {
        qDebug() <<"errorALARM"<< reply->readAll();
        return;
    }
    QByteArray json = reply->readAll();
    QString jsonDoc(json);
    rememberDopogData(jsonDoc);
    checkPeriodOfValidityDopog();
    qDebug() << "idDopog" << idDopog;
    qDebug() << "periodDopog" << periodDopog;
    reply->deleteLater();
}
void alarm::replyGetHandbookFinished(QNetworkReply* reply)
{
    if(reply->error())
    {
        qDebug() <<"errorALARM_Handbook"<< reply->readAll();
        return;
    }
    QByteArray json = reply->readAll();
    QString jsonDoc(json);
    rememberHandbookData(jsonDoc);
    compareGrm();
    compareTO();
    qDebug() <<"grmDiffMileage"<< grmDiffMileage;
    qDebug() <<"to_sDiffMileage"<<to_sDiffMileage;
    qDebug() << "period_between_to" <<period_between_to;
    qDebug() <<"period_between_grm" << period_between_grm;
    alarmLevelsToGrm();
    reply->deleteLater();
}
void alarm::replyGetTOFinished(QNetworkReply* reply)
{
    if(reply->error())
    {
        qDebug() <<"errorALARM_TO"<< reply->readAll();
        return;
    }
    QByteArray json = reply->readAll();
    QString jsonDoc(json);
    rememberToData(jsonDoc);
    connectManagerHandbook();
    reply->deleteLater();
}




void alarm::rememberVehicleData(QString jsonDoc, QString key1, QString key2)
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
                                    if(key==key1)
                                    {
                                        QString _key1 = obj[key].toString();
                                       vehicle_id.append(_key1);
                                    }
                                    if(key==key2)
                                    {
                                        QString _key2 = obj[key].toString();
                                        vehicle_mileage.append(_key2);
                                    }
                            }
                        }
                }
            }

}
void alarm::rememberTyreData(QString jsonDoc, QString key1, QString key2)
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
                                    if(key==key1)
                                    {
                                        QString _key1 = obj[key].toString();
                                       tyre_vehicle_id.append(_key1);
                                    }
                                    if(key==key2)
                                    {
                                        QString _key2 = obj[key].toString();
                                        tyre_mileage.append(_key2);
                                    }
                                    if(key=="id")
                                        tyre_id.append(obj[key].toString());
                            }
                        }
                }
            }

}
void alarm::rememberDopogData(QString jsonDoc)
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
                            if(key=="id")
                                idDopog.append(obj["id"].toString());
                            if(key=="period_of_validity")
                                periodDopog.append(obj["period_of_validity"].toString());
                        }
                    }
            }
        }

}
void alarm::rememberHandbookData(QString jsonDoc)
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
                        if(key=="id")
                            idHandbook.append(obj["id"].toString());
                        if(key=="period_between_to")
                            period_between_to.append(obj["period_between_to"].toString());
                        if(key=="period_between_grm_service")
                            period_between_grm.append(obj["period_between_grm_service"].toString());
                    }
                }
        }
    }
}
void alarm::rememberToData(QString jsonDoc)
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
                        if(key=="vehicle_id") idTo_s.append(obj["vehicle_id"].toString());
                        if(key=="mileage_for_next_to") mileage_for_next_to.append(obj["mileage_for_next_to"].toString());//пробег на момент проведения ТО

                    }
                }
        }
    }
}


void alarm::compareTO()
{
  int mileageDiff;
    for (int i =0; i<idTo_s.length();i++)
    {
        mileageDiff = mileage_for_next_to[i].toInt()+period_between_to[i].toInt();
        to_sDiffMileage.append(mileageDiff);
    }
}
void alarm::compareGrm()
{
  int mileageDiff;
    for (int i =0; i<idTo_s.length();i++)
    {
        mileageDiff = mileage_for_next_to[i].toInt()+period_between_grm[i].toInt();
        grmDiffMileage.append(mileageDiff);
    }
}
void alarm::alarmLevelsToGrm()
{
  int tempDiff;
  for(int i =0;i<to_sDiffMileage.length();i++)
    {
      tempDiff=to_sDiffMileage[i]-vehicle_mileage[i].toInt();
      if(tempDiff>=5000) {
          errorToId.append(0);
          toWithAlarmId.append(idTo_s[i]);
        }
      if(tempDiff<5000&&tempDiff>=1000){
          errorToId.append(1);
          toWithAlarmId.append(idTo_s[i]);
        }
      if(tempDiff<1000) {
          errorToId.append(2);
          toWithAlarmId.append(idTo_s[i]);
        }
    }
  int tempDiffGrm;
  for(int i =0;i<grmDiffMileage.length();i++)
    {
      tempDiffGrm=grmDiffMileage[i]-vehicle_mileage[i].toInt();
      if(tempDiffGrm>=5000) {
          errorGrmId.append(0);
          grmWithAlarmId.append(idTo_s[i]);
        }
      if(tempDiffGrm<5000&&tempDiff>=1000){
          errorGrmId.append(1);
          grmWithAlarmId.append(idTo_s[i]);
        }
      if(tempDiffGrm<1000) {
          errorGrmId.append(2);
          grmWithAlarmId.append(idTo_s[i]);
        }
    }
}

void alarm::alarmLevelsMileageTyre()
{
    qDebug() <<"i am in alarm";
    qDebug() <<mileageDiff.length();
   for(int i =0;i<mileageDiff.length();i++)
   {
       if(mileageDiff[i]<50000) {
           errorTyreId.append(0);
           qDebug() <<"errorTyreId="<<errorTyreId;
           tyreWithAlarmId.append(tyreId[i]);
       }
       if(mileageDiff[i]<70000&&mileageDiff[i]>=50000){
           errorTyreId.append(1);
           qDebug() <<"errorTyreId="<<errorTyreId;
           tyreWithAlarmId.append(tyreId[i]);
       }
       if(mileageDiff[i]>=70000) {
           errorTyreId.append(2);
           qDebug() <<"errorTyreId="<<errorTyreId;
           tyreWithAlarmId.append(tyreId[i]);
       }
   }
}
void alarm::appendMatrix()
{
     int temp_veh_mileage;
     int temp_tyre_mileage;
     QString temp_veh_id;
     int temp_diff;

    for(int i =0;i<vehicle_id.length();i++)
    {
        temp_veh_id= vehicle_id[i];
        for(int j=0;j<tyre_vehicle_id.size();j++)
        {
            if(tyre_vehicle_id.at(j)==temp_veh_id)
            {
                temp_veh_mileage = vehicle_mileage[i].toInt();
                temp_tyre_mileage = tyre_mileage[j].toInt();

                temp_diff=temp_veh_mileage-temp_tyre_mileage;
                tyreId.append(tyre_id[i]);
                idDiff.append(vehicle_id[i]);
                mileageDiff.append(temp_diff);
            }
        }
    }
}
void alarm::checkPeriodOfValidityDopog()
{
    QDate currentDate= QDate::currentDate();
    for(int i=0;i<idDopog.length();i++)
    {
        QDate dt = QDate::fromString(periodDopog[i],"dd.MM.yyyy");
        if(currentDate>= dt)
        {
            dopogWithAlarmId.append(idDopog[i]);
            errorDopogId.append(2);
        }
        if(currentDate.daysTo(dt) <=30&&currentDate.daysTo(dt) >0)
        {
            dopogWithAlarmId.append(idDopog[i]);
            errorDopogId.append(1);
        }
        if(currentDate.daysTo(dt) >30)
        {
            dopogWithAlarmId.append(idDopog[i]);
            errorDopogId.append(0);
        }
    }
    qDebug() << "errorDopogId=" <<errorDopogId;
    qDebug() << "dopogWithAlarmId=\n" << dopogWithAlarmId;
}
