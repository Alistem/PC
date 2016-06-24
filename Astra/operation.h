#ifndef OPERATION
#define OPERATION
#include <QtCore>

#include "comport.h"

class Operation : public QObject
{
public:
    Operation() {}
    virtual ~Operation () {}

    QByteArray sendCommandToPort(ComPort* port, QString string)
    {
        QString listen = "63";
        QByteArray buffer;
        buffer+=listen;
        uint res = port->write(buffer);
        // Проверка возврата OkOk

        QByteArray res2 = operation(port, string);
        return res2;

    }

    virtual QByteArray operation(ComPort*, QString) = 0;

};

#endif // OPERATION

