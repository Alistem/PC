#ifndef OPERATION
#define OPERATION
#include <QtCore>
#include "comport.h"

class Operation
{
public:
    Operation() {}
    virtual ~Operation () {}

    virtual QByteArray sendCommandToPort(ComPort,QString) = 0;
};

#endif // OPERATION

