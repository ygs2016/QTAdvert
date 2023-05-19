#include "advert.h"
#include "videoplayer/labelpicture.h"
#include "videoplayer/labelvideo.h"
#include <QJsonDocument>
#include <QString>

Advert::Advert()
{

}



Advert& Advert::operator=(const QJsonObject &A) {

    if(A.contains("Id")){
        QJsonValue val = A.value("Id");
        if(val.isString()){
            this->id = val.toString();
        }
    }

    if(A.contains("ItemCode") ){
        QJsonValue val = A.value("ItemCode");
        if(val.isString()){
            this->itemCode = val.toString();
        }
    }

    if(A.contains("ItemName") ){
        QJsonValue val = A.value("ItemName");
        if(val.isString()){
            this->itemName = val.toString();
        }
    }

    if(A.contains("LabelPicture") ){
        QJsonValue val = A.value("LabelPicture");
        if(val.isObject()){
            this->labelPicture = val.toObject();
        }
    }

    if(A.contains("LabelVideo") ){
        QJsonValue val = A.value("LabelVideo");
        if(val.isObject()){
            this->labelVideo = val.toObject();
        }
    }

    if(A.contains("Nlast") ){
        QJsonValue val = A.value("Nlast");
        if(!val.isNull()){
            this->nlast = val.toInt();
        }
    }

    QJsonDocument doc(A);
    rawJson = QString(doc.toJson(QJsonDocument::Indented));

    return *this;
}

QString Advert::getJson(){
    return rawJson;
}

