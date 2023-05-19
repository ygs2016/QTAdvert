#ifndef LABELPICTURE_H
#define LABELPICTURE_H

#include <QJsonObject>
#include <QString>

class LabelPicture
{
public:
    LabelPicture();
    LabelPicture& operator=(const QJsonObject &A);
public:
    int x;
    int y;
    int width;
    int height;
    QString pictureUrl;
    QString pictureName;
};
#endif // LABELPICTURE_H
