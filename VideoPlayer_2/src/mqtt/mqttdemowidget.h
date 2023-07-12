#ifndef MQTTDEMOWIDGET_H
#define MQTTDEMOWIDGET_H

#include <QWidget>

#include <QtNetwork>
#include <QHostAddress>
#include <mqtt/qmqtt.h>


class MqttDemoWidget : public QObject {
    Q_OBJECT

public:
    MqttDemoWidget(QObject *parent = nullptr);
    ~MqttDemoWidget();

    void clientMqtt();

private:
    QMQTT::Client *client; // 创建QMQTT 客户端

private slots:
    void onMQTT_Received(QMQTT::Message);
    void onMQTT_subscribed( QString, quint8);
    void onMQTT_unSubscribed( QString );
};

#endif // MQTTDEMOWIDGET_H
