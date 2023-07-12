#include "hwplayer.h"
#include "videoplayer/networkrequest.h"
#include <QJsonDocument>
#include <QFile>
#include <stdio.h>
#include <QPixmap>
extern "C"
{
#include "hwplayer/mainentry.h"
}

hwplayer::hwplayer(QObject *parent)
{
#ifndef PC_TEST
    play_video_init_hw();
#endif
    connect(&mRequest,SIGNAL(sig_GetNetworkReply(QString)),this,SLOT(slotGetNetworkReply(QString )));
    mRequest.start();
}

hwplayer::~hwplayer()
{
//    if(mRequest != NULL){
//        delete mRequest;
//    }
#ifndef PC_TEST
    play_video_uninit_hw();
#endif
}



void hwplayer::slotGetNetworkReply(QString Path)
{
#if 1
    qDebug() << Path;
    Advert tmpAd;
    QString jsonFile = Path + "/JsonRawData.txt";
    qDebug() << jsonFile;
    if(QFile().exists(jsonFile)){
        QFile file(jsonFile);
        bool bOpen = file.open(QIODevice::ReadOnly);
        if (bOpen == false)
        {
            return;
        }
        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (!doc.isObject())
        {
            return;
        }

        QJsonObject obj = doc.object();
        tmpAd = obj;
        mMutex.lock();
        if(curAd.id.compare(tmpAd.id)){
            QFile(curPath + "/JsonRawData.txt").remove();
            curAd = obj;
            curPath = Path;
        }
        mMutex.unlock();
    }
#endif
}


void hwplayer::run(){
    QString oldID;
    QString fileName;
    int x;
    int y;
    int width;
    int heigh;
    int index;
    bool canPlay = false;
    VideoObject vObj;
    while(1){
        mMutex.lock();
        canPlay = false;
        //printf("curAd.id.isNull() ==> %d, curAd.id.isEmpty() ==> %d\n",curAd.id.isNull(), curAd.id.isEmpty() );
        if(!curAd.id.isNull() && !curAd.id.isEmpty()){
            canPlay = true;
            if(curAd.id.compare(oldID)){
                oldID = curAd.id;
                x = curAd.labelVideo.x;
                y = curAd.labelVideo.y;
                heigh = curAd.labelVideo.height;
                width = curAd.labelVideo.width;
                index = 0;
                QPixmap map = QPixmap(curPath + "/Picture/" + curAd.labelPicture.pictureName);
                map.scaled(curAd.labelPicture.width,curAd.labelPicture.height,Qt::IgnoreAspectRatio);
                emit sig_GetOneImage(curAd.labelPicture.x,curAd.labelPicture.y,curAd.labelPicture.width,curAd.labelPicture.height,map);
            }

            if(index >= curAd.labelVideo.VideoList.count()){
                index = 0;
            }
            if(curAd.labelVideo.VideoList.count() > 0){
                vObj = curAd.labelVideo.VideoList[index];
            }
            index++;
        }
        mMutex.unlock();
        if(canPlay){
            //init(curPath + "/Video/" + vObj.videoName, width, heigh);
            //decodec(x,y);
            emit sig_GetOneFrame(x,y,width,heigh);
            //sleep(1);
#ifndef PC_TEST
            play_video_play_hw(QString(curPath + "/Video/" + vObj.videoName).toLatin1().data(),x, y, width, heigh);
#endif
        }else{
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}
