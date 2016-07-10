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
        connect(com_port,SIGNAL(PortError(QByteArray)),SLOT(comPortError(QByteArray)));
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

void ProcCommand::slot_read() // вычисление количества кадров анимации в контроллере
{
    int nums = nums_all_frames();
    emit frames_label(nums); // отправляем на форму

}

void ProcCommand::command_read(QString command)
{
    flag_command = 3;

    sector = command;

    unique_ptr<Operation> read_flash(new ReadFlash());

    BufferReadData = read_flash->sendCommandToPort(com_port, command);
}

void ProcCommand::slot_write(QList<QString> animation)
{
    flag_command = 4;

    unique_ptr<Operation> write_flash(new WriteFlash());

    write_flash->sendCommandToPort(com_port, "");
}

void ProcCommand::comPortError(QByteArray com_port_error)
{
    qDebug()<<com_port_error;
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
            command_read(sector);
            sector.clear();
            qDebug()<<"slot_read";
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
        if(nums_all_frames_flag){
            nums_all_frames();
        }
        //qDebug()<<TempReadData.toHex();
    }
    else if(TempReadData.contains("ErCM") || TempReadData.contains("ErCR")){
        qDebug()<<"control_sum_error"<<TempReadData.right(4);
    }
    else if(TempReadData.left(4) == ("OkCR"))
            qDebug()<<"control_sum_ok";
}

bool ProcCommand::ctrl_sum_verify(QByteArray dat) // верификация контрольной суммы пакета
{
    QByteArray ctrl_sum;
    QByteArray ctrl_sum_from_package=dat.right(1);// складываем контрольную сумму в буфер для сравнения
    dat.chop(1); // отрезаем контрольную сумму и получаем чистые данные
    ctrl_sum = ctrl_sum_xor(dat); // вычисляем контрольную сумму пакета данных
    if(ctrl_sum_from_package==ctrl_sum){
        emit error_label("Ctrl sum is valid");
        ctrl_sum_errors=0;
        return true;
    }
    else{
        emit error_label("Ctrl sum is not valid");
        ctrl_sum_errors+=1;
        return false;
    }
}
QByteArray ProcCommand::ctrl_sum_xor(QByteArray dat) // вычисление контрольной суммы
{
    int i;
    QByteArray ctrl_sum;
    char LRC=dat.at(0);
    for(i=1;i<dat.size();++i)
    LRC=(LRC^dat.at(i));
    ctrl_sum.resize(1);
    ctrl_sum[0]=LRC;
    return ctrl_sum;
}

int ProcCommand::nums_all_frames() // вычисление количества кадров анимации в контроллере
{
    if(!nums_all_frames_flag){
        nums_all_frames_flag = true;
        command_read("00000000");
    }
    else{
        nums_all_frames_flag = false;
        int num_sectors=TempReadData.mid(4,4).toHex().toInt(0,16);
        int num_frames=(num_sectors-512)*12/512; // вычисляем количество кадров
        return num_frames;
    }
}
