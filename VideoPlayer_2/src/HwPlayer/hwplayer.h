#ifndef HWPLAYER_H
#define HWPLAYER_H

#include <QThread>
#include <QDebug>
#include <QMutex>
#include "videoplayer/advert.h"
#include "videoplayer/networkrequest.h"

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/pixfmt.h"
#include "libswscale/swscale.h"
//#include "libavdevice/avdevice.h"
#include <libavutil/pixdesc.h>
//#include <libavutil/hwcontext.h>
#include <libavutil/opt.h>
#include <libavutil/avassert.h>
#include <libavutil/imgutils.h>
}



class hwplayer : public QThread
{
    Q_OBJECT

public:
    explicit hwplayer(QObject *parent = nullptr);
    ~hwplayer();
    void setFileName(QString path){mFileName = path;}
    void run();
signals:
//    void sig_GetOneFrame(int,int, int, int); //没获取到一帧图像 就发送此信号
//    void sig_GetOneImage(int,int, int, int,QPixmap); //没获取到一帧图像 就发送此信号
private slots:
    void slotGetNetworkReply(QString Path);

private:
    bool isRunning = false;
    QString curPath;
    QMutex mMutex;
    Advert curAd;
    NetworkRequest mRequest;
    QString mFileName;
    std::thread m_decodecThread;
    int iVideoIndex = -1;
 // QImage m_image;
    bool isFinish  =false;
};

#endif // HWPLAYER_H
