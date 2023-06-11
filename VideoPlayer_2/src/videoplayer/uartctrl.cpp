#include "uartctrl.h"

#include <QJsonDocument>
#include <QJsonObject>

uartctrl::uartctrl(QObject *parent)
{

}


void uartctrl::run(){


}

QString DoSomeCtrl(QString str){
    QString ret = "";
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(str.toLatin1(), &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
        if (doucment.isObject()) { // JSON 文档为对象
            QJsonObject object = doucment.object();  // 转化为对象
            if (object.contains("WIFI")) {  // 包含指定的 key
                QJsonValue value = object.value("WIFI");  // 获取指定 key 对应的 value
                if (value.isObject()) {  // 判断 value 是否为字符串
                    QJsonObject wifiObject = value.toObject();  // 将 value 转化为字符串
                    if(wifiObject.contains("ssid") && wifiObject.contains("passwd"))
                    {
                        QJsonValue ssid = wifiObject.value("ssid");
                        QJsonValue pwd = wifiObject.value("passwd");
                        QString conf;
                        conf += "ctrl_interface=/tmp/wifi/run/wpa_supplicant\n";
                        conf += "update_config=1\n";
                        conf += "network={\n scan_ssid=1 \n ssid=\"";
                        conf += ssid.toString();
                        conf += "\"\n";
                        conf += "psk=\"";
                        conf += pwd.toString();
                        conf += "\"\n";
                        conf += "}";
                        system("/config/wifi/ssw01bInit.sh");
                        system("ifconfig wlan0 up");
                        system("echo " + conf.toLatin1() + ">/appconfigs/wpa_supplicant.conf" );
                        system("/config/wifi/wpa_supplicant -D nl80211 -i wlan0 -c /appconfigs/wpa_supplicant.conf -B &");
                        ret += "wifi set finish!\n";
                    }
                }
            }

            if (object.contains("Remote-server")) {  // 包含指定的 key
                QJsonValue value = object.value("Remote-server");  // 获取指定 key 对应的 value
                if (value.isString()) {  // 判断 value 是否为字符串
                    QString server = value.toString();

                }
            }


        }
    }
}
