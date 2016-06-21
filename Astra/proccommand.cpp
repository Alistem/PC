#include "proccommand.h"
#include "getstatus.h"
#include "reset.h"
#include "readflash.h"
#include "writeflash.h"
#include "mainwindow.h"


ProcCommand::ProcCommand(QObject *parent) : QObject(parent)
{

}

void ProcCommand::slot_status()
{
    operation = new GetStatus();
    operation->sendCommandToPort(com_port, "");
}

void ProcCommand::slot_connect(int num)
{
    com_port = new ComPort(num);
}

void ProcCommand::slot_disconnect()
{
    delete com_port;
}

void ProcCommand::slot_reset()
{
    operation = new Reset();
    operation->sendCommandToPort(com_port, "");
}
void ProcCommand::slot_read()
{
    operation = new ReadFlash();
    operation->sendCommandToPort(com_port, "");
}
void ProcCommand::slot_write(QList<FrameInfo> animation)
{
    operation = new WriteFlash();
    operation->sendCommandToPort(com_port, "");
}
