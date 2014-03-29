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
    s.clear();
}

void idle_detect::on_timer_timeout()
{
    bool ok;
    s.clear();
    idle.start();
    idle.waitForReadyRead(1000);
    s=idle.readAll();
    time=s.toLong();
    if(time>threshold)
    {
        emit idled();
    }
}
