#include <QDebug>

#include "proccommand.h"
#include "getstatus.h"
#include "reset.h"
#include "readflash.h"
#include "writeflash.h"


ProcCommand::ProcCommand(QObject *parent) : QObject(parent), com_port(NULL)
{
    com_port = NULL;
}

void ProcCommand::slot_connect(int num)
{
    if(!com_port)
        com_port = new ComPort(QString("%1%2").arg("COM").arg(num));
}

void ProcCommand::slot_disconnect()
{
    if(com_port)
        delete com_port;
    com_port = NULL;
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
    QByteArray qbarr = read_flash->sendCommandToPort(com_port, "");
    QString qstr = qbarr;
    qDebug() << qstr;
}

void ProcCommand::slot_write(QList<QString> animation)
{
    Operation *write_flash = new WriteFlash();
    write_flash->sendCommandToPort(com_port, "");
}
