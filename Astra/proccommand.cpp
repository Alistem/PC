#include "proccommand.h"
#include "slisten.h"


ProcCommand::ProcCommand(QObject *parent) : QObject(parent)
{

}

void ProcCommand::slot_status()
{
    state->reset();
    state->command();
    state->command(SLC_STATUS);
}
