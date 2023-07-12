#ifndef VIDEOOBJECT_H
#define VIDEOOBJECT_H

#include <QJsonObject>
#include <QString>



class VideoObject
{
public:
    VideoObject();
    VideoObject& operator=(const QJsonObject &A);
public:
    int videoNo;
    QString videoUrl;
    QString videoName;
    QString videoMD5;
};

#endif // VIDEOOBJECT_H
