#include "idle_detect.h"

idle_detect::idle_detect(QObject *parent) :
    QObject(parent)
{
    connect(&timer,SIGNAL(timeout()),this,SLOT(on_timer_timeout()));
    idle.setProgram("xprintidle");
    waker.setProgram("xdotool");
    waker.setArguments(QStringList("click 1"));
    locker.setProgram("gnome-screensaver-command");
    locker.setArguments(QStringList("-l"));
    timer.setInterval(1000);
    timer.setSingleShot(false);
    timer.start();
    threshold=60000;
    time=0;
    s.clear();
}

void idle_detect::lock()
{
    locker.start();
    locker.waitForFinished(1000);
}

void idle_detect::wake()
{
    waker.start();
    waker.waitForFinished(1000);
}

void idle_detect::on_timer_timeout()
{
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
