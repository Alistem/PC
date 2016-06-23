#include "proccommand.h"
#include "getstatus.h"
#include "reset.h"
#include "readflash.h"
#include "writeflash.h"
#include "mainwindow.h"


ProcCommand::ProcCommand(QObject *parent) : QObject(parent)
{

}

void ProcCommand::slot_connect(int num)
{
    com_port = new ComPort(num);
}

void ProcCommand::slot_disconnect()
{
    delete com_port;
}

void ProcCommand::slot_status()
{
    Operation *get_status = new GetStatus();
    get_status->sendCommandToPort(com_port, "");
}

void ProcCommand::slot_reset()
{
    Operation *reset = new Reset();
    reset->sendCommandToPort(com_port, "");
}

void ProcCommand::slot_read()
{
    Operation *read_flash = new ReadFlash();
    read_flash->sendCommandToPort(com_port, "");
}

void ProcCommand::slot_write(QList<FrameInfo> animation)
{
    Operation *write_flash = new WriteFlash();
    write_flash->sendCommandToPort(com_port, "");
}
