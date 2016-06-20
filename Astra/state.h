#ifndef STATE_H
#define STATE_H


class State
{
public:
    State() {}
    virtual void command(int)=0;
    virtual void reset() {}
};

enum StateType {ST_RW, ST_PLAY, ST_LISTEN};

#endif // STATE_H
