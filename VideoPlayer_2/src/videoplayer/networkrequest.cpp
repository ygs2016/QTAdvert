#include "networkrequest.h"
#include "videoplayer/downloadtool.h"
#include "videoplayer/advert.h"
#include<QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QFile>
#include <QDirIterator>

NetworkRequest::NetworkRequest(QObject *parent)
{


}

NetworkRequest::~NetworkRequest()
{
    isRunning = false;
    if(dT){
        dT->cancelDownload();
        delete dT;
        dT = NULL;
    }
//    if(manager != NULL){
//        delete manager;
//        manager = NULL;
//    }
}

void NetworkRequest::run(){
    //qDebug() << "NetworkReques is --> " <<QThread::currentThreadId() << QThread::currentThread();

    isRunning = true;
//    manager = new QNetworkAccessManager();
//    connect(manager,SIGNAL(finished(QNetworkReply *)),this,SLOT(getBack(QNetworkReply *)),Qt::DirectConnection);//通信完成后，自动执行getBack
//    connect(&mTime,SIGNAL(timeout()),this,SLOT(slotTime()),Qt::DirectConnection);
//    mTime.start(2000);
    checkAndDownloadFiles();
    connect(&mqttManager,SIGNAL(sig_NeedGetLabelAd(Advert)),this,SLOT(slotGetDataAd(Advert)));
    mqttManager.start();
    exec();

}

void NetworkRequest::slotDownloadFinished(){
    qDebug() << dT->getFileName() + "Download done !!!";
    QString  subPathName = curAd.id;
    if(dT != NULL){
        dT->disconnect(dT,0,0,0);
        delete dT;
        dT = NULL;
    }
    if( ++videoDownloadCount < curAd.labelVideo.VideoList.count()){
        dT = new DownloadTool(curAd.labelVideo.VideoList[videoDownloadCount].videoUrl, pathName + "/" + subPathName + "/Video/");
        dT->startDownload();
        //dT->sigDownloadFinished();
        connect(dT,SIGNAL(sigDownloadFinished()),this,SLOT(slotDownloadFinished()));
    }else if(imageDownloadCount++ < 1){
        dT = new DownloadTool(curAd.labelPicture.pictureUrl, pathName + "/" + subPathName + "/Picture/");
        dT->startDownload();
        //dT->sigDownloadFinished();
        connect(dT,SIGNAL(sigDownloadFinished()),this,SLOT(slotDownloadFinished()));
    }else{
        imageDownloadCount = 0;
        videoDownloadCount = 0;
        QFile file(pathName +"/"+ subPathName+ "/JsonRawData.txt");
        file.open(QIODevice::WriteOnly|QIODevice::Text);
        file.write(curAd.getJson().toLatin1());
        file.close();
        emit sig_GetNetworkReply(pathName + "/" + subPathName);
        isBusy = false;

    }
}

//void NetworkRequest::slotTime()
//{
//    //qDebug() << "tiemr is --> " <<QThread::currentThreadId() << QThread::currentThread();
//    if(isRunning){
//        get();
//    }
//}

//void NetworkRequest::get()
//{
//    request.setUrl(QUrl("http://114.215.83.240:6366/ItemService/GetItemLabel?Token=123&ClientId=‎74-D4-35-65-5A-22&UserID=Admin&Nlast=0"));
//    manager->get(request); //发出get请求
//}



bool NetworkRequest::checkAndDownloadFiles(){
    if(isBusy){
        qDebug() << "Is busy!!!";
        return false;
    }
    isBusy = true;
    QString subPathName;
    bool foundPath = false;
    if(QDir().exists(pathName)){
        QDirIterator it(pathName, QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::AllDirs, QDirIterator::NoIteratorFlags);
        while(it.hasNext()){
            it.next();
            //qDebug() << "Found path ==> " + it.filePath();
            if(QFile().exists(it.filePath() + "/JsonRawData.txt")){
                if(!curAd.id.isNull() && !curAd.id.isEmpty()){
                    if( !curAd.id.compare(it.fileName())){
                        subPathName = it.fileName();
                        foundPath = true;
                        break;
                    }
                }else{
                    subPathName = it.fileName();
                    foundPath = true;
                    break;
                }
            }else{
                QDir(it.filePath()).removeRecursively();
                //qDebug() << "Delete path ==> " + it.filePath();
            }
        }
    }else{
        QDir().mkpath(pathName);
        //QDir().mkpath(pathName + "/Picture");
        //QDir().mkpath(pathName + "/Video");
    }

    if(curAd.id.isNull() || curAd.id.isEmpty()){
        if(foundPath){
            mLastUUID = subPathName;
            emit sig_GetNetworkReply(pathName + "/" + subPathName);
        }
        isBusy = false;
    }else if(!foundPath ||
               subPathName.isNull() ||
               subPathName.isEmpty() ||
               subPathName.compare(curAd.id)){
        subPathName = curAd.id;
        videoDownloadCount = 0;
        imageDownloadCount = 0;
        if(dT != NULL){
            dT->disconnect(dT,0,0,0);
            printf("Free DT ....\n");
            delete dT;
        }
        printf("New DT ....\n");
        dT = new DownloadTool(curAd.labelVideo.VideoList[videoDownloadCount].videoUrl, pathName +"/"+ subPathName+ "/Video/");
        dT->startDownload();
        //dT->sigDownloadFinished();
        connect(dT,SIGNAL(sigDownloadFinished()),this,SLOT(slotDownloadFinished()));
        //delete dT;
        //dT = NULL;
        //}

    }else{
        isBusy = false;
    }
    return false;

}



void NetworkRequest::Parse_Data_Json(QString jsonStr)
{
    QJsonParseError err_rpt;
    QJsonDocument jsonRootDoc = QJsonDocument::fromJson(jsonStr.toLatin1(),&err_rpt);
    if (!jsonRootDoc.isNull() && err_rpt.error == QJsonParseError::NoError){
        if(jsonRootDoc.isObject()){
            QJsonObject obj_root = jsonRootDoc.object();
            QJsonValue stateValue = obj_root.value("State");
            if(stateValue.isString() && stateValue.toString() == QString::fromLocal8Bit("Done")){
                QJsonValue dataValue = obj_root.value("Data");
                if(!dataValue.isNull() && dataValue.isString()){
                    QJsonDocument jsonDataDoc = QJsonDocument::fromJson(dataValue.toString().toLatin1(),&err_rpt);
                    if (!jsonDataDoc.isNull() && err_rpt.error == QJsonParseError::NoError){
                        if(jsonDataDoc.isObject()){
                            curAd = jsonDataDoc.object();
                            checkAndDownloadFiles();
                            //QThread::sleep(1);
                            //qDebug() << "sleep is --> " <<QThread::currentThreadId() << QThread::currentThread();
                        }
                    }
                }

            }
        }
    }
}

void NetworkRequest::praseDataStr(QString dataStr){
    QJsonParseError err_rpt;
    QJsonDocument jsonDataDoc = QJsonDocument::fromJson(dataStr.toLatin1(),&err_rpt);
    if (!jsonDataDoc.isNull() && err_rpt.error == QJsonParseError::NoError){
        if(jsonDataDoc.isObject()){
            curAd = jsonDataDoc.object();
            checkAndDownloadFiles();
        }
    }
}

void NetworkRequest::slotGetDataAd(Advert ad){
    curAd = ad;
    checkAndDownloadFiles();
}

void NetworkRequest::getBack(QNetworkReply * reply)
{
    Parse_Data_Json(reply->readAll().data());
}
