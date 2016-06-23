#ifndef READFLASH_H
#define READFLASH_H
#include "operation.h"

class ReadFlash : public Operation
{
public:
    ReadFlash();

    QByteArray operation(ComPort*, QString);
};

#endif // READFLASH_H
