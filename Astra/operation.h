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
        port->write(buffer);
        // Проверка возврата OkOk

        return operation(port, string);
    }

    virtual QByteArray operation(ComPort*, QString) = 0;

};

#endif // OPERATION

