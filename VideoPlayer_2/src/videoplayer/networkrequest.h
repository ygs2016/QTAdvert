#ifndef NETWORKREQUEST_H
#define NETWORKREQUEST_H
#include "videoplayer/advert.h"
#include <QThread>
#include <QDebug>
#include "videoplayer/downloadtool.h"
#include<QtNetwork/QNetworkRequest>
#include<QtNetwork/QNetworkReply>
#include<QtNetwork/QNetworkAccessManager>
#include <QTimer>


class NetworkRequest : public QThread
{
    Q_OBJECT

public:
    explicit NetworkRequest(QObject *parent = nullptr);
    ~NetworkRequest();

    void stop();
    void get();
    void run();
    bool checkAndDownloadFiles();
private slots:
    void getBack(QNetworkReply * reply);
    void slotTime();
    void slotDownloadFinished();
signals:
    void sig_GetNetworkReply(QString Path);
private:
    QString pathName = "./myRes";
    DownloadTool* dT = NULL;
    bool isBusy = false;
    QString mLastUUID;
    QString curPathName;
    bool isRunning = false;
    QTimer *mTime;
    int videoDownloadCount = 0;
    int imageDownloadCount = 0;
    Advert curAd;
    QNetworkAccessManager * manager;
    void Parse_Data_Json(QString jsonStr);
};

#endif // NETWORKREQUEST_H
