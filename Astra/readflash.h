#ifndef READFLASH_H
#define READFLASH_H
#include "operation.h"

class ReadFlash : public Operation
{
public:
    ReadFlash();
    QByteArray ctrl_sum_xor(QByteArray);

    QByteArray operation(IAdapterInterface*, QString);
};

#endif // READFLASH_H
