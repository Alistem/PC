#ifndef WRITEFLASH_H
#define WRITEFLASH_H
#include "operation.h"


class WriteFlash : public Operation
{
public:
    WriteFlash();

    QByteArray operation(ComPort*, QString);
};

#endif // WRITEFLASH_H
