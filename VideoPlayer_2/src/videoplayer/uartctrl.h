#ifndef UARTCTRL_H
#define UARTCTRL_H

#include <QThread>



class uartctrl : public QThread
{
    Q_OBJECT

public:
    explicit uartctrl(QObject *parent = nullptr);
    ~uartctrl();
    void run();
};


#endif // UARTCTRL_H
