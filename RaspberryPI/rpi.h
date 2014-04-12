#ifndef RPI_H
#define RPI_H

#include <QObject>
#include <QProcess>
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <QMessageBox>
#include <stdlib.h>
#include <unistd.h>

class rpi : public QObject
{
    Q_OBJECT
public:
    explicit rpi(QObject *parent = 0);


    int set_up();
    void pwm(int value);
    qreal adc(int channel);
    void lcd_off();
    char get_ret();
    int s;

    static void interrupt();
    
signals:
    void lcd_off2();

public slots:

private:
    QProcess gpio;
    char r;
    
};



#endif // RPI_H
