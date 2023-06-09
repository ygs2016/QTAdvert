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


INCLUDEPATH +=  /home/industio/ssd20xxx/ffmpeg/ffmpeg-4.1.3/host/include \
		/home/industio/ssd20xxx/project/release/include \
                $$PWD/src

LIBS += -L/home/industio/ssd20xxx/ffmpeg/ffmpeg-4.1.3/host/static/libavcodec.a \
	-L/home/industio/ssd20xxx/ffmpeg/ffmpeg-4.1.3/host/static/libavformat.a \
	-L/home/industio/ssd20xxx/ffmpeg/ffmpeg-4.1.3/host/static/libavutil.a \
	-L/home/industio/ssd20xxx/ffmpeg/ffmpeg-4.1.3/host/static/libswresample.a \
	-L/home/industio/ssd20xxx/ffmpeg/ffmpeg-4.1.3/host/static/libswscale.a 

LIBS += -L/home/industio/ssd20xxx/ffmpeg/ffmpeg-4.1.3/host/dynamic -lavcodec \
	-L/home/industio/ssd20xxx/ffmpeg/ffmpeg-4.1.3/host/dynamic -lavformat \
	-L/home/industio/ssd20xxx/ffmpeg/ffmpeg-4.1.3/host/dynamic -lavutil \
	-L/home/industio/ssd20xxx/ffmpeg/ffmpeg-4.1.3/host/dynamic -lswresample \
	-L/home/industio/ssd20xxx/ffmpeg/ffmpeg-4.1.3/host/dynamic -lswscale \
	-L/home/industio/ssd20xxx/ffmpeg/3rdparty/lib/8.2.1 -lcrypto \
	-L/home/industio/ssd20xxx/ffmpeg/3rdparty/lib/8.2.1 -lssl \
	-L/home/industio/ssd20xxx/project/image/output/miservice/config/lib -lmi_disp \
	-L/home/industio/ssd20xxx/project/image/output/miservice/config/lib -lmi_panel \
	-L/home/industio/ssd20xxx/project/image/output/miservice/config/lib -lmi_common \
	-L/home/industio/ssd20xxx/project/image/output/miservice/config/lib -lmi_vdec \
	-L/home/industio/ssd20xxx/project/image/output/miservice/config/lib -lmi_ai \
	-L/home/industio/ssd20xxx/project/image/output/miservice/config/lib -lmi_venc \
	-L/home/industio/ssd20xxx/project/image/output/miservice/config/lib -lmi_divp \
	-L/home/industio/ssd20xxx/project/image/output/miservice/config/lib -lmi_gfx \
	-L/home/industio/ssd20xxx/project/image/output/miservice/config/lib -lmi_ipu \
	-L/home/industio/ssd20xxx/project/image/output/miservice/config/lib -lmi_sys \
	-L/home/industio/ssd20xxx/project/image/output/miservice/config/lib -lmi_ao

QTPLUGIN	+= qlinuxfb
