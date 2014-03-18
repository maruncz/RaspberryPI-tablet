#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    q=0;
    rpiicon.addFile(":/icons/rpi",QSize(64,64),QIcon::Normal,QIcon::Off);
    quiticon.addFile(":/icons/quit",QSize(64,64),QIcon::Normal,QIcon::Off);
    trayicon.setIcon(rpiicon);
    quitaction = new QAction(quiticon,"quit",this);
    traymenu.addAction(quitaction);
    connect(quitaction, SIGNAL(triggered()), this, SLOT(on_actionQuit_triggered()));
    connect(&trayicon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(on_trayicon_activated(QSystemTrayIcon::ActivationReason)));
    connect(&timer,SIGNAL(timeout()), this, SLOT(on_timer_timeout()));
    trayicon.setContextMenu(&traymenu);
    trayicon.show();
    timer.setInterval(1000);
    timer.setSingleShot(false);
    timer.start();
    hw.set_up();
    int i;
    q=gpio.set_up();
    if(q>0)
    {
        close();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionQuit_triggered()
{
    q=1;
    close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    switch(q)
    {
    case 0:
        timer.stop();
        hide();
        trayicon.showMessage("Minimized to tray","Minimized to tray", QSystemTrayIcon::Information,3000);
        event->ignore();
        break;
    case 1:
        switch(QMessageBox::question(this,"Quit?","Do you really want to quit?",QMessageBox::Yes,QMessageBox::No))
        {
        case QMessageBox::Yes:
            event->accept();
            break;
        case QMessageBox::No:
            event->ignore();
            q=0;
            break;
        }
        break;
    case 2:
        event->accept();
        break;

    }

}

void MainWindow::on_trayicon_activated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        show();
        timer.start();
        break;
    default:
        break;
    }
}

void MainWindow::on_timer_timeout()
{
    ui->lcdNumber->display(gpio.adc(0));
    ui->lcdNumber_4->display(gpio.adc(1));
    ui->lcdNumber_3->display(hw.get_gpu_temp());
}

int MainWindow::rpi::set_up()
{
    int f;
    QString s;
    f=wiringPiSPISetup(0,50000);
    if(f<0)
    {
        QMessageBox::critical(0,"ERROR","Could not initialize SPI");
        return 2;
    }
    f=wiringPiSetupSys();
    if(f<0)
    {
        QMessageBox::critical(0,"ERROR","Could not initialize GPIO");
        return 2;
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
        return 2;
    }
    gpio.waitForFinished(1000);
    f=wiringPiISR(17,INT_EDGE_SETUP,interrupt);
    if(f<0)
    {
        QMessageBox::critical(0,"ERROR","Could not initialize interrupt");
        return 2;
    }
    return 0;

}

void MainWindow::rpi::pwm(int value)
{
    unsigned char i=0x40;
    char s;
    s=wiringPiSPIDataRW(0,&i,1);
    usleep(5000);
    i=value;
    s=wiringPiSPIDataRW(0,&i,1);
}

qreal MainWindow::rpi::adc(int channel)
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


void MainWindow::interrupt()
{

}


void MainWindow::hwinfo::set_up()
{
    vcgencmd.setProgram("vcgencmd");
    uname.setProgram("uname");
}

qreal MainWindow::hwinfo::out_to_qreal(QByteArray in)
{
    QString out;
    int i;
    for(i=0;i<in.size();i++)
    {
        if(((in[i]>='0')&&(in[i]<='9'))||(in[i]=='.'))
        {
            out.append(in.at(i));
        }
    }
    return out.toFloat();
}

qreal MainWindow::hwinfo::get_gpu_temp()
{
    QStringList in;
    QByteArray out;
    in << "measure_temp";
    out = from_vcdencmd(in);
    return out_to_qreal(out);
}


QString MainWindow::hwinfo::from_uname(QString arg)
{
    QStringList in;
    in << arg;
    uname.setArguments(in);
    uname.start();
    uname.waitForReadyRead(1000);
    return vcgencmd.readAllStandardOutput();
}

QByteArray MainWindow::hwinfo::from_vcdencmd(QStringList args)
{
    vcgencmd.setArguments(args);
    vcgencmd.start();
    vcgencmd.waitForReadyRead(1000);
    return vcgencmd.readAllStandardOutput();
}

void MainWindow::on_actionInfo_triggered()
{
    info_screen *i = new info_screen(this);
    i->show();
    i->raise();
    i->activateWindow();
}
