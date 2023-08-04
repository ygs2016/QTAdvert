
/**
 * 叶海辉
 * QQ群121376426
 * http://blog.yundiantech.com/
 */

#include <QCoreApplication>
#include <QTextCodec>
#include <QSslSocket>
#include <videoplayer/uartctrl.h>

#include "hwplayer/hwplayer.h"
//#include "mainwindow.h"

int main(int argc, char *argv[])
{
    if(argc == 2 && argv[1] != nullptr){
        if(QString("preInstall").compare(argv[1],Qt::CaseInsensitive)){
            return 3266;
        }
    }

    bool bSupp = QSslSocket::supportsSsl();
    QString buildVersion = QSslSocket::sslLibraryBuildVersionString();
    QString version = QSslSocket::sslLibraryVersionString();
    qDebug() << bSupp << buildVersion << version ;

    QCoreApplication a(argc, argv);
    setbuf(stdout, NULL);

    uartctrl::Instantialize()->start();

    //mRequest->start();
    //connect(&mPlayer,SIGNAL(sig_GetOneFrame(int, int ,int, int )),this,SLOT(slotGetOneFrame(int,int,int , int)));
    //connect(&mPlayer,SIGNAL(sig_GetOneImage(int, int,int,int ,QPixmap)),this,SLOT(slotGetOneImage(int,int,int,int,QPixmap)));
    hwplayer mPlayer; 
    mPlayer.start();


//    MainWindow w;
//    w.show();

    return a.exec();
}

