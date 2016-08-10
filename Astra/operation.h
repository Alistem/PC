#ifndef OPERATION
#define OPERATION
#include <QtCore>

#include "comport.h"

class Operation
{
public:
    Operation() {}
    virtual ~Operation () {}

    QByteArray sendCommandToPort(ComPort* port, QString string)
    {
        QByteArray buffer;
        // Работает только с успешно подключенным COM портом
        if (! port->portOpen())
            return buffer;
//        if(!port->read().endsWith("OkOk") /*|| !port->read().endsWith("OkWR")*/){

//        }
        if(port->read().contains("OkOk") || port->read().endsWith("OkWR")){
            qDebug()<<port->read().right(4);
            QByteArray res2 = operation(port, string);
            return res2;
        }
        else{
            qDebug()<<"63";
            QString listen = "63";
            buffer+=listen;
            port->write(buffer);
        }

        return buffer;
    }

    virtual QByteArray operation(ComPort*, QString) = 0;

};

#endif // OPERATION

