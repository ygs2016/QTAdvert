#include "uartctrl.h"

#include <QJsonDocument>
#include <QJsonObject>

#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>

uartctrl::uartctrl(QObject *parent)
{
    getConfigFromFile();
}

uartctrl::~uartctrl()
{
    if(port!=NULL){
        delete port;
        port = NULL;
    }
}

void uartctrl::getConfigFromFile(){
    if(isFileExist("./confJson.txt")){
        QString confStr;
        QFile configFile("./confJson.txt");
        if(!configFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Open config file failed.";
            return ;
        }

        QTextStream txtInput(&configFile);

        confStr = txtInput.readAll();

        qDebug() << "Raw conf data ==> " << confStr;
        QJsonParseError err_rpt;
        qDebug() << "Start to prase conf json";
        QJsonDocument jsonRootDoc = QJsonDocument::fromJson(confStr.toLatin1(),&err_rpt);
        if (!jsonRootDoc.isNull() && err_rpt.error == QJsonParseError::NoError){
            qDebug() << "prase no error";
            if(jsonRootDoc.isObject()){
                qDebug() << "jsonRoot is Object";
                QJsonObject obj_root = jsonRootDoc.object();

                QJsonValue appIDValue = obj_root.value("AppID");
                if(appIDValue.isString()){
                    appId = appIDValue.toString();
                    qDebug() << "app value is " << appId;
                }else{
                    qDebug() << "app value not string";
                }

                QJsonValue appKeyValue = obj_root.value("AppKey");
                if(appKeyValue.isString()){
                    appKey = appKeyValue.toString();
                    qDebug() << "app key value is " << appKey;
                }else{
                    qDebug() << "app key value not string";
                }

                QJsonValue labelSn = obj_root.value("LabelSN");
                if(labelSn.isString()){
                    labelSn = labelSn.toString();
                    qDebug() << "labelSn value is " << labelSn;
                }else{
                    qDebug() << "labelSn value not string";
                }
            }
        }else{
            qDebug() << err_rpt.errorString();
        }
    }
}
void uartctrl::saveConfigToFile(){

    QJsonObject obj_root;
    obj_root.insert("AppID",appId);
    obj_root.insert("AppKey",appKey);
    obj_root.insert("LabelSN",labelSn);

    QJsonDocument jsonDocu(obj_root);
    QByteArray jsonData = jsonDocu.toJson();

    QFile file("./confJson.txt");
    if(file.open(QIODevice::WriteOnly)){
        file.write(jsonData);
        file.close();
    }
}


bool uartctrl::isFileExist(QString fullFileName)
{
    QFileInfo fileInfo(fullFileName);
    if(fileInfo.isFile())
    {
        return true;
    }
    return false;
}

QMutex m_hCriticalSection; /* 临界区 */
uartctrl* uartctrl::pInstance = 0;
uartctrl *uartctrl::Instantialize()
{
    if(pInstance == NULL) //double check
    {
        m_hCriticalSection.lock(); //只有当pInstance等于null时，才开始使用加锁机制 二次检查
        if(pInstance == NULL)
        {
            pInstance = new uartctrl();
        }
        m_hCriticalSection.unlock();
    }
    return pInstance;
}

bool uartctrl::openPort(){
    port = new QSerialPort("ttyS1");//通过名字打开，可通过上面的list里查找
    /*根据需求对串口进行进行配置*/
    port->setBaudRate(QSerialPort::Baud115200);//配置波特率位115200
    port->setDataBits(QSerialPort::Data8);//配置数据格式为8bit
    port->setParity(QSerialPort::NoParity);//无校验位
    port->setStopBits(QSerialPort::OneStop);//一个停止位
    port->setFlowControl(QSerialPort::NoFlowControl);//不使用流控制
    /*打开设备*/
    if (!port->open(QIODevice::ReadWrite))
    {
        qDebug() << "serial open failed";
        return false;
    }
    else
    {
        qDebug() << "serial open success";
    }
    /*设置接收缓冲区大小*/
    port->setReadBufferSize(2048);
    /*绑定接受槽函数，但收到信息是将调用MainSerialRecvMsgEvent函数*/
    QObject::connect(port, &QSerialPort::readyRead, this, &uartctrl::slot_SerialRecvMsgEvent,Qt::DirectConnection);
    return true;
}

/*数据接收*/
void uartctrl::slot_SerialRecvMsgEvent(void)
{
    QByteArray MainSerialRecvData;//保存串口数据的数组
    if(port->bytesAvailable()>0)//判断等到读取的数据大小
    {
        MainSerialRecvData = port->readAll();//读取所有的接收数据
        if(!MainSerialRecvData.isEmpty()){
            qDebug() <<MainSerialRecvData.size();//打印数据大小
            recvData += MainSerialRecvData;
            qDebug() << "serial recv ==>" << recvData;
            sendData(recvData);
            if(recvData.length() > 1024 || recvData.contains("\r\n")){
                qDebug() << "serial handle ==>" << recvData;
                DoSomeCtrl(recvData);
                recvData.clear();
            }
//            for(int i = 0; i < MainSerialRecvData.size();i++)
//            {
//                qDebug() <<MainSerialRecvData.at(i);
//            }
        }else{
            qDebug()<<"接受数据出错";
        }
    }
}

/*数据发送*/
void uartctrl::sendData(QString data)
{
    if(port!=NULL&&port->isOpen())
    {
        port->write(data.toLatin1());
    }
    else
    {
        qDebug() <<"串口未打开！";
    }
}

void uartctrl::run(){
    openPort();
    exec();
}

QString uartctrl::DoSomeCtrl(QString str){
    QString ret = "";
    bool isNeedReboot = false;;
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
                        conf += "network={\n scan_ssid=1 \n ssid=\\\"";
                        conf += ssid.toString();
                        conf += "\\\"\n";
                        conf += "psk=\\\"";
                        conf += pwd.toString();
                        conf += "\\\"\n";
                        conf += "}";
                        //system("/config/wifi/ssw01bInit.sh");
                        //system("ifconfig wlan0 up");
                        system("echo \"" + conf.toLatin1() + "\">/appconfigs/wpa_supplicant.conf" );
                        //system("echo \"" + conf.toLatin1() + "\">/customer/wpa_supplicant.conf" );
                        //system("killall wpa_supplicant");
                        //system("/config/wifi/wpa_supplicant -D nl80211 -i wlan0 -c /appconfigs/wpa_supplicant.conf -B &");
                        ret += "wifi set finish!\n";
			isNeedReboot = true;
                    }
                }
            }

            if (object.contains("Remote-server")) {  // 包含指定的 key
                QJsonValue value = object.value("Remote-server");  // 获取指定 key 对应的 value
                if (value.isString()) {  // 判断 value 是否为字符串
                    QString server = value.toString();

                }
            }

            if (object.contains("AppID")) {  // 包含指定的 key
                QJsonValue value = object.value("AppID");  // 获取指定 key 对应的 value
                if (value.isString()) {  // 判断 value 是否为字符串
                    appId = value.toString();

                }
            }


            if (object.contains("AppKey")) {  // 包含指定的 key
                QJsonValue value = object.value("AppKey");  // 获取指定 key 对应的 value
                if (value.isString()) {  // 判断 value 是否为字符串
                    appKey = value.toString();
                }
            }

            if (object.contains("LabelSN")) {  // 包含指定的 key
                QJsonValue value = object.value("LabelSN");  // 获取指定 key 对应的 value
                if (value.isString()) {  // 判断 value 是否为字符串
                    labelSn = value.toString();
                }
            }
	    qDebug() << "Save config to file ";
            saveConfigToFile();

        }
    }
    if(isNeedReboot){
	qDebug() << "Need to reboot !!!";
	int pid = qApp->applicationPid();
       QString killCmd;
        killCmd = QString("kill -9 %1;reboot;").arg(pid);
	system(killCmd.toLatin1());
	//system("reboot");
//	int pid = qApp->applicationPid();
//       QString killCmd;
//        killCmd = QString("kill -9 %1; reboot;").arg(pid);
//       QProcess::startDetached(killCmd);
    }
    return QString("");
}
