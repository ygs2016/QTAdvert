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
    src/videoplayer/advert.cpp \
    src/videoplayer/downloadtool.cpp \
    src/videoplayer/labelpicture.cpp \
    src/videoplayer/labelvideo.cpp \
    src/videoplayer/networkrequest.cpp \
    src/videoplayer/videoobject.cpp \
    src/videoplayer/videoplayer.cpp \
    src/mainwindow.cpp

HEADERS  += \
    src/videoplayer/advert.h \
    src/videoplayer/downloadtool.h \
    src/videoplayer/labelpicture.h \
    src/videoplayer/labelvideo.h \
    src/videoplayer/networkrequest.h \
    src/videoplayer/videoobject.h \
    src/videoplayer/videoplayer.h \
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
