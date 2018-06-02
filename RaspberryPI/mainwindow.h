#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "info_screen.h"
#include "hwinfo.h"
#include "rpi.h"
#include "idle_detect.h"
#include <QMainWindow>

#include <QMenu>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QCloseEvent>
#include <QAction>
#include <QTimer>
#include <stdlib.h>
#include <unistd.h>
#include <QProcess>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    

private slots:

    void on_actionQuit_triggered();

    void closeEvent(QCloseEvent *event);

    void on_trayicon_activated(QSystemTrayIcon::ActivationReason reason);

    void on_timer_timeout();

    void on_actionInfo_triggered();

    void on_idled();

    void on_lock();

    void on_wake();

    void on_timer_on();

    void on_timer_off();

    void on_shutdown();

    void on_idle_set_clicked();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon trayicon;
    QMenu traymenu;
    QIcon rpiicon;
    QIcon quiticon;
    QAction *quitaction;
    QTimer timer;
    int q;
    idle_detect *idle;

    rpi gpio;

    hwinfo hw;

    QProcess shutdown;




};


#endif // MAINWINDOW_H
