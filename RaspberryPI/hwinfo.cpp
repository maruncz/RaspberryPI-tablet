#include "hwinfo.h"


void hwinfo::set_up()
{
    vcgencmd.setProgram("vcgencmd");
    uname.setProgram("uname");
}

qreal hwinfo::out_to_qreal(QByteArray in)
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

qreal hwinfo::get_gpu_temp()
{
    QStringList in;
    QByteArray out;
    in << "measure_temp";
    out = from_vcdencmd(in);
    return out_to_qreal(out);
}

QString hwinfo::from_uname(QString arg)
{
    QStringList in;
    in << arg;
    uname.setArguments(in);
    uname.start();
    uname.waitForReadyRead(1000);
    return vcgencmd.readAllStandardOutput();
}

QByteArray hwinfo::from_vcdencmd(QStringList args)
{
    vcgencmd.setArguments(args);
    vcgencmd.start();
    vcgencmd.waitForReadyRead(1000);
    return vcgencmd.readAllStandardOutput();
}
