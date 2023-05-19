/**
 * 叶海辉
 * QQ群121376426
 * http://blog.yundiantech.com/
 */

#include "videoplayer.h"

#include <stdio.h>

VideoPlayer::VideoPlayer(QObject *parent)
{

}

VideoPlayer::~VideoPlayer()
{

}



void VideoPlayer::init()
{
    QByteArray ba = mFileName.toLatin1();
    const char * filename = ba.data();

    pAVFormatCtx = avformat_alloc_context();
    if (!pAVFormatCtx)
    {
        qDebug() <<"avformat_alloc_context error!\n";
        return;
    }

    //描述多媒体文件的构成及其基本信息
    if (avformat_open_input(&pAVFormatCtx, filename, NULL, NULL) != 0)
    {
        qDebug() <<"open file fail";
        avformat_free_context(pAVFormatCtx);
        return;
    }

    //读取一部分视音频数据并且获得一些相关的信息
    if (avformat_find_stream_info(pAVFormatCtx, NULL) < 0)
    {
        qDebug() <<"vformat find stream fail";
        avformat_close_input(&pAVFormatCtx);
        return;
    }
    // 根据解码器枚举类型找到解码器
    AVCodec *pAVCodec;
    int ret = av_find_best_stream(pAVFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, &pAVCodec, 0);
    if (ret < 0) {
        qDebug() << "av_find_best_stream faliture";
        avformat_close_input(&pAVFormatCtx);
        return;
    }
    iVideoIndex = ret;

    pAVCodec = avcodec_find_decoder(pAVFormatCtx->streams[iVideoIndex]->codecpar->codec_id);
    if (pAVCodec == NULL)
    {
        qDebug() <<"not find decoder";
        return;
    }

    qDebug() <<"avcodec_open2 pAVCodec->name:" << QString::fromStdString(pAVCodec->name);


    if(pAVFormatCtx->streams[iVideoIndex]->avg_frame_rate.den != 0) {
        float fps_ = pAVFormatCtx->streams[iVideoIndex]->avg_frame_rate.num / pAVFormatCtx->streams[iVideoIndex]->avg_frame_rate.den;
        qDebug() <<"fps:" << fps_;
    }
    int64_t video_length_sec_ = pAVFormatCtx->duration/AV_TIME_BASE;
    qDebug() <<"video_length_sec_:" << video_length_sec_;
    pAVCodecCtx = avcodec_alloc_context3(pAVCodec);
    if (pAVCodecCtx == NULL)
    {
        qDebug() <<"get pAVCodecCtx fail";
        avformat_close_input(&pAVFormatCtx);
        return;
    }
    ret = avcodec_parameters_to_context(pAVCodecCtx,pAVFormatCtx->streams[iVideoIndex]->codecpar);
    if (ret < 0)
    {
       qDebug() <<"avcodec_parameters_to_context fail";
        avformat_close_input(&pAVFormatCtx);
        return;
    }
    if (avcodec_open2(pAVCodecCtx, pAVCodec, NULL) < 0)
    {
        qDebug() <<"avcodec_open2 fail";
        return;
    }
    //为解码帧分配内存
    //AVFrame 存放从AVPacket中解码出来的原始数据
    pAVFrame = av_frame_alloc();
    pAVFrameRGB = av_frame_alloc();
    //用于视频图像的转换,将源数据转换为RGB32的目标数据
    pSwsCtx = sws_getContext(pAVCodecCtx->width, pAVCodecCtx->height, pAVCodecCtx->pix_fmt,
                             pAVCodecCtx->width, pAVCodecCtx->height, AV_PIX_FMT_RGB32,
                             SWS_BICUBIC, NULL, NULL, NULL);

    int  m_size = av_image_get_buffer_size(AVPixelFormat(AV_PIX_FMT_RGB32), pAVCodecCtx->width, pAVCodecCtx->height, 1);
    pRgbBuffer = (uint8_t *)(av_malloc(m_size));
    //为已经分配的空间的结构体AVPicture挂上一段用于保存数据的空间
    avpicture_fill((AVPicture *)pAVFrameRGB, pRgbBuffer, AV_PIX_FMT_BGR32, pAVCodecCtx->width, pAVCodecCtx->height);
    //av_image_fill_arrays
    //AVpacket 用来存放解码数据
    av_new_packet(&packet, pAVCodecCtx->width * pAVCodecCtx->height);
}

void VideoPlayer::play()
{
    m_decodecThread = std::thread([this]()
                                  {
                                      decodec();
                                  });
    m_decodecThread.detach();
}

void VideoPlayer::decodec()
{
    //读取码流中视频帧
    while (true)
    {
        int ret = av_read_frame(pAVFormatCtx, &packet);
        if(ret != 0)
        {
           qDebug()<<"file end";
            isFinish = !isFinish;
            return;
        }
        if (packet.stream_index != iVideoIndex)
        {
            av_packet_unref(&packet);
            continue;
        }
        int iGotPic = AVERROR(EAGAIN);
        //             //解码一帧视频数据
        iGotPic = avcodec_send_packet(pAVCodecCtx, &packet);
        if(iGotPic!=0){
           qDebug()<<"avcodec_send_packet error";
            continue;
        }
        iGotPic = avcodec_receive_frame(pAVCodecCtx, pAVFrame);
        if(iGotPic == 0){
            //转换像素
            sws_scale(pSwsCtx, (uint8_t const * const *)pAVFrame->data, pAVFrame->linesize, 0, pAVCodecCtx->height, pAVFrameRGB->data, pAVFrameRGB->linesize);

            //构造QImage
            QImage img(pRgbBuffer, pAVCodecCtx->width, pAVCodecCtx->height, QImage::Format_RGB32);
            //qDebug()<<"decode img";
            m_image = img;
            emit sig_GetOneFrame(m_image);
        }else {
           qDebug()<<"decode error";
        }

        av_packet_unref(&packet);
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }
    //资源回收
    av_free(pAVFrame);
    av_free(pAVFrameRGB);
    sws_freeContext(pSwsCtx);
    avcodec_close(pAVCodecCtx);
    avformat_close_input(&pAVFormatCtx);
}

