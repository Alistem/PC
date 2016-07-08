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
    flag_command = 0;
}

void ProcCommand::slot_connect(int num)
{
    if (!com_port){
        com_port = new ComPort(QString("%1%2").arg("COM").arg(num));
        connect(com_port,SIGNAL(finish_read()),SLOT(listen_on_off()));
        //connect(com_port,SIGNAL(finish_read()),SLOT(listen_on_off()));
        if(com_port->portOpen()){
            emit connection("Connected");
            slot_status();
        }
        else{
            emit connection("Disconnected");
            slot_disconnect();
        }
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
    flag_command = 1;

    emit status(false);

    unique_ptr<Operation> get_status(new GetStatus());

    get_status->sendCommandToPort(com_port, "");

}

void ProcCommand::slot_reset()
{
    flag_command = 2;

    unique_ptr<Operation> reset(new Reset());

    reset->sendCommandToPort(com_port, "");


}

void ProcCommand::slot_read()
{
    flag_command = 3;

    unique_ptr<Operation> read_flash(new ReadFlash());

    BufferReadData = read_flash->sendCommandToPort(com_port, "");
    //QString qstr = BufferReadData;
    //qDebug() << qstr;

}

void ProcCommand::slot_write(QList<QString> animation)
{
    flag_command = 4;

    unique_ptr<Operation> write_flash(new WriteFlash());

    write_flash->sendCommandToPort(com_port, "");
}

void ProcCommand::comPortError()
{
    qDebug()<<"comPortError";
}

void ProcCommand::listen_on_off()
{
    TempReadData=com_port->read();
    if(TempReadData.endsWith("OkOk")){
        switch (flag_command) {
        case 1:
            slot_status();
            break;
        case 2:
            slot_reset();
            break;
        case 3:
            slot_read();
            break;
        case 4:
            //slot_write(QList<QString> animation);
            break;
        default:
            break;
        }
    }

    if(TempReadData.endsWith("OKOB")){
        emit status(true);
    }
    else if (TempReadData.endsWith("RROK")){
        //qDebug()<<TempReadData.toHex();
    }
    else if(TempReadData.contains("ErCM") || TempReadData.contains("ErCR")){
        qDebug()<<"data_plc_read-error"<<TempReadData.right(4);
    }
}
