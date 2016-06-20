#ifndef SLISTEN_H
#define SLISTEN_H
#include "state.h"


class SListen : public State
{
public:
    SListen();
    static int state;
    void reset();
};

int SListen::state = ST_LISTEN;

enum SListenCommand {SLC_STATUS, SLC_READ, SLC_WRITE, SLC_RESET};

#endif // SLISTEN_H
