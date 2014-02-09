#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <wiringPiSPI.h>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QCloseEvent>
#include <QAction>

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

private:
    Ui::MainWindow *ui;

    bool q;
    QSystemTrayIcon trayicon;
    QMenu traymenu;
    QIcon rpiicon;
    QIcon quiticon;
    QAction *quitaction;
};

#endif // MAINWINDOW_H
