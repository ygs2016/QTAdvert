#include "videoplayer/videoplayerhw.h"
#include <QJsonDocument>
#include <QFile>
#include <stdio.h>
#include <QPixmap>


static bool b_exit = false;
MI_U16 u16OutWidth, u16OutHeight;
MI_U16 u16SrcWidth, u16SrcHeight;

VideoPlayerHw *VideoPlayerHw::myPlayer = NULL;

typedef void (*sighandler_t)(int);

static int add_wave_header(WaveFileHeader_t* tWavHead, AencType_e eAencType, SoundMode_e eSoundMode, SampleRate_e eSampleRate, int raw_len)
{
        tWavHead->chRIFF[0] = 'R';
        tWavHead->chRIFF[1] = 'I';
        tWavHead->chRIFF[2] = 'F';
        tWavHead->chRIFF[3] = 'F';

        tWavHead->chWAVE[0] = 'W';
        tWavHead->chWAVE[1] = 'A';
        tWavHead->chWAVE[2] = 'V';
        tWavHead->chWAVE[3] = 'E';

        tWavHead->chFMT[0] = 'f';
        tWavHead->chFMT[1] = 'm';
        tWavHead->chFMT[2] = 't';
        tWavHead->chFMT[3] = 0x20;
        tWavHead->dwFMTLen = 0x10;

        if(eAencType == E_ANEC_TYPE_G711A)
        {
            tWavHead->wave.wFormatTag = 0x06;
        }

        if(eAencType == E_ANEC_TYPE_G711U)
        {
            tWavHead->wave.wFormatTag = 0x07;
        }

        if(eAencType == E_ANEC_TYPE_G711U || eAencType == E_ANEC_TYPE_G711A)
        {
            if(eSoundMode == E_SOUND_MODE_MONO)
                tWavHead->wave.wChannels = 0x01;
            else
                tWavHead->wave.wChannels = 0x02;

            tWavHead->wave.wBitsPerSample = 8;//bitWidth;g711encode鍑烘潵鏄?bit锛岃繖閲岄渶瑕佸啓姝?        tWavHead->wave.dwSamplesPerSec = eSampleRate;
            tWavHead->wave.dwAvgBytesPerSec = (tWavHead->wave.wBitsPerSample  * tWavHead->wave.dwSamplesPerSec * tWavHead->wave.wChannels) / 8;
            tWavHead->wave.wBlockAlign = (tWavHead->wave.wBitsPerSample  * tWavHead->wave.wChannels) / 8;
        }
        else if(eAencType == PCM)
        {
            if(eSoundMode == E_SOUND_MODE_MONO)
                tWavHead->wave.wChannels = 0x01;
            else
                tWavHead->wave.wChannels = 0x02;

            tWavHead->wave.wFormatTag = 0x1;
            tWavHead->wave.wBitsPerSample = 16; //16bit
            tWavHead->wave.dwSamplesPerSec = eSampleRate;
            tWavHead->wave.dwAvgBytesPerSec = (tWavHead->wave.wBitsPerSample  * tWavHead->wave.dwSamplesPerSec * tWavHead->wave.wChannels) / 8;
            tWavHead->wave.wBlockAlign = 1024 ;
        }
        else //g726
        {
            tWavHead->wave.wFormatTag = 0x45;
            tWavHead->wave.wChannels  = 0x01;
            switch(eAencType)
            {
            case E_AENC_TYPE_G726_40:
                tWavHead->wave.wBitsPerSample = 5;
                tWavHead->wave.wBlockAlign =  5;
                break;
            case E_AENC_TYPE_G726_32:
                tWavHead->wave.wBitsPerSample = 4;
                tWavHead->wave.wBlockAlign =  4;
                break;
            case E_AENC_TYPE_G726_24:
                tWavHead->wave.wBitsPerSample = 3;
                tWavHead->wave.wBlockAlign =  3;
                break;
            case E_AENC_TYPE_G726_16:
                tWavHead->wave.wBitsPerSample = 2;
                tWavHead->wave.wBlockAlign =  2;
                break;
            default:
                printf("eAencType error:%d\n", eAencType);
                return -1;
            }

            tWavHead->wave.dwSamplesPerSec = eSampleRate;
            tWavHead->wave.dwAvgBytesPerSec = (tWavHead->wave.wBitsPerSample * tWavHead->wave.dwSamplesPerSec * tWavHead->wave.wChannels) / 8;
        }

        tWavHead->chDATA[0] = 'd';
        tWavHead->chDATA[1] = 'a';
        tWavHead->chDATA[2] = 't';
        tWavHead->chDATA[3] = 'a';
        tWavHead->dwDATALen = raw_len;
        tWavHead->dwRIFFLen = raw_len + sizeof(WaveFileHeader_t) - 8;

        return -1;
}

static MI_S32 sstar_sys_init(void)
{
        MI_SYS_Version_t stVersion;
        MI_U64 u64Pts = 0;

        STCHECKRESULT(MI_SYS_Init());
        memset(&stVersion, 0x0, sizeof(MI_SYS_Version_t));
        STCHECKRESULT(MI_SYS_GetVersion(&stVersion));
        STCHECKRESULT(MI_SYS_GetCurPts(&u64Pts));
        u64Pts = 0xF1237890F1237890;
        STCHECKRESULT(MI_SYS_InitPtsBase(u64Pts));
        u64Pts = 0xE1237890E1237890;
        STCHECKRESULT(MI_SYS_SyncPts(u64Pts));
        return MI_SUCCESS;
}

static MI_S32 sstar_sys_deinit(void)
{
        STCHECKRESULT(MI_SYS_Exit());
        return MI_SUCCESS;
}


static int sstar_ao_deinit(void)
{
        MI_AUDIO_DEV AoDevId = AUDIO_DEV;
        MI_AO_CHN AoChn = 0;

        /* disable ao channel of */
        ExecFunc(MI_AO_DisableChn(AoDevId, AoChn), MI_SUCCESS);
        /* disable ao device */
        ExecFunc(MI_AO_Disable(AoDevId), MI_SUCCESS);

        return 0;
}

static int sstar_ao_init(void)
{
        MI_AUDIO_Attr_t stSetAttr;
        MI_AUDIO_Attr_t stGetAttr;
        MI_AUDIO_DEV AoDevId = AUDIO_DEV;
        MI_AO_CHN AoChn = 0;

        MI_S32 s32SetVolumeDb;
        MI_S32 s32GetVolumeDb;

        //set Ao Attr struct
        memset(&stSetAttr, 0, sizeof(MI_AUDIO_Attr_t));
        stSetAttr.eBitwidth = AUDIO_OUTPUT_SAMPFMT;
        stSetAttr.eWorkmode = E_MI_AUDIO_MODE_I2S_MASTER;
        stSetAttr.u32FrmNum = 6;
        stSetAttr.u32PtNumPerFrm = MI_AUDIO_SAMPLE_PER_FRAME;
        stSetAttr.u32ChnCnt   = 1;
        stSetAttr.eSoundmode  = AUDIO_OUTPUT_CHLAYOUT;
        stSetAttr.eSamplerate = AUDIO_OUTPUT_SAMPRATE;

        /* set ao public attr*/
        ExecFunc(MI_AO_SetPubAttr(AoDevId, &stSetAttr), MI_SUCCESS);
        /* get ao device*/
        ExecFunc(MI_AO_GetPubAttr(AoDevId, &stGetAttr), MI_SUCCESS);
        /* enable ao device */
        ExecFunc(MI_AO_Enable(AoDevId), MI_SUCCESS);
        /* enable ao channel of device*/
        ExecFunc(MI_AO_EnableChn(AoDevId, AoChn), MI_SUCCESS);

        /* if test AO Volume */
        s32SetVolumeDb = -10;
        ExecFunc(MI_AO_SetVolume(AoDevId, s32SetVolumeDb), MI_SUCCESS);
        /* get AO volume */
        ExecFunc(MI_AO_GetVolume(AoDevId, &s32GetVolumeDb), MI_SUCCESS);
        return 0;
}


static MI_S32 sstar_module_unbind(void)
{
        MI_SYS_ChnPort_t stSrcChnPort;
        MI_SYS_ChnPort_t stDstChnPort;

        //bind vdec to disp
        stSrcChnPort.eModId    = E_MI_MODULE_ID_VDEC;
        stSrcChnPort.u32DevId  = 0;
        stSrcChnPort.u32ChnId  = 0;
        stSrcChnPort.u32PortId = 0;

        stDstChnPort.eModId    = E_MI_MODULE_ID_DISP;
        stDstChnPort.u32DevId  = 0;
        stDstChnPort.u32ChnId  = 0;
        stDstChnPort.u32PortId = 0;
        STCHECKRESULT(MI_SYS_UnBindChnPort(&stSrcChnPort, &stDstChnPort));
        STCHECKRESULT(MI_DISP_DisableInputPort(0, 0));

        return SUCCESS;
}


static MI_S32 sstar_module_bind(void)
{
        MI_SYS_ChnPort_t stSrcChnPort;
        MI_SYS_ChnPort_t stDstChnPort;

        //bind vdec to disp
        stSrcChnPort.eModId    = E_MI_MODULE_ID_VDEC;
        stSrcChnPort.u32DevId  = 0;
        stSrcChnPort.u32ChnId  = 0;
        stSrcChnPort.u32PortId = 0;

        stDstChnPort.eModId    = E_MI_MODULE_ID_DISP;
        stDstChnPort.u32DevId  = 0;
        stDstChnPort.u32ChnId  = 0;
        stDstChnPort.u32PortId = 0;
        STCHECKRESULT(MI_SYS_BindChnPort(&stSrcChnPort, &stDstChnPort, 30, 30));
        return SUCCESS;
}

static int sstar_sdk_init(void)
{
        //init system
        sstar_sys_init();

        //init audio
        sstar_ao_init();

        return 0;
}

static int sstar_sdk_deinit(void)
{
        sstar_ao_deinit();

        sstar_sys_deinit();

        return MI_SUCCESS;
}

sighandler_t signal(int signum, sighandler_t handler);

static int sstar_packet_put(AVPacketQueue *q, AVPacket *pkt)
{
        AVPacketList *pkt_list;

        pkt_list = (AVPacketList *)av_mallocz(sizeof(AVPacketList));
        if (!pkt_list)
            return AVERROR(ENOMEM);

        pthread_mutex_lock(&q->mutex);

        pkt_list->pkt = *pkt;
        pkt_list->next = NULL;

        if (!q->next_pkt) {
            q->pkt = pkt_list;
        } else {
            q->next_pkt->next = pkt_list;
        }
        q->next_pkt = pkt_list;
        q->nb_packets++;
        q->size += pkt_list->pkt.size + sizeof(*pkt_list);

        pthread_cond_signal(&q->cond);
        pthread_mutex_unlock(&q->mutex);

        return 0;
}

static int sstar_packet_get(AVPacketQueue *q, AVPacket *pkt, int block)
{
        AVPacketList *pkt_node;
        int ret = -1;
        pthread_mutex_lock(&q->mutex);
        while (1)
        {
            if(q->abort_request) {
                ret = -1;
                break;
            }
            pkt_node = q->pkt;
            if (pkt_node) {
                q->pkt = pkt_node->next;
                if (!q->pkt) {
                    q->next_pkt = NULL;
                }
                q->nb_packets--;
                q->size -= pkt_node->pkt.size + sizeof(*pkt_node);
                *pkt = pkt_node->pkt;
                av_freep(&pkt_node);
                ret = 1;
                break;
            }
            else if (!block) {
                ret = 0;
                break;
            }
            else
            {
                //printf("no pkt wait pktnb: %d\n", q->nb_packets);
                pthread_cond_wait(&q->cond, &q->mutex);
            }
        }
        pthread_mutex_unlock(&q->mutex);
        return ret;
}

static void *sstar_packet_flush(AVPacketQueue *q)
{
        AVPacketList *pkt, *pkt1;

        pthread_mutex_lock(&q->mutex);
        for (pkt = q->pkt; pkt; pkt = pkt1) {
            pkt1 = pkt->next;
            av_packet_unref(&pkt->pkt);
            av_freep(&pkt);
        }
        q->pkt = NULL;
        q->next_pkt = NULL;
        q->nb_packets = 0;
        q->size = 0;
        pthread_mutex_unlock(&q->mutex);
}

static void *sstar_audio_process(void *args)
{
        int ret;
        AVPlayer *is = (AVPlayer *)args;
        uint8_t *audio_buffer = NULL;
        struct SwrContext *audio_swr_ctx = NULL;
        AVPacket *packet = av_packet_alloc();
        AVFrame *frame = av_frame_alloc();

        //输入的采样格式, 采样率, 声道布局
        enum AVSampleFormat in_sample_fmt = AV_SAMPLE_FMT_S16;
        int in_sample_rate = 48000;
        uint64_t in_ch_layout = AV_CH_LAYOUT_MONO;
        int in_channels = 0;
        unsigned int audio_frm_rwr_size = 0;
        //输出的采样格式, 采样率, 声道布局
        enum AVSampleFormat out_sample_fmt = AUDIO_INPUT_SAMPFMT;
        int out_sample_rate = AUDIO_INPUT_SAMPRATE;
        uint64_t out_ch_layout = AUDIO_INPUT_CHLAYOUT;
        int out_channels = av_get_channel_layout_nb_channels(out_ch_layout);
        int bytes_per_sec = av_samples_get_buffer_size(NULL, out_channels, out_sample_rate, out_sample_fmt, 1);

        AVRational tb = is->format_ctx->streams[is->audio_stream_idx]->time_base;

        while (!b_exit)
        {
            ret = sstar_packet_get(&is->a_pkt_queue, packet, 1);
            if(ret > 0)
            {
                if(packet->stream_index == is->audio_stream_idx)
                {
                    MI_AUDIO_Frame_t stAoSendFrame;
                    MI_S32 s32RetSendStatus = 0;
                    MI_AUDIO_DEV AoDevId = AUDIO_DEV;
                    MI_AO_CHN AoChn = 0;
                    MI_S16 s16DataLen = 0;
                    MI_U16 u16DataIndex = 0;
                    MI_S32 s32TimeOut = 128;

                    //解码AVPacket->AVFrame
                    ret = avcodec_send_packet(is->a_codec_ctx, packet);
                    if (ret < 0 && ret != AVERROR(EAGAIN))
                    {
                        av_log(NULL, AV_LOG_ERROR, "avcodec_send_packet fail!\n");
                        goto audio_exit;
                    }

                    ret = avcodec_receive_frame(is->a_codec_ctx, frame);
                    if (ret < 0 && ret != AVERROR(EAGAIN))
                    {
                        printf("avcodec_receive_frame fail\n");
                        goto audio_exit;
                    }
                    else if (ret >= 0)
                    {
                        enum AVSampleFormat sample_fmt = (enum AVSampleFormat)(frame->format);
                        // 根据frame中指定的音频参数获取缓冲区的大小
                        int data_size = av_samples_get_buffer_size(NULL, frame->channels, frame->nb_samples, sample_fmt, 1);

                        // 获取声道布局
                        uint64_t dec_channel_layout = (frame->channel_layout && frame->channels == av_get_channel_layout_nb_channels(frame->channel_layout)) ?
                                                          frame->channel_layout : av_get_default_channel_layout(frame->channels);
                        int wanted_nb_samples = frame->nb_samples;

                        //printf("data size = %d, layout = %d, wanted_nb_samples = %d\n", data_size, dec_channel_layout, wanted_nb_samples);

                        if (sample_fmt != in_sample_fmt ||
                            dec_channel_layout != in_ch_layout ||
                            frame->sample_rate != in_sample_rate ||
                            frame->channels != in_channels)
                        {
                            swr_free(&audio_swr_ctx);

                            // 使用frame(源)和is->audio_param_tgt(目标)中的音频参数来设置is->audio_swr_ctx
                            audio_swr_ctx = swr_alloc_set_opts(NULL, out_ch_layout, out_sample_fmt, out_sample_rate,
                                                               dec_channel_layout, sample_fmt, frame->sample_rate, 0, NULL);
                            if (!audio_swr_ctx || swr_init(audio_swr_ctx) < 0)
                            {
                                av_log(NULL, AV_LOG_ERROR,
                                       "cannot create sample rate converter for conversion of %d Hz %s %d channels to %d Hz %s %lld channels!\n",
                                       frame->sample_rate, av_get_sample_fmt_name(sample_fmt), frame->channels,
                                       out_sample_rate, av_get_sample_fmt_name(out_sample_fmt), out_ch_layout);
                                swr_free(&audio_swr_ctx);
                                goto audio_exit;
                            }

                            in_ch_layout = dec_channel_layout;
                            in_channels = frame->channels;
                            in_sample_rate = frame->sample_rate;
                            in_sample_fmt = sample_fmt;
                            av_log(NULL, AV_LOG_INFO, "swr_alloc_set_opts ok!\n");
                        }

                        if (audio_swr_ctx)
                        {
                            const uint8_t **in = (const uint8_t **)frame->extended_data;
                            // 重采样输出参数:输出音频样本数(多加了256个样本)
                            int out_count = (int64_t)wanted_nb_samples * out_sample_rate / frame->sample_rate + 256;
                            // 重采样输出参数:输出音频缓冲区尺寸(以字节为单位)
                            int out_size = av_samples_get_buffer_size(NULL, out_channels, out_count, out_sample_fmt, 1);
                            if (out_size < 0)
                            {
                                av_log(NULL, AV_LOG_ERROR, "av_samples_get_buffer_size() failed\n");
                                goto audio_exit;
                            }

                            av_fast_malloc(&audio_buffer, &audio_frm_rwr_size, out_size);
                            if (!audio_buffer)
                            {
                                goto audio_exit;
                            }

                            // 音频重采样：返回值是重采样后得到的音频数据中单个声道的样本数
                            int len2 = swr_convert(audio_swr_ctx, &audio_buffer, out_count, in, frame->nb_samples);
                            if (len2 < 0)
                            {
                                av_log(NULL, AV_LOG_ERROR, "swr_convert() failed\n");
                                goto audio_exit;
                            }

                            if (len2 == out_count)
                            {
                                av_log(NULL, AV_LOG_WARNING, "audio buffer is probably too small\n");
                                if (swr_init(audio_swr_ctx) < 0)
                                    swr_free(&audio_swr_ctx);
                                goto audio_exit;
                            }

                            // 重采样返回的一帧音频数据大小(以字节为单位)
                            s16DataLen = len2 * out_channels * av_get_bytes_per_sample(out_sample_fmt);
                            //av_log(NULL, AV_LOG_INFO, "audio frame data size: %d, wanted_nb_samples: %d\n", s16DataLen, wanted_nb_samples);
                        }
                        else
                        {
                            audio_buffer = frame->data[0];
                            s16DataLen = data_size;
                        }

                        if (s16DataLen < 0)
                        {
                            av_log(NULL, AV_LOG_ERROR, "valid audio frame data size: %d\n", s16DataLen);
                        }

                        //read data and send to ao module
                        do {
                            if (s16DataLen <= MI_AUDIO_MAX_DATA_SIZE)
                                stAoSendFrame.u32Len = s16DataLen;
                            else
                                stAoSendFrame.u32Len = MI_AUDIO_MAX_DATA_SIZE;
                            stAoSendFrame.apVirAddr[0] = &audio_buffer[u16DataIndex];
                            stAoSendFrame.apVirAddr[1] = NULL;

                            s32TimeOut = s16DataLen * 1000 / (out_sample_rate * out_channels);
                            s32TimeOut = (s32TimeOut < 24) ? 24 : s32TimeOut;
                            //av_log(NULL, AV_LOG_INFO, "channels: %d, mi_ao_sendframe timeout: %d\n", out_channels, s32TimeOut);

                            s16DataLen   -= MI_AUDIO_MAX_DATA_SIZE;
                            u16DataIndex += MI_AUDIO_MAX_DATA_SIZE;

                            do{
                                s32RetSendStatus = MI_AO_SendFrame(AoDevId, AoChn, &stAoSendFrame, s32TimeOut);
                            }while(s32RetSendStatus == MI_AO_ERR_NOBUF);

                            if(s32RetSendStatus != MI_SUCCESS)
                            {
                                printf("[Warning]: MI_AO_SendFrame fail, error is 0x%x: \n",s32RetSendStatus);
                            }
                        }while(s16DataLen > 0);

                        //计算音频pts
                        MI_AO_ChnState_t stState;
                        MI_AO_QueryChnStat(AoDevId, AoChn, &stState);
                        double audio_pts = (frame->pts == AV_NOPTS_VALUE) ? NAN : frame->pts * av_q2d(tb);
                        if (!isnan(audio_pts))
                        {
                            is->audio_clock = audio_pts + (double)frame->nb_samples / frame->sample_rate;
                            is->audio_pts = is->audio_clock - stState.u32ChnBusyNum / bytes_per_sec;
                        }
                        else
                        {
                            is->audio_clock = NAN;
                            is->audio_pts = NAN;
                        }
                        //av_log(NULL, AV_LOG_INFO, "audio frame clock: %f %d %d\n", is->audio_pts, stState.u32ChnBusyNum, bytes_per_sec);

                        av_frame_unref(frame);
                    }
                }
                av_packet_unref(packet);
            }
        }

audio_exit:
        av_frame_free(&frame);
        av_packet_free(&packet);
        av_freep(&audio_buffer);
        swr_free(&audio_swr_ctx);

        b_exit = true;
        av_log(NULL, AV_LOG_INFO, "audio process thread exit!\n");

        return NULL;
}

static int sstar_video_config(AVCodecContext *pVideoCodeCtx)
{
        //取较小值进行缩小
        u16SrcWidth  = FFMIN(pVideoCodeCtx->width , u16OutWidth);
        u16SrcHeight = FFMIN(pVideoCodeCtx->height, u16OutHeight);
        pVideoCodeCtx->flags  = u16SrcWidth;
        pVideoCodeCtx->flags2 = u16SrcHeight;
        return 0;
}


static void * sstar_video_process(void *args)
{
        int ret, avsync_done = 1;
        bool is_soft_codec = false;
        AVPlayer *is = (AVPlayer *)args;
        AVFrame *frame_yuv = NULL;
        uint8_t *video_buffer = NULL;
        struct SwsContext *video_sws_ctx = NULL;
        AVPacket *packet = av_packet_alloc();
        AVFrame *frame = av_frame_alloc();

        AVStream *st = is->format_ctx->streams[is->video_stream_idx];
        AVRational tb = st->time_base;
        AVRational frame_rate = av_guess_frame_rate(is->format_ctx, st, NULL);
        double duration = av_q2d((AVRational){frame_rate.den, frame_rate.num});
        double start_time, now_time, diff_time, remaining_time;

        duration = isnan(duration) ? 0.040 : duration;
        av_log(NULL, AV_LOG_INFO, "video frame rate time:  %f\n", duration);

        is_soft_codec = false;

        //none h264 sw decode need transcode to NV12 for disp input
        //if((is->v_codec_ctx->codec_id != AV_CODEC_ID_H264 && is->v_codec_ctx->codec_id != AV_CODEC_ID_HEVC) || is_soft_codec)
        {
            //转换成YUV420P
            frame_yuv = av_frame_alloc();
            //只有指定了AVFrame的像素格式,画面大小才能真正分配内存
            //缓冲区分配内存
            video_buffer = (uint8_t *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_BGR32, u16SrcWidth, u16SrcHeight, 1));
            if (!video_buffer)
            {
                av_log(NULL, AV_LOG_ERROR, "malloc video buffer failed!\n");
                goto video_exit;
            }
            //初始化缓冲区
            frame_yuv->width  = u16SrcWidth;
            frame_yuv->height = u16SrcHeight;
            ret = av_image_fill_arrays(frame_yuv->data,     // dst data[]
                                       frame_yuv->linesize, // dst linesize[]
                                       video_buffer,        // src buffer
                                       AV_PIX_FMT_BGR32,     // pixel format
                                       u16SrcWidth,
                                       u16SrcHeight,
                                       1                    // align
                                       );
            if (ret < 0)
            {
                av_log(NULL, AV_LOG_ERROR, "av_image_fill_arrays() failed %d\n", ret);
                goto video_exit;
            }

            //用于转码（缩放）的参数，转之前的宽高，转之后的宽高，格式等
            //video_sws_ctx = sws_getContext(is->v_codec_ctx->width, is->v_codec_ctx->height, is->v_codec_ctx->pix_fmt,
            //                              u16SrcWidth, u16SrcHeight, AV_PIX_FMT_NV12,
            //                               SWS_BICUBIC, NULL, NULL, NULL);
            video_sws_ctx = sws_getContext(is->v_codec_ctx->width, is->v_codec_ctx->height, AV_PIX_FMT_NV12,
                                           u16SrcWidth, u16SrcHeight, AV_PIX_FMT_BGR32,
                                           SWS_BICUBIC, NULL, NULL, NULL);
        }


        //h264 hw decode need bind vdec 2 disp
        if (is->v_codec_ctx->codec_id == AV_CODEC_ID_H264 || is->v_codec_ctx->codec_id == AV_CODEC_ID_HEVC)
        {
            is->v_codec_ctx->flags |= (1 << 8);
            sstar_module_bind();
        }

        while (!b_exit)
        {
            ret = sstar_packet_get(&is->v_pkt_queue, packet, 1);
            if(ret > 0)
            {
                if (packet->stream_index == is->video_stream_idx)
                {
                    start_time = av_gettime_relative() / 1000000.0;
                    //7.解码一帧视频压缩数据
                    ret = avcodec_send_packet(is->v_codec_ctx, packet);
                    if (ret < 0 && ret != AVERROR(EAGAIN))
                    {
                        av_log(NULL, AV_LOG_ERROR, "avcodec_send_packet fail!\n");
                        goto video_exit;
                    }

                    ret = avcodec_receive_frame(is->v_codec_ctx, frame);
                    if (ret < 0 && ret != AVERROR(EAGAIN))
                    {
                        printf("avcodec_receive_frame fail\n");
                        goto video_exit;
                    }
                    else if (ret >= 0)
                    {
                        double codec_time = av_gettime_relative() / 1000000.0;
                        av_log(NULL, AV_LOG_INFO, "video decoder cost: %f \n", codec_time - start_time);
                        //none h264 sw decode need put frame to disp
                        //if((is->v_codec_ctx->codec_id != AV_CODEC_ID_H264 && is->v_codec_ctx->codec_id != AV_CODEC_ID_HEVC) || is_soft_codec)
                        {
                            //av_log(NULL, AV_LOG_WARNING, "frame pixel format: %d\n", frame->format);
                            sws_scale(video_sws_ctx, frame->data, frame->linesize, 0, is->v_codec_ctx->height, frame_yuv->data, frame_yuv->linesize);

#if 0

                            int y_size = frame_yuv->width * frame_yuv->height;

                            MI_SYS_BUF_HANDLE hHandle;
                            MI_SYS_ChnPort_t pstSysChnPort;
                            MI_SYS_BufConf_t stBufConf;
                            MI_SYS_BufInfo_t stBufInfo;

                            pstSysChnPort.eModId    = E_MI_MODULE_ID_DISP;
                            pstSysChnPort.u32ChnId  = 0;
                            pstSysChnPort.u32DevId  = 0;
                            pstSysChnPort.u32PortId = 0;

                            memset(&stBufInfo , 0 , sizeof(MI_SYS_BufInfo_t));
                            memset(&stBufConf , 0 , sizeof(MI_SYS_BufConf_t));

                            stBufConf.eBufType = E_MI_SYS_BUFDATA_FRAME;
                            stBufConf.u64TargetPts = 0;
                            stBufConf.stFrameCfg.u16Width  = frame_yuv->width;
                            stBufConf.stFrameCfg.u16Height = frame_yuv->height;
                            stBufConf.stFrameCfg.eFormat   = E_MI_SYS_PIXEL_FRAME_YUV_SEMIPLANAR_420;
                            stBufConf.stFrameCfg.eFrameScanMode = E_MI_SYS_FRAME_SCAN_MODE_PROGRESSIVE;

                            if(MI_SUCCESS == MI_SYS_ChnInputPortGetBuf(&pstSysChnPort,&stBufConf,&stBufInfo,&hHandle, -1))
                            {
                                stBufInfo.stFrameData.eCompressMode = E_MI_SYS_COMPRESS_MODE_NONE;
                                stBufInfo.stFrameData.eFieldType = E_MI_SYS_FIELDTYPE_NONE;
                                stBufInfo.stFrameData.eTileMode = E_MI_SYS_FRAME_TILE_MODE_NONE;
                                stBufInfo.bEndOfStream = FALSE;

                                memcpy(stBufInfo.stFrameData.pVirAddr[0], frame_yuv->data[0], y_size);
                                memcpy(stBufInfo.stFrameData.pVirAddr[1], frame_yuv->data[1], y_size / 2);

                                MI_SYS_ChnInputPortPutBuf(hHandle ,&stBufInfo , FALSE);
                            }
#else
                            //构造QImage
                            QImage img(video_buffer, u16SrcWidth, u16SrcHeight, QImage::Format_RGB32);
                            //qDebug()<<"decode img";
                            VideoPlayerHw::sendMsg(img);
#endif

                        }
                        double convert_time = av_gettime_relative() / 1000000.0;
                        av_log(NULL, AV_LOG_INFO, "video format convert cost: %f \n", convert_time - codec_time);

                        //计算视频时间戳
                        is->video_clock += duration;
                        if (frame->pts == AV_NOPTS_VALUE)
                            is->video_pts = is->video_clock;
                        else
                            is->video_pts = frame->pts * av_q2d(tb);

                        diff_time = (is->audio_pts > 0) ? (is->video_pts - is->audio_pts) : 0;

                        now_time = av_gettime_relative() / 1000000.0;
                        if (fabs(diff_time) > 5 * duration)
                        {
                            avsync_done = 0;
                            if (diff_time > 0)
                                remaining_time = 2 * duration;
                            else
                                remaining_time = FFMAX(0.01 + start_time - now_time, 0);
                        }
                        else
                        {
                            if (fabs(diff_time) < duration)
                                avsync_done = 1;
                            if (avsync_done)
                                remaining_time = FFMAX(duration + start_time - now_time, 0);
                        }
                        av_log(NULL, AV_LOG_INFO, "video frame clock: %f %f %f\n", is->video_pts, remaining_time, now_time - start_time);
                        av_usleep((unsigned long)(remaining_time * 1000000.0));

                        av_frame_unref(frame);
                    }
                }
                av_packet_unref(packet);
            }
        }

video_exit:
        if(is->v_codec_ctx->codec_id == AV_CODEC_ID_H264 || is->v_codec_ctx->codec_id == AV_CODEC_ID_HEVC)
        {
            sstar_module_unbind();
        }

        av_frame_free(&frame);
        av_frame_free(&frame_yuv);
        av_packet_free(&packet);
        av_freep(&video_buffer);
        sws_freeContext(video_sws_ctx);

        b_exit = true;
        av_log(NULL, AV_LOG_INFO, "video process thread exit!\n");

        return NULL;
}

void VideoPlayerHw::sendMsg(QImage img){
    emit myPlayer->sig_GetOneFrame(0,0,img);
}

static void signal_handler_fun(int signum) {
        printf("catch signal [%d]\n", signum);
        b_exit = true;
}



static int videohandler(QString myFileName)
{
        QByteArray ba = myFileName.toLatin1();
        const char * filename = NULL;
        int ret, file_eof = 0;
        pthread_t audio_tid;
        pthread_t video_tid;
        AVPacket pkt, *packet = &pkt;
        AVPlayer *mmplayer;

        filename = ba.data();
        if (!filename)
        {
            printf("please input a vaild filename!\n");
            return -1;
        }

        printf("welcome to test mmplayer!\n");

        signal(SIGINT, signal_handler_fun);

        sstar_sdk_init();

        mmplayer = (AVPlayer *)av_malloc(sizeof(AVPlayer));
        if (!mmplayer)
        {
            av_log(NULL, AV_LOG_ERROR, "no memory for mmplayer!\n");
            goto fail;
        }
        memset(mmplayer, 0x0, sizeof(AVPlayer));
        mmplayer->audio_stream_idx = -1;
        mmplayer->video_stream_idx = -1;

        //1.封装格式上下文
        mmplayer->format_ctx = avformat_alloc_context();
        if (!mmplayer->format_ctx)
        {
            av_log(NULL, AV_LOG_ERROR, "avformat_alloc_context error!\n");
            goto fail;
        }

        //2.打开输入音频文件
        if (avformat_open_input(&mmplayer->format_ctx, filename, NULL, NULL) != 0) {
            av_log(NULL, AV_LOG_ERROR, "avformat_open_input error!\n");
            goto fail;
        }

        //3.获取音频信息
        if (avformat_find_stream_info(mmplayer->format_ctx, NULL) < 0) {
            av_log(NULL, AV_LOG_ERROR, "avformat_find_stream_info error!\n");
            goto fail;
        }

        //音频解码，需要找到对应的AVStream所在的pFormatCtx->streams的索引位置
        for (int i = 0; i < mmplayer->format_ctx->nb_streams; i ++) {
            //根据类型判断是否是音频流
            if (mmplayer->format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
                mmplayer->audio_stream_idx = i;
                break;
            }
        }
        for (int i = 0; i < mmplayer->format_ctx->nb_streams; i ++) {
            //根据类型判断是否是音频流
            if (mmplayer->format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                mmplayer->video_stream_idx = i;
                break;
            }
        }
        if (mmplayer->audio_stream_idx < 0 && mmplayer->video_stream_idx < 0)
        {
            av_log(NULL, AV_LOG_ERROR, "cannot find valid audio or video stream!\n");
            goto fail;
        }

        //4.获取解码器
        //根据索引拿到对应的流,根据流拿到解码器上下文
        if (mmplayer->audio_stream_idx >= 0)
        {
            AVCodecParameters *pAudioPar = mmplayer->format_ctx->streams[mmplayer->audio_stream_idx]->codecpar;

            //再根据上下文拿到编解码id，通过该id拿到解码器
            AVCodec *pAudioCodec = avcodec_find_decoder(pAudioPar->codec_id);
            if (pAudioCodec == NULL) {
                av_log(NULL, AV_LOG_ERROR, "avcodec_find_decoder error!\n");
                goto fail;
            }
            av_log(NULL, AV_LOG_INFO, "open audio codec: %s\n", pAudioCodec->name);

            mmplayer->a_codec_ctx = avcodec_alloc_context3(pAudioCodec);
            if (!mmplayer->a_codec_ctx)
            {
                av_log(NULL, AV_LOG_ERROR, "audio avcodec_alloc_context3 failed\n");
                goto fail;
            }
            ret = avcodec_parameters_to_context(mmplayer->a_codec_ctx, pAudioPar);
            if (ret < 0)
            {
                av_log(NULL, AV_LOG_ERROR, "audio avcodec_parameters_to_context failed %d\n", ret);
                goto fail;
            }

            //5.打开解码器
            if (avcodec_open2(mmplayer->a_codec_ctx, pAudioCodec, NULL) < 0) {
                av_log(NULL, AV_LOG_ERROR, "avcodec_open2 audio failed!\n");
                goto fail;
            }
        }

        if (mmplayer->video_stream_idx >= 0)
        {
            AVCodecParameters *pVideoPar = mmplayer->format_ctx->streams[mmplayer->video_stream_idx]->codecpar;

            AVCodec *pVideoCodec = NULL;

            if (pVideoPar->codec_id == AV_CODEC_ID_H264)
            {
                pVideoCodec = avcodec_find_decoder_by_name("ssh264");
            }
            else if (pVideoPar->codec_id == AV_CODEC_ID_HEVC)
            {
                pVideoCodec = avcodec_find_decoder_by_name("sshevc");
            }
            else
            {
                pVideoCodec = avcodec_find_decoder(pVideoPar->codec_id);
            }

            if (pVideoCodec == NULL) {
                av_log(NULL, AV_LOG_ERROR, "avcodec_find_decoder failed!\n");
                goto fail;
            }
            av_log(NULL, AV_LOG_INFO, "open video codec: %s\n", pVideoCodec->name);

            mmplayer->v_codec_ctx = avcodec_alloc_context3(pVideoCodec);
            if (!mmplayer->v_codec_ctx)
            {
                av_log(NULL, AV_LOG_ERROR, "video avcodec_alloc_context3 failed\n");
                goto fail;
            }
            ret = avcodec_parameters_to_context(mmplayer->v_codec_ctx, pVideoPar);
            if (ret < 0)
            {
                av_log(NULL, AV_LOG_ERROR, "video avcodec_parameters_to_context failed %d\n", ret);
                goto fail;
            }
            printf("video code width: %d, height: %d, codeid: %d\n", mmplayer->v_codec_ctx->width, mmplayer->v_codec_ctx->height, mmplayer->v_codec_ctx->codec_id);

            sstar_video_config(mmplayer->v_codec_ctx);

            if (avcodec_open2(mmplayer->v_codec_ctx, pVideoCodec, NULL) < 0) {
                av_log(NULL, AV_LOG_ERROR, "avcodec_open2 video failed!\n");
                goto fail;
            }
        }

        av_dump_format(mmplayer->format_ctx, 0, filename, 0);

        pthread_mutex_init(&mmplayer->a_pkt_queue.mutex, NULL);
        pthread_cond_init(&mmplayer->a_pkt_queue.cond, NULL);
        pthread_mutex_init(&mmplayer->v_pkt_queue.mutex, NULL);
        pthread_cond_init(&mmplayer->v_pkt_queue.cond, NULL);

        if (mmplayer->audio_stream_idx >= 0)
        {
            pthread_create(&audio_tid, NULL, sstar_audio_process, (void *)mmplayer);
        }

        if (mmplayer->video_stream_idx >= 0)
        {
            pthread_create(&video_tid, NULL, sstar_video_process, (void *)mmplayer);
        }

        //6.一帧一帧读取压缩的音频数据AVPacket
        while (!b_exit)
        {
            if (mmplayer->a_pkt_queue.size + mmplayer->v_pkt_queue.size >= 2 * 1024 * 1024)
            {
                av_usleep(10 * 1000);
                continue;
            }

            if ((ret = av_read_frame(mmplayer->format_ctx, packet)) >= 0)
            {
                file_eof = 0;
                if (packet->stream_index == mmplayer->audio_stream_idx)
                {
                    sstar_packet_put(&mmplayer->a_pkt_queue, packet);
                    //printf("packet queue num : %d\n", mmplayer->a_pkt_queue.nb_packets);
                }
                else if (packet->stream_index == mmplayer->video_stream_idx)
                {
                    sstar_packet_put(&mmplayer->v_pkt_queue, packet);
                    //printf("packet queue num : %d\n", mmplayer->v_pkt_queue.nb_packets);
                }
                else
                {
                    av_packet_unref(packet);
                    //printf("unkown pkt[%d]\n",packet->stream_index);
                }
            }
            else
            {
                if ((ret == AVERROR_EOF || avio_feof(mmplayer->format_ctx->pb)) && !file_eof)
                {
                    printf("avio buffer has been read finish!\n");
                    file_eof = 1;
                }

                if (file_eof && !mmplayer->a_pkt_queue.nb_packets && !mmplayer->v_pkt_queue.nb_packets)
                {
#if 0
                int seekflag = AVSEEK_FLAG_ANY;
                seekflag &= ~AVSEEK_FLAG_BYTE;
                ret = avformat_seek_file(mmplayer->format_ctx, -1, INT64_MIN, mmplayer->format_ctx->start_time, INT64_MAX, seekflag);
                if (ret < 0)
                {
                    av_log(NULL, AV_LOG_ERROR, "%s: error while seeking\n", mmplayer->format_ctx->url);
                    b_exit = true;
                }
                mmplayer->audio_clock = 0;
                mmplayer->audio_pts = 0;
                mmplayer->video_clock = 0;
                mmplayer->video_pts = 0;
#else

                    b_exit = true;
                    mmplayer->audio_clock = 0;
                    mmplayer->audio_pts = 0;
                    mmplayer->video_clock = 0;
                    mmplayer->video_pts = 0;
#endif
                }
            }
        }

        mmplayer->a_pkt_queue.abort_request = 1;
        mmplayer->v_pkt_queue.abort_request = 1;
        pthread_cond_signal(&mmplayer->a_pkt_queue.cond);
        pthread_cond_signal(&mmplayer->v_pkt_queue.cond);

        if (mmplayer->audio_stream_idx >= 0)
        {
            pthread_join(audio_tid, NULL);
        }

        if (mmplayer->video_stream_idx >= 0)
        {
            pthread_join(video_tid, NULL);
        }

        sstar_packet_flush(&mmplayer->a_pkt_queue);
        sstar_packet_flush(&mmplayer->v_pkt_queue);
        pthread_mutex_destroy(&mmplayer->a_pkt_queue.mutex);
        pthread_cond_destroy(&mmplayer->a_pkt_queue.cond);
        pthread_mutex_destroy(&mmplayer->v_pkt_queue.mutex);
        pthread_cond_destroy(&mmplayer->v_pkt_queue.cond);

fail:
        avcodec_close(mmplayer->a_codec_ctx);
        avcodec_close(mmplayer->v_codec_ctx);
        avformat_close_input(&mmplayer->format_ctx);
        av_freep(&mmplayer);

        sstar_sdk_deinit();

        return SUCCESS;
}





VideoPlayerHw::VideoPlayerHw(QObject *parent)
{
    myPlayer = this;
    mRequest = new NetworkRequest;

    connect(mRequest,SIGNAL(sig_GetNetworkReply(QString)),this,SLOT(slotGetNetworkReply(QString )));
    mRequest->start();
}

VideoPlayerHw::~VideoPlayerHw()
{
    if(mRequest != NULL){
        delete mRequest;
    }
}



void VideoPlayerHw::slotGetNetworkReply(QString Path)
{
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
}

void VideoPlayerHw::run(){
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
            decodec(curPath + "/Video/" + vObj.videoName, width, heigh,x,y);
        }else{
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}



void VideoPlayerHw::decodec(QString myFileName, int myWidth, int myHeight, int myX, int myY)
{
    u16OutWidth = myWidth;
    u16OutHeight = myHeight;
    videohandler(myFileName);

}

