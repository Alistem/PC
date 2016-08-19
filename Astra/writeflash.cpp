#include "writeflash.h"

WriteFlash::WriteFlash()
{

}

QByteArray WriteFlash::operation(IAdapterInterface *port, QString string)
{
    if(!port->read().endsWith("OkWR")){
    QByteArray buffer,ba,ba1,buff;
    QString sector,templ;
    int i,m;
    templ="00000000";
    m = string.toInt()*512;
    sector.setNum(m,16);

    for(i=0;i<sector.size();++i){

        templ.replace(templ.size()-1-i,1,sector.at(sector.size()-1-i));
    }

    sector = templ;

    templ="00000000";

    QString listen = "63ff"+sector+"02"; //"63ff00000000019d"

    ba+=listen;
    buff = ctrl_sum_xor(ba1.fromHex(ba));
    listen=listen+buff.toHex();
    buffer+=listen;
    port->write(buffer);
    qDebug()<<buffer;
    return buffer;
    }
    else{
        QByteArray buffer;
        buffer+=string;
        //qDebug()<<buffer;
        port->write(buffer);

        return buffer;
    }
}

QByteArray WriteFlash::ctrl_sum_xor(QByteArray dat) // вычисление контрольной суммы
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
