#ifndef HWINFO_H
#define HWINFO_H

#include <QProcess>
#include <QString>

class hwinfo
{
    QProcess vcgencmd;
    QProcess uname;
public:
    void set_up();
    qreal out_to_qreal(QByteArray in);
    qreal get_gpu_temp();
    QString from_uname(QString arg);
    QByteArray from_vcdencmd(QString args);
    const QString temp="measure_temp";
    const QString get_conf="get_config int";
    const QString mem_arm="get_mem arm";
    const QString mem_gpu="get_mem gpu";
};

#endif // HWINFO_H
