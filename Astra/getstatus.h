#ifndef GETSTATUS_H
#define GETSTATUS_H
#include "operation.h"

class GetStatus : public Operation
{
public:
    GetStatus();
    QByteArray sendCommandToPort(ComPort, QString);
};

#endif // GETSTATUS_H
