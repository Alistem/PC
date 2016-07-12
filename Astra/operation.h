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
        if(!port->read().endsWith("OkOk") || !port->read().endsWith("OkWR")){
            QString listen = "63";
            buffer+=listen;
            port->write(buffer);
        }
        if(port->read().endsWith("OkOk") || port->read().endsWith("OkWR")){
            QByteArray res2 = operation(port, string);
            return res2;
        }

        return buffer;
    }

    virtual QByteArray operation(ComPort*, QString) = 0;

};

#endif // OPERATION

