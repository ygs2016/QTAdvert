/**
 * 叶海辉
 * QQ群121376426
 * http://blog.yundiantech.com/
 */


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mRequest = new NetworkRequest;

    connect(mRequest,SIGNAL(sig_GetNetworkReply(QString)),this,SLOT(slotGetNetworkReply(QString )));
    //mRequest->start();
    mRequest->init();

    mPlayer = new VideoPlayer;
    connect(mPlayer,SIGNAL(sig_GetOneFrame(QImage)),this,SLOT(slotGetOneFrame(QImage)));
    //mPlayer->setFileName("/root/test.mp4");
    mPlayer->setFileName("./test.mp4");
    mPlayer->init();
    mPlayer->play();
}


MainWindow::~MainWindow()
{
    delete ui;
    delete mRequest;
    delete mPlayer;
}



void MainWindow::paintEvent(QPaintEvent *event)
{
        QPainter videoPainter(this);
        videoPainter.setBrush(Qt::black);
        videoPainter.drawRect(0, 0, this->width(), this->height()); //先画成黑色

        if (mImage.size().width() <= 0) return;

        int xVedio = 0;
        int yVedio = 0;

        int wVedio = 650;
        int hVedio = 360;

        ///将图像按比例缩放成和窗口一样大小
        //QImage img = mImage.scaled(this->size(),Qt::KeepAspectRatio);
        QImage img = mImage.scaled(wVedio,hVedio,Qt::IgnoreAspectRatio);

        int x = this->width() - img.width();
        int y = this->height() - img.height();

        x /= 2;
        y /= 2;

        //videoPainter.drawImage(QPoint(x,y),img); //画出图像
         videoPainter.drawImage(QPoint(xVedio,yVedio),img);


        QPainter imgPainter(this);
        //QPixmap map = QPixmap("/root/test.jpg");
        QPixmap map = QPixmap("./test.jpg");
        int xImg = 0;
        int yImg = 360;
        int wImg = 650;
        int hImg = 440;
        map.scaled(wImg,hImg,Qt::IgnoreAspectRatio);

        // 启用抗锯齿(反走样)
        imgPainter.setRenderHint(QPainter::Antialiasing, true);
        // 指定要绘制的图片（将图片路径替换为有效的图片路径）
        //imgPainter.drawPixmap(rect(),map);
        imgPainter.drawPixmap(xImg,yImg,map);
}


void MainWindow::slotGetNetworkReply(QString Path)
{
        qDebug() << Path;
}

void MainWindow::slotGetOneFrame(QImage img)
{
    updateVedio = true;
    mImage = img;
    update(); //调用update将执行 paintEvent函数
}
