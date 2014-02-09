#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    q=false;
    char f;
    f=wiringPiSPISetup(0,61000);
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
    if(ui->tabWidget->currentIndex()==1)
    {
        timer.start();
    }
    hw.set_up();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionQuit_triggered()
{
    q=true;
    close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(q)
    {
        switch(QMessageBox::question(this,"Quit?","Do you really want to quit?",QMessageBox::Yes,QMessageBox::No))
        {
        case QMessageBox::Yes:
            event->accept();
            break;
        case QMessageBox::No:
            event->ignore();
            q=false;
            break;
        }
    }
    else
    {
        hide();
        trayicon.showMessage("In tray","Application was minimized to tray",QSystemTrayIcon::Information,1000);
        event->ignore();
    }
}

void MainWindow::on_trayicon_activated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        MainWindow::show();
        break;
    default:
        break;
    }
}




void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(index==1)
    {
        timer.start();
    }
    else
    {
        timer.stop();
    }
}

void MainWindow::on_timer_timeout()
{
    ui->lcdNumber->display(adc(0));
    ui->lcdNumber_2->display(hw.get_cpu_temp());
    ui->lcdNumber_3->display(hw.get_gpu_temp());
}

void MainWindow::pwm(int value)
{
    unsigned char i=0x40;
    char s;
    s=wiringPiSPIDataRW(0,&i,1);
    usleep(5000);
    i=value;
    s=wiringPiSPIDataRW(0,&i,1);
}

qreal MainWindow::adc(int channel)
{
    unsigned char i=0x41;
    unsigned char v1=0,v2=0,x=0,j=0;
    int v;
    qreal volty;
    char s;
    do
    {
        s=wiringPiSPIDataRW(0,&i,1);
        usleep(5000);
        s=wiringPiSPIDataRW(0,&v1,1);
        usleep(5000);
        s=wiringPiSPIDataRW(0,&v2,1);
        usleep(5000);
        s=wiringPiSPIDataRW(0,&x,1);
        usleep(5000);
        j++;
    }
    while((x!=(v1^v2))&&(j<5));
    v=((v1-168)*256)+v2;
    volty=v;
    volty=volty*5/1024;
    return volty;
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    pwm(value);
    ui->pwm->setText(QString::number(value));
    ui->proc->setText(QString::number((qreal(value)/256)*100)+"%");
}


void MainWindow::hwinfo::set_up()
{
    cpu_temp.setFileName("/sys/class/thermal/thermal_zone0/temp");
    vcgencmd.setProgram("vcgencmd");
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

qreal MainWindow::hwinfo::get_cpu_temp()
{
    QString tmp;
    qreal temp;
    if(cpu_temp.open(QIODevice::ReadOnly))
    {
        tmp=cpu_temp.readAll();
        temp=tmp.toFloat()/1000;
    }
    else
    {
        temp=-1;
    }
    cpu_temp.close();
    return temp;
}

qreal MainWindow::hwinfo::get_gpu_temp()
{
    QStringList in;
    QByteArray out;
    in << "measure_temp";
    out = from_vcdencmd(in);
    return out_to_qreal(out);
}

QByteArray MainWindow::hwinfo::from_vcdencmd(QStringList args)
{
    vcgencmd.setArguments(args);
    vcgencmd.start();
    vcgencmd.waitForReadyRead(1000);
    return vcgencmd.readAllStandardOutput();
}
