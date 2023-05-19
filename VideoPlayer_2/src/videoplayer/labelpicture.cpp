#include "labelpicture.h"

#include <QJsonObject>

LabelPicture::LabelPicture()
{

}



LabelPicture& LabelPicture::operator=(const QJsonObject &A) {

    if(A.contains("X")){
        QJsonValue val = A.value("X");
        if(!val.isNull()){
            this->x = val.toInt();
        }
    }

    if(A.contains("Y")){
        QJsonValue val = A.value("Y");
        if(!val.isNull()){
            this->y = val.toInt();
        }
    }

    if(A.contains("Width")){
        QJsonValue val = A.value("Width");
        if(!val.isNull()){
            this->width = val.toInt();
        }
    }

    if(A.contains("Height")){
        QJsonValue val = A.value("Height");
        if(!val.isNull()){
            this->height = val.toInt();
        }
    }

    if(A.contains("PictureUrl")){
        QJsonValue val = A.value("PictureUrl");
        if(val.isString()){
            this->pictureUrl = val.toString();
        }
    }

    if(A.contains("PictureName")){
        QJsonValue val = A.value("PictureName");
        if(val.isString()){
            this->pictureName = val.toString();
        }
    }
    return *this;
}
