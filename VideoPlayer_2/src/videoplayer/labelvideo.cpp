#include "labelvideo.h"

#include <QJsonObject>
#include <QJsonArray>
#include <videoplayer/videoobject.h>

LabelVideo::LabelVideo()
{

}

LabelVideo& LabelVideo::operator=(const QJsonObject &A) {

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

    VideoObject obj;
    if(A.contains("VideoList")){
        QJsonValue val = A.value("VideoList");
        if(val.isArray()){
            QJsonArray arr = val.toArray();
            this->VideoList.clear();
            for(int i = 0; i < arr.count(); ++i){
                if(arr.at(i).isObject()){
                    obj = arr.at(i).toObject();
                    this->VideoList.append(obj);
                }
            }


        }
    }

    return *this;
}
