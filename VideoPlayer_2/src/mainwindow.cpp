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


    //mRequest->start();

    mPlayer = new VideoPlayer;
    connect(mPlayer,SIGNAL(sig_GetOneFrame(int, int ,QImage)),this,SLOT(slotGetOneFrame(int,int,QImage)));
    connect(mPlayer,SIGNAL(sig_GetOneImage(int, int,int,int ,QPixmap)),this,SLOT(slotGetOneImage(int,int,int,int,QPixmap)));
    mPlayer->start();
    //qDebug() << "main is --> " <<QThread::currentThreadId() << QThread::currentThread();

}


MainWindow::~MainWindow()
{
    delete ui;
    delete mPlayer;
}

void MainWindow::showEvent(QShowEvent *event){
    //mRequest->init();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
        QPainter videoPainter(this);
        videoPainter.setBrush(Qt::black);
        videoPainter.drawRect(0, 0, this->width(), this->height()); //先画成黑色

        if (mImage.size().width() <= 0) return;

        //int wVedio = 650;
        //int hVedio = 360;

        ///将图像按比例缩放成和窗口一样大小
        //QImage img = mImage.scaled(this->size(),Qt::KeepAspectRatio);
        //QImage img = mImage.scaled(wVedio,hVedio,Qt::IgnoreAspectRatio);

        //int x = this->width() - img.width();
        //int y = this->height() - img.height();

        //x /= 2;
        //y /= 2;

        //videoPainter.drawImage(QPoint(x,y),img); //画出图像
         videoPainter.drawImage(QPoint(xVideo,yVideo),mImage);


        QPainter imgPainter(this);
        //QPixmap map = QPixmap("/root/test.jpg");
//        QPixmap map = QPixmap("./test.jpg");
//        int xImg = 0;
//        int yImg = 360;
//        int wImg = 650;
//        int hImg = 440;
//        map.scaled(wImg,hImg,Qt::IgnoreAspectRatio);

        // 启用抗锯齿(反走样)
        //imgPainter.setRenderHint(QPainter::Antialiasing, true);
        // 指定要绘制的图片（将图片路径替换为有效的图片路径）
        //imgPainter.drawPixmap(rect(),map);
        updateImage = false;
        imgPainter.drawPixmap(xImage,yImage,map);
}

void MainWindow::slotGetOneFrame(int x, int y,QImage img)
{
    xVideo = x;
    yVideo = y;
    mImage = img;
    update(); //调用update将执行 paintEvent函数
}


void MainWindow::slotGetOneImage(int x, int y,int width,int heigh, QPixmap img)
{
    map = img;
    xImage = x;
    yImage = y;
    update(); //调用update将执行 paintEvent函数
}
