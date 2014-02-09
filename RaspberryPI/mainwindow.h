#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <wiringPiSPI.h>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QCloseEvent>
#include <QAction>
#include <QTimer>
#include <stdlib.h>
#include <unistd.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    void pwm(int value);
    qreal adc(int channel);
private slots:

    void on_actionQuit_triggered();

    void closeEvent(QCloseEvent *event);

    void on_trayicon_activated(QSystemTrayIcon::ActivationReason reason);

    void on_tabWidget_currentChanged(int index);

    void on_timer_timeout();

    void on_verticalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    bool q;
    QSystemTrayIcon trayicon;
    QMenu traymenu;
    QIcon rpiicon;
    QIcon quiticon;
    QAction *quitaction;
    QTimer timer;
};

#endif // MAINWINDOW_H
