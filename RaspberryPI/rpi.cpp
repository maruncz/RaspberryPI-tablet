#include "rpi.h"

rpi::rpi(QObject *parent) :
    QObject(parent)
{
    int f;
    QString s;
    f=wiringPiSPISetup(0,50000);
    if(f<0)
    {
        QMessageBox::critical(0,"ERROR","Could not initialize SPI");
        r=2;
    }
    f=wiringPiSetupSys();
    if(f<0)
    {
        QMessageBox::critical(0,"ERROR","Could not initialize GPIO");
        r=2;
    }
    gpio.setProgram("gpio");
    gpio.setArguments(QStringList() << "edge" << "17" << "falling");
    gpio.start();
    gpio.waitForReadyRead(1000);
    s=gpio.readAllStandardError();
    if(s.size()>0)
    {
        QMessageBox::critical(0,"GPIO","Could not initialize GPIO"+s);
        gpio.terminate();
        r=2;
    }
    gpio.waitForFinished(1000);
    /*f=wiringPiISR(17,INT_EDGE_SETUP,&rpi::interrupt);
    if(f<0)
    {
        QMessageBox::critical(0,"ERROR","Could not initialize interrupt");
        r=2;
    }*/
    r=0;
}


void rpi::pwm(int value)
{
    unsigned char i=0x40;
    char s;
    s=wiringPiSPIDataRW(0,&i,1);
    usleep(5000);
    i=value;
    s=wiringPiSPIDataRW(0,&i,1);
}

qreal rpi::adc(int channel)
{
    unsigned char i;
    switch(channel)
    {
    case 0:
        channel=0x41;
        break;
    case 1:
        channel=0x42;
        break;
    default:
        return -1;
        break;
    }
    unsigned char v1=0,v2=0,x=0,j=0;
    int v;
    char s;
    qreal volty;
    do
    {
        i=channel;
        s=wiringPiSPIDataRW(0,&i,1);
        usleep(10000);
        s=wiringPiSPIDataRW(0,&v1,1);
        usleep(10000);
        s=wiringPiSPIDataRW(0,&v2,1);
        usleep(10000);
        s=wiringPiSPIDataRW(0,&x,1);
        usleep(10000);
        j++;
    }
    while((x!=(v1^v2))&&(j<5)&&(v1<168));
    v=((v1-168)*256)+v2;
    volty=v;
    volty=2*volty*4.096/1024;
    if((v>1023)||(v<0))
    {
        v=-1;
        volty=-1;
    }
    return volty;
}

void rpi::lcd_off()
{
    unsigned char i=0x44;
    wiringPiSPIDataRW(0,&i,1);
}


char rpi::get_ret()
{
    return r;
}

void rpi::interrupt()
{
    unsigned char i=0x44;
    wiringPiSPIDataRW(0,&i,1);
    usleep(5000);
    wiringPiSPIDataRW(0,&i,1);
    switch(i)
    {
    case 0x40:
        rpi::lcd_off2();
        break;
    }
}


