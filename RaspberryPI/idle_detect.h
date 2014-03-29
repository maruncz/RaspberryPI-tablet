#ifndef IDLE_DETECT_H
#define IDLE_DETECT_H

#include <QObject>
#include <QProcess>
#include <QTimer>

class idle_detect : public QObject
{
    Q_OBJECT
public:
    explicit idle_detect(QObject *parent = 0);

    QProcess idle;
    QTimer timer;
    long threshold;
    long time;
    QString s;

signals:
    void idled();
    
public slots:

    void on_timer_timeout();
    
};

#endif // IDLE_DETECT_H
