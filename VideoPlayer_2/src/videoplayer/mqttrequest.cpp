#include "mqttrequest.h"
#include "videoplayer/uartctrl.h"

#include <QCryptographicHash>
#include <QNetworkReply>

mqttrequest::mqttrequest(QObject *parent)
{
    qRegisterMetaType<QMQTT::Message>("QMQTT::Message");
    qRegisterMetaType<QMQTT::ClientError>("QMQTT::ClientError");
    qRegisterMetaType<Advert>("Advert");
}

mqttrequest::~mqttrequest()
{
    if(client != NULL){
        delete client;
        client = NULL;
    }
    if(manager != NULL){
        delete manager;
        manager = NULL;
    }
}


QString mqttrequest::getSignStr(QString urlStr, QString key){

    QString checkStr = urlStr;
    checkStr += "&key=";
    checkStr += key;

    QString md5;
    QByteArray ba,bb;
    QCryptographicHash md(QCryptographicHash::Md5);
    ba.append(checkStr.toLatin1());
    md.addData(ba);
    bb = md.result();
    md5.append(bb.toHex());

    return md5.toUpper();
}

QString mqttrequest::regMqtt(QString appid, QString sn, QString ts){
    QString ret = "";
    ret += "appid=" + appid;
    ret += "&";
    ret += "sn=" + sn;
    ret += "&";
    ret += "ts=" + ts;
    return ret;
}

QString mqttrequest::getLabelMqtt(QString appid,QString clientId,QString nlast, QString ts){
    QString ret = "";
    ret += "appid=" + appid;
    ret += "&";
    ret += "clientid=" + clientId;
    ret += "&";
    ret += "nlast=" + nlast;
    ret += "&";
    ret += "ts=" + ts;
    return ret;
}



QString mqttrequest::getBodyStr(QString rawStr, QString sign){
    QString str = "";
    str += rawStr;
    str += "&";
    str += "sign=" + sign;
    return str;
}

void mqttrequest::run(){
#if 0
    uartctrl::Instantialize()->appId = "36e5d767-92f9-46b0-b34f-dc5a6d0f9c62_2306151600000066";
    uartctrl::Instantialize()->appKey = "4026b2b26b8a48948c1e2a5e3037ee04";
    uartctrl::Instantialize()->labelSn = "4e6eb56ee04b";
#else

#endif
    while(1){
        if(uartctrl::Instantialize()->appId.isEmpty() || uartctrl::Instantialize()->appKey.isEmpty() || uartctrl::Instantialize()->labelSn.isEmpty()){
            qDebug() << "uart config param is error !!!";
            sleep(3);
        }else{
            break;
        }
    }
    manager = new QNetworkAccessManager();
    connect(manager,SIGNAL(finished(QNetworkReply *)),this,SLOT(getBack(QNetworkReply *)),Qt::DirectConnection);
    QDateTime timeDate = QDateTime::currentDateTime();  // 获取当前时间
    int timeStr = timeDate .toTime_t();
    QString rawPostStr = regMqtt(uartctrl::Instantialize()->appId, uartctrl::Instantialize()->labelSn,  QString::number(timeStr, 10));
    curPostBody = getBodyStr(rawPostStr,getSignStr(rawPostStr,uartctrl::Instantialize()->appKey));
    QString httpPostUrl = "https://esl.ttdh.cc/MQTTService/RegisterTag";
    qDebug() << "Http post message url ==> " << httpPostUrl;
    qDebug() << "Http post body ==> " << curPostBody;
    request.setUrl(QUrl::fromUserInput(httpPostUrl));
    request.setRawHeader("Host", "esl.ttdh.cc");
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
    requestStep = 1;
    manager->post(request,curPostBody.toLatin1()); //发出post请求
    exec();
}


void mqttrequest::getBack(QNetworkReply * reply)
{
    //qDebug() << reply->readAll().data();
    QString jsonStr = reply->readAll().data();
    qDebug() << "Raw recv data ==> " << jsonStr;
    QJsonParseError err_rpt;
    qDebug() << "Start to prase mqtt json";
    QJsonDocument jsonRootDoc = QJsonDocument::fromJson(jsonStr.toLatin1(),&err_rpt);
    if (!jsonRootDoc.isNull() && err_rpt.error == QJsonParseError::NoError){
        qDebug() << "prase no error";
        if(jsonRootDoc.isObject()){
            qDebug() << "jsonRoot is Object";
            QJsonObject obj_root = jsonRootDoc.object();
            QJsonValue stateValue = obj_root.value("State");
            if(stateValue.isString() && stateValue.toString() == QString::fromLocal8Bit("Done")){
                qDebug() << "State is Done";
                QJsonValue dataValue = obj_root.value("Data");
                if(!dataValue.isNull() && dataValue.isObject()){
                    qDebug() << "Data is Object";
                    QString mqttHost = NULL;
                    QString mqttPort = NULL;
                    QString mqttName = NULL;
                    QString mqttPwd = NULL;
                    QString mqttClientId = NULL;

                    QJsonObject obj_data_root = dataValue.toObject();
                    QJsonValue hostValue = obj_data_root.value("mqtthost");
                    if(!hostValue.isNull() && hostValue.isString()){
                        mqttHost = hostValue.toString();
                    }
                    QJsonValue portValue = obj_data_root.value("mqttport");
                    if(!portValue.isNull()&& portValue.isString()){
                        mqttPort = portValue.toString();
                    }
                    QJsonValue nameValue = obj_data_root.value("username");
                    if(!nameValue.isNull() && nameValue.isString()){
                        mqttName = nameValue.toString();
                    }
                    QJsonValue pwdValue = obj_data_root.value("password");
                    if(!pwdValue.isNull() && pwdValue.isString()){
                        mqttPwd = pwdValue.toString();
                    }
                    QJsonValue clientIdValue = obj_data_root.value("client_id");
                    if(!clientIdValue.isNull() && clientIdValue.isString()){
                        mqttClientId = clientIdValue.toString();
                        curClientId = mqttClientId;
                    }
                    qDebug() << "go to clientMqtt";
                    clientMqtt(mqttHost,mqttPort.toInt(),mqttName,mqttPwd,mqttClientId);
                    return;
                }
                else if(!dataValue.isNull() && dataValue.isString())
                {
                    QJsonDocument jsonDataDoc = QJsonDocument::fromJson(dataValue.toString().toLatin1(),&err_rpt);
                    if (!jsonDataDoc.isNull() && err_rpt.error == QJsonParseError::NoError)
                    {
                        if(jsonDataDoc.isObject()){
                            curAd = jsonDataDoc.object();
                            emit sig_NeedGetLabelAd(curAd);
                            return;
                        }
                    }
                }
            }else if( stateValue.isString()){
                qDebug() << "State fail ==> " << jsonStr;
            }else{
                qDebug() << "State value not string";
            }
        }
    }else{
        qDebug() << err_rpt.errorString();
    }

    sleep(3);
    if(requestStep == 1){
        manager->post(request,curPostBody.toLatin1());
    }else if(requestStep == 2){
        manager->get(request); //发出get请求
    }

}



void mqttrequest::clientMqtt(QString hostName, int dstPort, QString userName, QString pwd, QString clientId) {
    qDebug() << "start reg client Mqtt";
    client = new QMQTT::Client(); // 初始化QMQTT客户指针

    connect(client, &QMQTT::Client::received,this, &mqttrequest::onMQTT_Received,Qt::DirectConnection);
    connect(client, &QMQTT::Client::subscribed, this, &mqttrequest::onMQTT_subscribed,Qt::DirectConnection );
    connect(client, &QMQTT::Client::unsubscribed, this, &mqttrequest::onMQTT_unSubscribed,Qt::DirectConnection);
    connect(client, &QMQTT::Client::error, this, &mqttrequest::onMQTT_error);

    client->setKeepAlive(120); // 心跳
    client->setHost(QHostInfo::fromName(hostName).addresses()[0]); // 设置 EMQ 代理服务器
    client->setPort(dstPort); // 设置 EMQ 代理服务器端口
    client->setClientId(clientId); // 设备 ID
    client->setUsername(userName); // 产品 ID
    client->setPassword(pwd.toLatin1());
    client->cleanSession();
    client->setVersion(QMQTT::MQTTVersion::V3_1_1); // 设置mqtt版本

    client->connectToHost(); // 连接 EMQ 代理服务器

    client->subscribe("",0);
    qDebug() << "subscribe";
    //QTimer::singleShot(1000, this, [=](){
    //    client->subscribe("",0);
    //});
}

void mqttrequest::getDataStrByNetwork(){
    QDateTime timeDate = QDateTime::currentDateTime();  // 获取当前时间
    int timeStr = timeDate .toTime_t();
    QString rawPostStr = getLabelMqtt(uartctrl::Instantialize()->appId, uartctrl::Instantialize()->labelSn, QString::number(curAd.nlast,10), QString::number(timeStr, 10));
    QString getBody = getBodyStr(rawPostStr,getSignStr(rawPostStr,uartctrl::Instantialize()->appKey));
    QString httpGetUrl = "https://esl.ttdh.cc/ItemService/GetItemLabel?" + getBody;
    qDebug() << "Http get message url ==> " << httpGetUrl;
    request.setUrl(QUrl(httpGetUrl));
    requestStep = 2;
    manager->get(request); //发出get请求
}

void mqttrequest::onMQTT_Received(QMQTT::Message message) {
    QString str = message.payload();
    qDebug() << "Raw recv mqtt message ==> " <<"\nTopic: " << message.topic() << "payload:" <<str;

    QJsonParseError err_rpt;
    QJsonDocument jsonRootDoc = QJsonDocument::fromJson(str.toLatin1(),&err_rpt);
    if (!jsonRootDoc.isNull() && err_rpt.error == QJsonParseError::NoError){
        if(jsonRootDoc.isObject()){
            QJsonObject obj_root = jsonRootDoc.object();
            QJsonValue actionValue = obj_root.value("action");
            if(!actionValue.isNull() && actionValue.isString()){
                if( actionValue.toString() == QString::fromLocal8Bit("updatelabel")){
                    getDataStrByNetwork();
                }else if( actionValue.toString() == QString::fromLocal8Bit("updatelabelbydata")){
                    QJsonValue dataValue = obj_root.value("Data");
                    if(!dataValue.isNull() && dataValue.isString())
                    {
                        QJsonDocument jsonDataDoc = QJsonDocument::fromJson(dataValue.toString().toLatin1(),&err_rpt);
                        if (!jsonDataDoc.isNull() && err_rpt.error == QJsonParseError::NoError)
                        {
                            if(jsonDataDoc.isObject()){
                                curAd = jsonDataDoc.object();
                                emit sig_NeedGetLabelAd(curAd);
                            }
                        }
                    }
                }
            }
        }
    }

}

void mqttrequest::onMQTT_subscribed(QString topic, quint8 qos) {
    qDebug() << "\n订阅topic = " << topic << "Qos = " << qos << " 成功!";
}

void mqttrequest::onMQTT_unSubscribed(QString topic) {
    qDebug() << "\nonMQTT_unSubscribed:" << topic;
}

void mqttrequest::onMQTT_error(const QMQTT::ClientError error) {
    qDebug() << "\nonMQTT_unSubscribed:" << error;
}
