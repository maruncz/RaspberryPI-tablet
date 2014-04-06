#ifndef RPI_H
#define RPI_H

#include <QProcess>
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <QMessageBox>
#include <stdlib.h>
#include <unistd.h>

class rpi
{
    QProcess gpio;
public:
    int set_up();
    void pwm(int value);
    qreal adc(int channel);
    void lcd_off();

    int d1,d2;

};
static void interrupt();

#endif // RPI_H
