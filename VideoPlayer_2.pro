#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T16:10:47
#
#-------------------------------------------------


QT       += core network serialport
QT	 -= gui

CONFIG += c++17 cmdline
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VideoPlayer_2
#TEMPLATE = app


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
    #src/videoplayer/videoplayer.cpp \
    #src/videoplayer/videoplayerhw.cpp \
    #src/mainwindow.cpp

HEADERS  += \
    src/hwplayer/MYTEST_800x1280.h \
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
    #src/videoplayer/videoplayer.h \
    #src/videoplayer/videoplayerhw.h \
    #src/mainwindow.h

#FORMS    += \
#    src/mainwindow.ui


INCLUDEPATH +=  /home/industio/ssd20xxx/ffmpeg/ffmpeg-4.1.3/host/include \
		/home/industio/ssd20xxx/project/release/include \
                $$PWD/src \
		$$PWD/src/mqtt

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
