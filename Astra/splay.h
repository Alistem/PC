#ifndef SPLAY_H
#define SPLAY_H
#include "state.h"

class SPlay : public State
{
public:
    SPlay();
    static int state;
};

int SPlay::state = ST_PLAY;
#endif // SPLAY_H
