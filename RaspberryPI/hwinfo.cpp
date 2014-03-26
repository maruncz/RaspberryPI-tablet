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

qreal hwinfo::vcgencmd_qreal(QString args)
{
    return out_to_qreal(from_vcdencmd(args));
}

QString hwinfo::vcgencmd_qstring(QString args)
{
    return QString::number(out_to_qreal(from_vcdencmd(args)));
}


QString hwinfo::from_uname(QString arg)
{
    QStringList in;
    QByteArray out;
    out.clear();
    in << arg;
    uname.setArguments(in);
    uname.start();
    do
    {
        uname.waitForReadyRead(1000);
        out.append(uname.readAllStandardOutput());
    }
    while(!uname.atEnd());
    uname.waitForFinished(1000);
    return out;
}

QByteArray hwinfo::from_vcdencmd(QString args)
{
    QStringList in;
    QByteArray out;
    out.clear();
    in << args;
    vcgencmd.setArguments(in);
    vcgencmd.start();
    do
    {
        vcgencmd.waitForReadyRead(1000);
        out.append(vcgencmd.readAllStandardOutput());
    }
    while(!vcgencmd.atEnd());
    vcgencmd.waitForFinished(1000);
    return out;
}
