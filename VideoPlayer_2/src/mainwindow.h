/**
 * 叶海辉
 * QQ群121376426
 * http://blog.yundiantech.com/
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <QPaintEvent>

#include <videoplayer/uartctrl.h>

#include <videoplayer/networkrequest.h>
#include "hwplayer/hwplayer.h"
//#include "videoplayer/videoplayerhw.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::MainWindow *ui;
    int xVideo = 0;
    int yVideo = 0;
    int widthVideo = 0;
    int heighVideo = 0;
    QPixmap map;
    int xImage = 0;
    int yImage = 0;
    hwplayer mPlayer; //播放线程
    QLabel mLabel;
    QImage mImage; //记录当前的图像
    bool updateImage = false;
    bool updateVedio = false;
private slots:
    void slotGetOneFrame(int x, int y,int width, int heigh);
    void slotGetOneImage(int x, int y, int width, int heigh, QPixmap img);

};

#endif // MAINWINDOW_H
