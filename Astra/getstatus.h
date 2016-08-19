#ifndef GETSTATUS_H
#define GETSTATUS_H

#include "operation.h"

class GetStatus : public Operation
{
public:
    GetStatus();

    QByteArray operation(IAdapterInterface *, QString);
};

#endif // GETSTATUS_H
