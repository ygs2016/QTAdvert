#ifndef VIDEOPLAYERHW_H
#define VIDEOPLAYERHW_H

#include <QThread>
#include <QImage>
#include <QDebug>
#include <QMutex>
#include <QPixmap>
#include "advert.h"
#include "videoplayer/networkrequest.h"

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/pixfmt.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include <libavutil/time.h>
//#include "libavdevice/avdevice.h"
#include <libavutil/pixdesc.h>
//#include <libavutil/hwcontext.h>
#include <libavutil/opt.h>
#include <libavutil/avassert.h>
#include <libavutil/imgutils.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <assert.h>

//sstar sdk lib
#include "mi_common.h"
#include "mi_common_datatype.h"
#include "mi_sys.h"
#include "mi_sys_datatype.h"
#include "mi_ao.h"
#include "mi_ao_datatype.h"
#include "mi_vdec.h"
#include "mi_vdec_datatype.h"
#include "mi_hdmi.h"
#include "mi_hdmi_datatype.h"
#include "mi_disp.h"
#include "mi_disp_datatype.h"
#include "mi_panel.h"
#include "mi_panel_datatype.h"


#ifdef ST_DEFAULT_SOC_ID
#undef ST_DEFAULT_SOC_ID
#define ST_DEFAULT_SOC_ID 0
#else
#define ST_DEFAULT_SOC_ID 0
#endif

//sdk audio input/outout param
#define     AUDIO_INPUT_SAMPRATE        48000
#define     AUDIO_INPUT_CHLAYOUT        AV_CH_LAYOUT_MONO
#define     AUDIO_INPUT_SAMPFMT         AV_SAMPLE_FMT_S16

#define     AUDIO_OUTPUT_SAMPRATE       E_MI_AUDIO_SAMPLE_RATE_48000
#define     AUDIO_OUTPUT_CHLAYOUT       E_MI_AUDIO_SOUND_MODE_MONO
#define     AUDIO_OUTPUT_SAMPFMT        E_MI_AUDIO_BIT_WIDTH_16

#define     MI_AUDIO_MAX_DATA_SIZE      25000

#ifdef SUPPORT_HDMI
#ifdef CHIP_IS_SSD20X
#define AUDIO_DEV   3
#else
#define AUDIO_DEV   4
#endif
#else
#define AUDIO_DEV   0
#endif

//func return value
#define     SUCCESS     0
#define     FAIL        1

#define STCHECKRESULT(result)\
if (result != MI_SUCCESS)\
    {\
            printf("[%s %d]exec function failed\n", __FUNCTION__, __LINE__);\
    }\
    else\
{\
        printf("(%s %d)exec function pass\n", __FUNCTION__,__LINE__);\
}

#define ExecFunc(func, _ret_) \
printf("%d Start test: %s\n", __LINE__, #func);\
    if (func != _ret_)\
{\
        printf("AO_TEST [%d] %s exec function failed\n",__LINE__, #func);\
}\
    else\
{\
        printf("AO_TEST [%d] %s  exec function pass\n", __LINE__, #func);\
}\
    printf("%d End test: %s\n", __LINE__, #func);

#define MI_AUDIO_SAMPLE_PER_FRAME 4096

#define WAV_G711A 0x06
#define WAV_G711U 0x7
#define WAV_G726 0x45
#define WAV_PCM  0x1

#define G726_16 2
#define G726_24 3
#define G726_32 4
#define G726_40 5

#define DMA_BUF_SIZE_8K (8000)
#define DMA_BUF_SIZE_16K    (16000)
#define DMA_BUF_SIZE_32K    (32000)
#define DMA_BUF_SIZE_48K    (48000)
/*=============================================================*/
// Global Variable definition
/*=============================================================*/
// WAVE file header format
typedef struct _WavHeader_s{
    MI_U8   riff[4];                // RIFF string
    MI_U32  ChunkSize;              // overall size of file in bytes
    MI_U8   wave[4];                // WAVE string
    MI_U8   fmt_chunk_marker[4];    // fmt string with trailing null char
    MI_U32  length_of_fmt;          // length of the format data
    MI_U16  format_type;            // format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law
    MI_U16  channels;               // no.of channels
    MI_U32  sample_rate;            // sampling rate (blocks per second)
    MI_U32  byterate;               // SampleRate * NumChannels * BitsPerSample/8
    MI_U16  block_align;            // NumChannels * BitsPerSample/8
    MI_U16  bits_per_sample;        // bits per sample, 8- 8bits, 16- 16 bits etc
    MI_U8   data_chunk_header [4];  // DATA string or FLLR string
    MI_U32  data_size;              // NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
}_WavHeader_t;

typedef struct WAVE_FORMAT
{
    signed short wFormatTag;
    signed short wChannels;
    unsigned int  dwSamplesPerSec;
    unsigned int  dwAvgBytesPerSec;
    signed short wBlockAlign;
    signed short wBitsPerSample;
} WaveFormat_t;

typedef struct WAVEFILEHEADER
{
    char chRIFF[4];
    unsigned int  dwRIFFLen;
    char chWAVE[4];
    char chFMT[4];
    unsigned int  dwFMTLen;
    WaveFormat_t wave;
    char chDATA[4];
    unsigned int  dwDATALen;
} WaveFileHeader_t;

typedef enum
{
    E_SOUND_MODE_MONO =0, /* mono */
    E_SOUND_MODE_STERO =1, /* stereo */
}SoundMode_e;

typedef enum
{
    E_SAMPLE_RATE_8000 =8000, /* 8kHz sampling rate */
    E_SAMPLE_RATE_16000 =16000, /* 16kHz sampling rate */
    E_SAMPLE_RATE_32000 =32000, /* 32kHz sampling rate */
    E_SAMPLE_RATE_48000 =48000, /* 48kHz sampling rate */
}SampleRate_e;

typedef enum
{
    E_ANEC_TYPE_G711A = 0,
    E_ANEC_TYPE_G711U,
    E_AENC_TYPE_G726_16,
    E_AENC_TYPE_G726_24,
    E_AENC_TYPE_G726_32,
    E_AENC_TYPE_G726_40,
    PCM,
}AencType_e;

typedef struct AVPacketQueue {
    AVPacketList *pkt;
    AVPacketList *next_pkt;
    int nb_packets;
    int abort_request;
    int size;
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
}AVPacketQueue;

typedef struct AVPlayer {
    AVFormatContext *format_ctx;
    AVCodecContext *v_codec_ctx;
    AVCodecContext *a_codec_ctx;
    AVPacketQueue v_pkt_queue;
    AVPacketQueue a_pkt_queue;
    int video_stream_idx;
    int audio_stream_idx;
    double audio_clock;
    double audio_pts;
    double video_clock;
    double video_pts;
}AVPlayer;

#define ALIGN_BACK(x, a)            (((x) / (a)) * (a))
#define DISP_WIDTH_ALIGN    2
#define DISP_HEIGHT_ALIGN   2

#define NALU_PACKET_SIZE            512*1024
#define MI_U32VALUE(pu8Data, index) (pu8Data[index]<<24)|(pu8Data[index+1]<<16)|(pu8Data[index+2]<<8)|(pu8Data[index+3])

#define MAKE_YUYV_VALUE(y,u,v) ((y) << 24) | ((u) << 16) | ((y) << 8) | (v)
#define YUYV_BLACK MAKE_YUYV_VALUE(0,128,128)
#define YUYV_WHITE MAKE_YUYV_VALUE(255,128,128)
#define YUYV_RED MAKE_YUYV_VALUE(76,84,255)
#define YUYV_GREEN MAKE_YUYV_VALUE(149,43,21)
#define YUYV_BLUE MAKE_YUYV_VALUE(29,225,107)


#define SAVE_YUV_TO_FILE 0
}

class VideoPlayerHw : public QThread
{
    Q_OBJECT

public:
    explicit VideoPlayerHw(QObject *parent = nullptr);
    ~VideoPlayerHw();
    void init(QString myFileName, int myWidth, int myHeight);;
    void stop();
    void run();
    static void sendMsg(QImage);
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
    static VideoPlayerHw *myPlayer;
    void decodec(QString myFileName, int myWidth, int myHeight, int myX, int myY);


//    int add_wave_header(WaveFileHeader_t* tWavHead, AencType_e eAencType, SoundMode_e eSoundMode, SampleRate_e eSampleRate, int raw_len);
//    MI_S32 sstar_sys_init(void);
//    MI_S32 sstar_sys_deinit(void);
//    int sstar_ao_deinit(void);
//    int sstar_ao_init(void);
//    MI_S32 sstar_module_unbind(void);
//    MI_S32 sstar_module_bind(void);
//    int sstar_sdk_init(void);
//    int sstar_sdk_deinit(void);
//    int sstar_packet_put(AVPacketQueue *q, AVPacket *pkt);
//    int sstar_packet_get(AVPacketQueue *q, AVPacket *pkt, int block);
//    void sstar_packet_flush(AVPacketQueue *q);
};


#endif // VIDEOPLAYERHW_H
