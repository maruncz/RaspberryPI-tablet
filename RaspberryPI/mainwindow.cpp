#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    q=0;
    idle = new idle_detect;
    rpiicon.addFile(":/icons/rpi",QSize(64,64),QIcon::Normal,QIcon::Off);
    quiticon.addFile(":/icons/quit",QSize(64,64),QIcon::Normal,QIcon::Off);
    trayicon.setIcon(rpiicon);
    quitaction = new QAction(quiticon,"quit",this);
    traymenu.addAction(quitaction);
    connect(quitaction, SIGNAL(triggered()), this, SLOT(on_actionQuit_triggered()));
    connect(&trayicon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(on_trayicon_activated(QSystemTrayIcon::ActivationReason)));
    connect(&timer,SIGNAL(timeout()), this, SLOT(on_timer_timeout()));
    connect(idle,SIGNAL(idled()),this,SLOT(on_idled()));
    trayicon.setContextMenu(&traymenu);
    trayicon.show();
    timer.setInterval(1000);
    timer.setSingleShot(false);
    timer.start();
    hw.set_up();
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
    ui->lcdNumber_3->display(hw.vcgencmd_qreal(hw.temp));
    ui->label_3->setText(QString::number(idle->time));
}




void MainWindow::on_actionInfo_triggered()
{
    info_screen *i = new info_screen(this);
    timer.stop();
    i->show();
    timer.start();
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    gpio.pwm(value);
}

void MainWindow::on_idled()
{
    idle->lock();
    gpio.lcd_off();
}

void MainWindow::on_pushButton_clicked()
{
    gpio.lcd_off();
}
