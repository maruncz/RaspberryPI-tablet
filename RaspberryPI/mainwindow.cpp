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
}
