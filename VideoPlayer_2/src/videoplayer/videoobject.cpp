#include "videoobject.h"

#include <QJsonObject>

VideoObject::VideoObject()
{
    printf("test VideoObject\n");
}



VideoObject& VideoObject::operator=(const QJsonObject &A) {

    if(A.contains("VideoNo")){
        QJsonValue val = A.value("VideoNo");
        if(!val.isNull()){
            this->videoNo = val.toInt();
        }
    }

    if(A.contains("VideoUrl")){
        QJsonValue val = A.value("VideoUrl");
        if(val.isString()){
            this->videoUrl = val.toString();
        }
    }

    if(A.contains("VideoName")){
        QJsonValue val = A.value("VideoName");
        if(val.isString()){
            this->videoName = val.toString();
        }
    }

    if(A.contains("VideoMD5")){
        QJsonValue val = A.value("VideoMD5");
        if(val.isString()){
            this->videoMD5 = val.toString();
        }
    }
    return *this;
}
