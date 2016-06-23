#ifndef RESET_H
#define RESET_H
#include "operation.h"

class Reset : public Operation
{
public:
    Reset();

    QByteArray operation(ComPort*, QString);
};

#endif // RESET_H
