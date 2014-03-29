#include "idle_detect.h"

idle_detect::idle_detect(QObject *parent) :
    QObject(parent)
{
    connect(&timer,SIGNAL(timeout()),this,SLOT(on_timer_timeout()));
    idle.setProgram("xprintidle");
    timer.setInterval(1000);
    timer.setSingleShot(false);
    timer.start();
    threshold=60000;
    time=0;
}

void idle_detect::on_timer_timeout()
{
    idle.start();
    idle.waitForReadyRead(1000);
    time=idle.readAll().toLong();
    if(time>threshold)
    {
        emit idled();
    }
}
