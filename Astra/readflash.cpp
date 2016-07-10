#include "readflash.h"

ReadFlash::ReadFlash()
{

}

QByteArray ReadFlash::operation(ComPort *port, QString string)
{
    QByteArray buffer,ba,ba1,buff;
    QString listen = "63ff"+string+"01";//"63ff00000000019d"
    ba+=listen;
    buff = ctrl_sum_xor(ba1.fromHex(ba));
    listen=listen+buff.toHex();
    buffer+=listen;
    uint res = port->write(buffer);
    //buffer = port->read();

    return buffer;
}

QByteArray ReadFlash::ctrl_sum_xor(QByteArray dat) // вычисление контрольной суммы
{
    int i;
    QByteArray ctrl_sum;
    char LRC=dat.at(0);
    for(i=1;i<dat.size();++i)
    LRC=(LRC^dat.at(i));
    ctrl_sum.resize(1);
    ctrl_sum[0]=LRC;
    return ctrl_sum;
}
