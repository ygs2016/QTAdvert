#ifndef LABELVIDEO_H
#define LABELVIDEO_H

#include "videoplayer/videoobject.h"
#include <QJsonObject>
#include <QList>

class LabelVideo
{
public:
    LabelVideo();
    LabelVideo& operator=(const QJsonObject &A);
public:
    int x;
    int y;
    int width;
    int height;
    QList<VideoObject> VideoList;
};

#endif // LABELVIDEO_H
