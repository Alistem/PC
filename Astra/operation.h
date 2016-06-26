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

        QString listen = "63";
        buffer+=listen;
        port->write(buffer);
        // Проверка возврата OkOk

        QByteArray res2 = operation(port, string);
        return res2;

    }

    virtual QByteArray operation(ComPort*, QString) = 0;

};

#endif // OPERATION

