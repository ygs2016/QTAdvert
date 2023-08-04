#ifndef UARTCTRL_H
#define UARTCTRL_H

#include <QSerialPort>
#include <QThread>



class uartctrl : public QThread
{
    Q_OBJECT

public:
    QString appId = NULL;
    QString appKey = NULL;
    QString labelSn = NULL;
    bool openPort();
    void sendData(QString data);
    void run();
    static uartctrl *Instantialize();
private:
    QString recvData;
    bool isRunning = false;
    void getConfigFromFile();
    void saveConfigToFile();
    bool isFileExist(QString fullFileName);
    QString DoSomeCtrl(QString str);
    static uartctrl *pInstance;
    QSerialPort *port;
    explicit uartctrl(QObject *parent = nullptr);
    uartctrl(const uartctrl &);
    ~uartctrl();
    uartctrl& operator = (const uartctrl &);
private slots:
    void slot_SerialRecvMsgEvent(void);
};


#endif // UARTCTRL_H
