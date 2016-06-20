#ifndef SRW_H
#define SRW_H
#include "state.h"

class SRW : public State
{
public:
    SRW();
    static int state;
    void reset();
};

int SRW::state = ST_RW;

#endif // SRW_H
