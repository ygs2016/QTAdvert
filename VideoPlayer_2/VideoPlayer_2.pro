#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T16:10:47
#
#-------------------------------------------------

QT       += core gui network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VideoPlayer_2
TEMPLATE = app
DEFINES += PC_TEST

SOURCES += src/main.cpp \
#    src/hwplayer/audio.c \
#    src/hwplayer/blitutil.c \
#    src/hwplayer/demux.c \
#    src/hwplayer/frame.c \
    src/hwplayer/hwplayer.cpp \
#    src/hwplayer/interface.c \
#    src/hwplayer/mainentry.c \
#    src/hwplayer/packet.c \
#    src/hwplayer/player.c \
#    src/hwplayer/sd20xpanel.c \
#    src/hwplayer/video.c \
    src/mqtt/qmqtt_client.cpp \
    src/mqtt/qmqtt_client_p.cpp \
    src/mqtt/qmqtt_frame.cpp \
    src/mqtt/qmqtt_message.cpp \
    src/mqtt/qmqtt_network.cpp \
    src/mqtt/qmqtt_router.cpp \
    src/mqtt/qmqtt_routesubscription.cpp \
    src/mqtt/qmqtt_socket.cpp \
    src/mqtt/qmqtt_ssl_socket.cpp \
    src/mqtt/qmqtt_timer.cpp \
    src/mqtt/qmqtt_websocket.cpp \
    src/mqtt/qmqtt_websocketiodevice.cpp \
    src/videoplayer/advert.cpp \
    src/videoplayer/downloadtool.cpp \
    src/videoplayer/labelpicture.cpp \
    src/videoplayer/labelvideo.cpp \
    src/videoplayer/mqttrequest.cpp \
    src/videoplayer/networkrequest.cpp \
    src/videoplayer/uartctrl.cpp \
    src/videoplayer/videoobject.cpp \
#    src/videoplayer/videoplayerhw.cpp \
    src/mainwindow.cpp

HEADERS  += \
#    src/hwplayer/CC0702I50R_1024x600.h \
#    src/hwplayer/audio.h \
#    src/hwplayer/blitutil.h \
#    src/hwplayer/demux.h \
#    src/hwplayer/frame.h \
    src/hwplayer/hwplayer.h \
#    src/hwplayer/interface.h \
#    src/hwplayer/mainentry.h \
#    src/hwplayer/mi_vdec_extra.h \
#    src/hwplayer/packet.h \
#    src/hwplayer/player.h \
#    src/hwplayer/sd20xpanel.h \
#    src/hwplayer/video.h \
    src/mqtt/qmqtt.h \
    src/mqtt/qmqtt_client.h \
    src/mqtt/qmqtt_client_p.h \
    src/mqtt/qmqtt_frame.h \
    src/mqtt/qmqtt_global.h \
    src/mqtt/qmqtt_message.h \
    src/mqtt/qmqtt_message_p.h \
    src/mqtt/qmqtt_network_p.h \
    src/mqtt/qmqtt_networkinterface.h \
    src/mqtt/qmqtt_routedmessage.h \
    src/mqtt/qmqtt_router.h \
    src/mqtt/qmqtt_routesubscription.h \
    src/mqtt/qmqtt_socket_p.h \
    src/mqtt/qmqtt_socketinterface.h \
    src/mqtt/qmqtt_ssl_socket_p.h \
    src/mqtt/qmqtt_timer_p.h \
    src/mqtt/qmqtt_timerinterface.h \
    src/mqtt/qmqtt_websocket_p.h \
    src/mqtt/qmqtt_websocketiodevice_p.h \
    src/videoplayer/advert.h \
    src/videoplayer/downloadtool.h \
    src/videoplayer/labelpicture.h \
    src/videoplayer/labelvideo.h \
    src/videoplayer/mqttrequest.h \
    src/videoplayer/networkrequest.h \
    src/videoplayer/uartctrl.h \
    src/videoplayer/videoobject.h \
#    src/videoplayer/videoplayerhw.h \
    src/mainwindow.h

FORMS    += \
    src/mainwindow.ui

INCLUDEPATH += $$PWD/ffmpeg/include \
                $$PWD/src \
                $$PWD/src/mqtt


    ## Windows common build here
LIBS += $$PWD/ffmpeg/lib/avcodec.lib \
        $$PWD/ffmpeg/lib/avdevice.lib \
        $$PWD/ffmpeg/lib/avfilter.lib \
        $$PWD/ffmpeg/lib/avformat.lib \
        $$PWD/ffmpeg/lib/avutil.lib \
        $$PWD/ffmpeg/lib/postproc.lib \
        $$PWD/ffmpeg/lib/swresample.lib \
        $$PWD/ffmpeg/lib/swscale.lib
