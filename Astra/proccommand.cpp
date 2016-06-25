#include <QDebug>
#include <memory>

#include "proccommand.h"
#include "getstatus.h"
#include "reset.h"
#include "readflash.h"
#include "writeflash.h"

using namespace std;


ProcCommand::ProcCommand(QObject *parent) : QObject(parent), com_port(NULL)
{

}

void ProcCommand::slot_connect(int num)
{
    if (!com_port)
        com_port = new ComPort(QString("%1%2").arg("COM").arg(num));
}

void ProcCommand::slot_disconnect()
{
    if (com_port)
        delete com_port;
    com_port = NULL;
}

void ProcCommand::slot_status()
{
    unique_ptr<Operation> get_status(new GetStatus());

    get_status->sendCommandToPort(com_port, "");
}

void ProcCommand::slot_reset()
{
    unique_ptr<Operation> reset(new Reset());

    reset->sendCommandToPort(com_port, "");
}

void ProcCommand::slot_read()
{
    unique_ptr<Operation> read_flash(new ReadFlash());

    QByteArray qbarr = read_flash->sendCommandToPort(com_port, "");
    QString qstr = qbarr;
    qDebug() << qstr;
}

void ProcCommand::slot_write(QList<QString> animation)
{
    unique_ptr<Operation> write_flash(new WriteFlash());

    write_flash->sendCommandToPort(com_port, "");
}
