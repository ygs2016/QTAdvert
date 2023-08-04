#ifndef MQTTREQUEST_H
#define MQTTREQUEST_H


#include "videoplayer/advert.h"
#include <QNetworkAccessManager>
#include <QThread>
#include <qmqtt_client.h>
#include <QtNetwork>
#include <QHostAddress>
#include <mqtt/qmqtt.h>
#include <mqtt/qmqtt_message.h>

class mqttrequest: public QThread
{
    Q_OBJECT

public:
    explicit mqttrequest(QObject *parent = nullptr);
    ~mqttrequest();

    QString getSignStr(QString urlStr, QString key);
    QString regMqtt(QString appid, QString sn, QString ts);
    QString getBodyStr(QString rawStr, QString sign);
    void getDataStrByNetwork();
    void run();
    void clientMqtt(QString host, int port, QString userName, QString pwd, QString clientId);
    QString getLabelMqtt(QString appid,QString clientId,QString nlast, QString ts);
private:
    bool isRunning = false;
    int requestStep = 0; //1,request mqtt 2, request label
    QString curPostBody = NULL;
    QString curClientId = NULL;
    Advert curAd;
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QMQTT::Client *client; // 创建QMQTT 客户端
private slots:
    void getBack(QNetworkReply * reply);
    void onMQTT_Received(QMQTT::Message);
    void onMQTT_subscribed( QString, quint8);
    void onMQTT_unSubscribed( QString );
    void onMQTT_error(const QMQTT::ClientError error);
signals:
    void sig_NeedGetLabelAd(Advert str);
};

#endif // MQTTREQUEST_H
