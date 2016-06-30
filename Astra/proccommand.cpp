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
    if (!com_port){
        com_port = new ComPort(QString("%1%2").arg("COM").arg(num));

        //connect(com_port,SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(readError(QSerialPort::SerialPortError)));
        //emit connection("Connected");
    }
}

void ProcCommand::slot_disconnect()
{
    if (com_port)
        delete com_port;
    com_port = NULL;
    emit connection("Disconnected");
}

void ProcCommand::slot_status()
{
    unique_ptr<Operation> get_status(new GetStatus());

    get_status->sendCommandToPort(com_port, "");

    slot_read();

    if(BufferReadData.contains("OKOB"))
        emit status(true);
    else
        emit status(false);
}

void ProcCommand::slot_reset()
{
    unique_ptr<Operation> reset(new Reset());

    reset->sendCommandToPort(com_port, "");
}

void ProcCommand::slot_read()
{
    unique_ptr<Operation> read_flash(new ReadFlash());

    BufferReadData = read_flash->sendCommandToPort(com_port, "");
    QString qstr = BufferReadData;
    qDebug() << qstr;
}

void ProcCommand::slot_write(QList<QString> animation)
{
    unique_ptr<Operation> write_flash(new WriteFlash());

    write_flash->sendCommandToPort(com_port, "");
}

void ProcCommand::readError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        QString errors;/*=QObject::tr("An I/O error occurred while reading the data from port %1, error: %2").arg(com1->portName()).arg(com1->errorString());*/
        emit error_label(errors);
        }
}
