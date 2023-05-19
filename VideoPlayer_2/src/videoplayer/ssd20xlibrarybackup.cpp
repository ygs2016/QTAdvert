




extern "C" {

#include "ssd20xlibrary.h"
#include "CC0702I50R_1024x600.h"

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




static MI_S32 hdmi_callback_impl(MI_HDMI_DeviceId_e eHdmi, MI_HDMI_EventType_e Event, void *pEventParam, void *pUsrParam)
{
    switch (Event)
    {
    case E_MI_HDMI_EVENT_HOTPLUG:
        printf("E_MI_HDMI_EVENT_HOTPLUG.\n");
        break;
    case E_MI_HDMI_EVENT_NO_PLUG:
        printf("E_MI_HDMI_EVENT_NO_PLUG.\n");
        break;
    default:
        printf("Unsupport event.\n");
        break;
    }

    return MI_SUCCESS;
}



static MI_S32 sstar_hdmi_init(MI_DISP_OutputTiming_e stHdmiMode)
{
    MI_HDMI_InitParam_t stInitParam;
    MI_HDMI_DeviceId_e eHdmi = E_MI_HDMI_ID_0;
    MI_HDMI_Attr_t stAttr;
    MI_DISP_PubAttr_t stPubAttr;
    MI_DISP_VideoLayerAttr_t stLayerAttr;
    MI_DISP_InputPortAttr_t stInputPortAttr;

    memset(&stPubAttr, 0, sizeof(MI_DISP_PubAttr_t));
    stPubAttr.u32BgColor = YUYV_BLACK;
    stPubAttr.eIntfType  = E_MI_DISP_INTF_HDMI;
    stPubAttr.eIntfSync  = stHdmiMode;//E_MI_DISP_OUTPUT_1080P60
    STCHECKRESULT(MI_DISP_SetPubAttr(0, &stPubAttr));
    STCHECKRESULT(MI_DISP_Enable(0));

    memset(&stLayerAttr, 0, sizeof(stLayerAttr));
    stLayerAttr.stVidLayerDispWin.u16X      = 0;
    stLayerAttr.stVidLayerDispWin.u16Y      = 0;
    stLayerAttr.stVidLayerSize.u16Width     = 1920;
    stLayerAttr.stVidLayerSize.u16Height    = 1080;
    if(stHdmiMode == E_MI_DISP_OUTPUT_720P60) {
        stLayerAttr.stVidLayerDispWin.u16Width  = 1280;
        stLayerAttr.stVidLayerDispWin.u16Height = 720;
    } else {
        stLayerAttr.stVidLayerDispWin.u16Width  = 1920;
        stLayerAttr.stVidLayerDispWin.u16Height = 1080;
    }
    STCHECKRESULT(MI_DISP_BindVideoLayer(0, 0));
    STCHECKRESULT(MI_DISP_SetVideoLayerAttr(0, &stLayerAttr));
    STCHECKRESULT(MI_DISP_EnableVideoLayer(0));

    STCHECKRESULT(MI_DISP_GetInputPortAttr(0, 0, &stInputPortAttr));
    stInputPortAttr.u16SrcWidth         = stLayerAttr.stVidLayerDispWin.u16Width;
    stInputPortAttr.u16SrcHeight        = stLayerAttr.stVidLayerDispWin.u16Height;
    stInputPortAttr.stDispWin.u16X      = 0;
    stInputPortAttr.stDispWin.u16Y      = 0;
    stInputPortAttr.stDispWin.u16Width  = stLayerAttr.stVidLayerDispWin.u16Width;
    stInputPortAttr.stDispWin.u16Height = stLayerAttr.stVidLayerDispWin.u16Height;
    STCHECKRESULT(MI_DISP_SetInputPortAttr(0, 0, &stInputPortAttr));
    STCHECKRESULT(MI_DISP_EnableInputPort(0, 0));
    STCHECKRESULT(MI_DISP_SetInputPortSyncMode(0, 0, E_MI_DISP_SYNC_MODE_FREE_RUN));

    memset(&stInitParam, 0x0, sizeof(MI_HDMI_InitParam_t));
    stInitParam.pCallBackArgs = NULL;
    stInitParam.pfnHdmiEventCallback = hdmi_callback_impl;
    STCHECKRESULT(MI_HDMI_Init(&stInitParam));
    STCHECKRESULT(MI_HDMI_Open(eHdmi));

    memset(&stAttr, 0, sizeof(MI_HDMI_Attr_t));
    stAttr.stEnInfoFrame.bEnableAudInfoFrame  = FALSE;
    stAttr.stEnInfoFrame.bEnableAviInfoFrame  = FALSE;
    stAttr.stEnInfoFrame.bEnableSpdInfoFrame  = FALSE;
    stAttr.stAudioAttr.bEnableAudio    = TRUE;
    stAttr.stAudioAttr.bIsMultiChannel = 0;
    stAttr.stAudioAttr.eBitDepth       = E_MI_HDMI_BIT_DEPTH_16;
    stAttr.stAudioAttr.eCodeType       = E_MI_HDMI_ACODE_PCM;
    stAttr.stAudioAttr.eSampleRate     = E_MI_HDMI_AUDIO_SAMPLERATE_48K;
    stAttr.stVideoAttr.bEnableVideo    = TRUE;
    stAttr.stVideoAttr.eColorType      = E_MI_HDMI_COLOR_TYPE_RGB444;
    stAttr.stVideoAttr.eDeepColorMode  = E_MI_HDMI_DEEP_COLOR_MAX;
    if (stHdmiMode == E_MI_DISP_OUTPUT_720P60)
        stAttr.stVideoAttr.eTimingType = E_MI_HDMI_TIMING_720_60P;
    else
        stAttr.stVideoAttr.eTimingType = E_MI_HDMI_TIMING_1080_60P;
    stAttr.stVideoAttr.eOutputMode     = E_MI_HDMI_OUTPUT_MODE_HDMI;
    STCHECKRESULT(MI_HDMI_SetAttr(eHdmi, &stAttr));
    STCHECKRESULT(MI_HDMI_Start(eHdmi));

    return MI_SUCCESS;
}



static MI_S32 sstar_hdmi_deinit(void)
{
    MI_HDMI_DeviceId_e eHdmi = E_MI_HDMI_ID_0;

    //STCHECKRESULT(MI_DISP_DisableInputPort(0, 0));
    STCHECKRESULT(MI_DISP_DisableVideoLayer(0));
    STCHECKRESULT(MI_DISP_UnBindVideoLayer(0, 0));
    STCHECKRESULT(MI_DISP_Disable(0));

    STCHECKRESULT(MI_HDMI_Stop(eHdmi));
    STCHECKRESULT(MI_HDMI_Close(eHdmi));
    STCHECKRESULT(MI_HDMI_DeInit());

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



static int sstar_panel_init(void)
{
    MI_DISP_PubAttr_t stDispPubAttr;
    MI_DISP_InputPortAttr_t stInputPortAttr;
    memset(&stDispPubAttr, 0x0, sizeof(MI_DISP_PubAttr_t));
    stDispPubAttr.stSyncInfo.u16Vact       = stPanelParam.u16Height;
    stDispPubAttr.stSyncInfo.u16Vbb        = stPanelParam.u16VSyncBackPorch;
    stDispPubAttr.stSyncInfo.u16Vfb        = stPanelParam.u16VTotal - (stPanelParam.u16VSyncWidth +
                                                                stPanelParam.u16Height + stPanelParam.u16VSyncBackPorch);
    stDispPubAttr.stSyncInfo.u16Hact       = stPanelParam.u16Width;
    stDispPubAttr.stSyncInfo.u16Hbb        = stPanelParam.u16HSyncBackPorch;
    stDispPubAttr.stSyncInfo.u16Hfb        = stPanelParam.u16HTotal - (stPanelParam.u16HSyncWidth +
                                                                stPanelParam.u16Width + stPanelParam.u16HSyncBackPorch);
    stDispPubAttr.stSyncInfo.u16Bvact      = 0;
    stDispPubAttr.stSyncInfo.u16Bvbb       = 0;
    stDispPubAttr.stSyncInfo.u16Bvfb       = 0;
    stDispPubAttr.stSyncInfo.u16Hpw        = stPanelParam.u16HSyncWidth;
    stDispPubAttr.stSyncInfo.u16Vpw        = stPanelParam.u16VSyncWidth;
    stDispPubAttr.stSyncInfo.u32FrameRate  = stPanelParam.u16DCLK * 1000000 / (stPanelParam.u16HTotal * stPanelParam.u16VTotal);
    stDispPubAttr.eIntfSync                = E_MI_DISP_OUTPUT_USER;
    stDispPubAttr.eIntfType                = E_MI_DISP_INTF_LCD;
    stDispPubAttr.u32BgColor               = YUYV_BLACK;

    MI_DISP_SetPubAttr(0, &stDispPubAttr);
    MI_DISP_Enable(0);
    MI_DISP_BindVideoLayer(0, 0);
    MI_DISP_EnableVideoLayer(0);

    MI_DISP_GetInputPortAttr(0, 0, &stInputPortAttr);
    stInputPortAttr.u16SrcWidth         = stDispPubAttr.stSyncInfo.u16Hact;
    stInputPortAttr.u16SrcHeight        = stDispPubAttr.stSyncInfo.u16Vact;
    stInputPortAttr.stDispWin.u16X      = 0;
    stInputPortAttr.stDispWin.u16Y      = 0;
    stInputPortAttr.stDispWin.u16Width  = stDispPubAttr.stSyncInfo.u16Hact;
    stInputPortAttr.stDispWin.u16Height = stDispPubAttr.stSyncInfo.u16Vact;

    MI_DISP_SetInputPortAttr(0, 0, &stInputPortAttr);
    MI_DISP_EnableInputPort(0, 0);
    MI_DISP_SetInputPortSyncMode(0, 0, E_MI_DISP_SYNC_MODE_FREE_RUN);

    MI_PANEL_Init(E_MI_PNL_LINK_TTL);
    MI_PANEL_SetPanelParam(&stPanelParam);

    return 0;
}


static int sstar_panel_deinit(void)
{
    STCHECKRESULT(MI_DISP_DisableInputPort(0, 0));
    STCHECKRESULT(MI_DISP_DisableVideoLayer(0));
    STCHECKRESULT(MI_DISP_UnBindVideoLayer(0, 0));
    STCHECKRESULT(MI_DISP_Disable(0));
    STCHECKRESULT(MI_PANEL_DeInit());

    return 0;
}

static void sstar_getpanel_wh(uint16_t *width, uint16_t *height)
{
#ifndef SUPPORT_HDMI
    MI_DISP_PubAttr_t stPubAttr;
    MI_DISP_GetPubAttr(0,&stPubAttr);
    *width = stPubAttr.stSyncInfo.u16Hact;
    *height = stPubAttr.stSyncInfo.u16Vact;
#else
    MI_DISP_VideoLayerAttr_t stLayerAttr;
    MI_DISP_GetVideoLayerAttr(0, &stLayerAttr);
    *width = stLayerAttr.stVidLayerDispWin.u16Width;
    *height = stLayerAttr.stVidLayerDispWin.u16Height;
#endif
    printf("sstar_getpanel_wh = [%d %d]\n", *width, *height);
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

 bool b_exit = false;

void signal_handler_fun(int signum) {
    printf("catch signal [%d]\n", signum);
    b_exit = true;
}

MI_U16 u16OutWidth, u16OutHeight;
MI_U16 u16SrcWidth, u16SrcHeight;

typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);

 int sstar_video_config(AVCodecContext *pVideoCodeCtx)
{
    MI_DISP_InputPortAttr_t stInputPortAttr;
    MI_DISP_RotateConfig_t stRotateConfig;

    //获取屏幕宽高参数
    sstar_getpanel_wh(&u16OutWidth, &u16OutHeight);

    //取较小值进行缩小
    u16SrcWidth  = FFMIN(pVideoCodeCtx->width , u16OutWidth);
    u16SrcHeight = FFMIN(pVideoCodeCtx->height, u16OutHeight);
    pVideoCodeCtx->flags  = u16SrcWidth;
    pVideoCodeCtx->flags2 = u16SrcHeight;

    //重设DISP参数
    memset(&stInputPortAttr, 0, sizeof(stInputPortAttr));
    STCHECKRESULT(MI_DISP_GetInputPortAttr(0, 0, &stInputPortAttr));
    stInputPortAttr.stDispWin.u16X      = 0;
    stInputPortAttr.stDispWin.u16Y      = 0;
    stInputPortAttr.stDispWin.u16Width  = u16OutWidth;
    stInputPortAttr.stDispWin.u16Height = u16OutHeight;
    stInputPortAttr.u16SrcWidth         = u16SrcWidth;
    stInputPortAttr.u16SrcHeight        = u16SrcHeight;
    STCHECKRESULT(MI_DISP_SetInputPortAttr(0, 0, &stInputPortAttr));
    STCHECKRESULT(MI_DISP_EnableInputPort(0, 0));
    STCHECKRESULT(MI_DISP_SetInputPortSyncMode(0, 0, E_MI_DISP_SYNC_MODE_FREE_RUN));

    memset(&stRotateConfig, 0x0, sizeof(MI_DISP_RotateConfig_t));
    stRotateConfig.eRotateMode = E_MI_DISP_ROTATE_NONE;
    STCHECKRESULT(MI_DISP_SetVideoLayerRotateMode(0, &stRotateConfig));

    return 0;
}


 int sstar_packet_put(AVPacketQueue *q, AVPacket *pkt)
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


 int sstar_packet_get(AVPacketQueue *q, AVPacket *pkt, int block)
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



 void sstar_packet_flush(AVPacketQueue *q)
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




int sstar_sdk_init(void)
{
    //init system
    sstar_sys_init();

    //init audio
    sstar_ao_init();

//init hdmi/panel
#ifdef SUPPORT_HDMI
    sstar_hdmi_init(E_MI_DISP_OUTPUT_1080P60);
#else
    sstar_panel_init();
#endif

    return 0;
}

int sstar_sdk_deinit(void)
{
#ifdef SUPPORT_HDMI
    sstar_hdmi_deinit();
#else
    sstar_panel_deinit();
#endif

    sstar_ao_deinit();

    sstar_sys_deinit();

    return MI_SUCCESS;
}


 void * sstar_audio_process(void *args)
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



 void * sstar_video_process(void *args)
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
    if((is->v_codec_ctx->codec_id != AV_CODEC_ID_H264 && is->v_codec_ctx->codec_id != AV_CODEC_ID_HEVC) || is_soft_codec)
    {
        //转换成YUV420P
        frame_yuv = av_frame_alloc();
        //只有指定了AVFrame的像素格式,画面大小才能真正分配内存
        //缓冲区分配内存
        video_buffer = (uint8_t *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_NV12, u16SrcWidth, u16SrcHeight, 1));
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
                                   AV_PIX_FMT_NV12,     // pixel format
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
        video_sws_ctx = sws_getContext(is->v_codec_ctx->width, is->v_codec_ctx->height, is->v_codec_ctx->pix_fmt,
                                       u16SrcWidth, u16SrcHeight, AV_PIX_FMT_NV12,
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
                    if((is->v_codec_ctx->codec_id != AV_CODEC_ID_H264 && is->v_codec_ctx->codec_id != AV_CODEC_ID_HEVC) || is_soft_codec)
                    {
                        //av_log(NULL, AV_LOG_WARNING, "frame pixel format: %d\n", frame->format);
                        sws_scale(video_sws_ctx, frame->data, frame->linesize, 0, is->v_codec_ctx->height, frame_yuv->data, frame_yuv->linesize);

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
    #if ((defined CHIP_IS_SSD20X) || (defined CHIP_IS_SS268))
        if(is->v_codec_ctx->codec_id == AV_CODEC_ID_H264 || is->v_codec_ctx->codec_id == AV_CODEC_ID_HEVC)
        {
            sstar_module_unbind();
        }
    #endif

        av_frame_free(&frame);
        av_frame_free(&frame_yuv);
        av_packet_free(&packet);
        av_freep(&video_buffer);
        sws_freeContext(video_sws_ctx);

        b_exit = true;
        av_log(NULL, AV_LOG_INFO, "video process thread exit!\n");

        return NULL;
}

