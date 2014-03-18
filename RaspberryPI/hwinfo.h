#ifndef HWINFO_H
#define HWINFO_H

#include <QProcess>

class hwinfo
{
    QProcess vcgencmd;
    QProcess uname;
public:
    void set_up();
    qreal out_to_qreal(QByteArray in);
    qreal get_gpu_temp();
    QString from_uname(QString arg);
    QByteArray from_vcdencmd(QStringList args);
};

#endif // HWINFO_H
