/**
 * 叶海辉
 * QQ群121376426
 * http://blog.yundiantech.com/
 */

#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QThread>
#include <QImage>
#include <QDebug>
#include <QMutex>
#include <QPixmap>>
#include "advert.h"
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



class VideoPlayer : public QThread
{
    Q_OBJECT

public:
    explicit VideoPlayer(QObject *parent = nullptr);
    ~VideoPlayer();
    void setFileName(QString path){mFileName = path;}
    void init(QString myFileName, int myWidth, int myHeight);
    //void play();
    void stop();
    void run();
signals:
    void sig_GetOneFrame(int,int,QImage); //没获取到一帧图像 就发送此信号
    void sig_GetOneImage(int,int, int, int,QPixmap); //没获取到一帧图像 就发送此信号
private slots:
    void slotGetNetworkReply(QString Path);

private:
    QString curPath;
    QMutex mMutex;
    Advert curAd;
    NetworkRequest *mRequest;
    QString mFileName;
    std::thread m_decodecThread;
    AVFormatContext *pAVFormatCtx;
    AVCodecContext *pAVCodecCtx;
    SwsContext *pSwsCtx;
    uint8_t *pRgbBuffer;
    AVPacket packet;
    AVFrame *pAVFrame = NULL;
    AVFrame *pAVFrameRGB;
    int iVideoIndex = -1;
    QImage m_image;
    bool isFinish  =false;
    void decodec(int myX, int myY);
};

 static void VideoCallbackFun (void *data, void *data2);

#endif // VIDEOPLAYER_H
