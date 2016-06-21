#ifndef READFLASH_H
#define READFLASH_H
#include "operation.h"

class ReadFlash : public Operation
{
public:
    ReadFlash();

    QByteArray sendCommandToPort(ComPort, QString);
};

#endif // READFLASH_H
