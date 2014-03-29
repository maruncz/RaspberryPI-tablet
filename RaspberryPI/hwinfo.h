#ifndef HWINFO_H
#define HWINFO_H

#include <QProcess>
#include <QString>
#include <unistd.h>

class hwinfo
{
    QProcess vcgencmd;
    QProcess uname;

public:
    const QString temp="measure_temp";
    const QString arm_freq_min="get_config arm_freq_min";
    const QString arm_freq="get_config arm_freq";
    const QString core_freq_min="get_config core_freq_min";
    const QString core_freq="get_config core_freq";
    const QString overvoltage="get_config over_voltage";
    const QString mem_arm="get_mem arm";
    const QString mem_gpu="get_mem gpu";

    void set_up();
    qreal out_to_qreal(QByteArray in);
    qreal vcgencmd_qreal(QString args);
    QString vcgencmd_qstring(QString args);

    QString from_uname(QString arg);
    QByteArray from_vcdencmd(QString args);
};

#endif // HWINFO_H
