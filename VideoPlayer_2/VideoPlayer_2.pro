#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T16:10:47
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VideoPlayer_2
TEMPLATE = app


SOURCES += src/main.cpp \
    src/hwplayer/audio.c \
    src/hwplayer/blitutil.c \
    src/hwplayer/demux.c \
    src/hwplayer/frame.c \
    src/hwplayer/hwplayer.cpp \
    src/hwplayer/interface.c \
    src/hwplayer/mainentry.c \
    src/hwplayer/packet.c \
    src/hwplayer/player.c \
    src/hwplayer/sd20xpanel.c \
    src/hwplayer/video.c \
    src/videoplayer/advert.cpp \
    src/videoplayer/downloadtool.cpp \
    src/videoplayer/labelpicture.cpp \
    src/videoplayer/labelvideo.cpp \
    src/videoplayer/networkrequest.cpp \
    src/videoplayer/videoobject.cpp \
    src/videoplayer/videoplayer.cpp \
    #src/videoplayer/videoplayerhw.cpp \
    src/mainwindow.cpp

HEADERS  += \
    src/hwplayer/CC0702I50R_1024x600.h \
    src/hwplayer/audio.h \
    src/hwplayer/blitutil.h \
    src/hwplayer/demux.h \
    src/hwplayer/frame.h \
    src/hwplayer/hwplayer.h \
    src/hwplayer/interface.h \
    src/hwplayer/mainentry.h \
    src/hwplayer/mi_vdec_extra.h \
    src/hwplayer/packet.h \
    src/hwplayer/player.h \
    src/hwplayer/sd20xpanel.h \
    src/hwplayer/video.h \
    src/videoplayer/advert.h \
    src/videoplayer/downloadtool.h \
    src/videoplayer/labelpicture.h \
    src/videoplayer/labelvideo.h \
    src/videoplayer/networkrequest.h \
    src/videoplayer/videoobject.h \
    src/videoplayer/videoplayer.h \
    #src/videoplayer/videoplayerhw.h \
    src/mainwindow.h

FORMS    += \
    src/mainwindow.ui

INCLUDEPATH += $$PWD/ffmpeg/include \
                $$PWD/src


    ## Windows common build here
LIBS += $$PWD/ffmpeg/lib/avcodec.lib \
        $$PWD/ffmpeg/lib/avdevice.lib \
        $$PWD/ffmpeg/lib/avfilter.lib \
        $$PWD/ffmpeg/lib/avformat.lib \
        $$PWD/ffmpeg/lib/avutil.lib \
        $$PWD/ffmpeg/lib/postproc.lib \
        $$PWD/ffmpeg/lib/swresample.lib \
        $$PWD/ffmpeg/lib/swscale.lib
