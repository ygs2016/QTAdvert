#ifndef ADVERT_H
#define ADVERT_H

#include "videoplayer/labelpicture.h"
#include "videoplayer/labelvideo.h"
#include <QJsonObject>
#include <QString>



class Advert
{

public:
    Advert();
    Advert& operator=(const QJsonObject &A) ;
public:
    QString getJson();
    QString rawJson;
    QString id;
    QString itemCode;
    QString itemName;
    LabelPicture labelPicture;
    LabelVideo labelVideo;
    int nlast;
    QString CurrentDir;
};

#endif // ADVERT_H
