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
    read_stage = 0;
    errors = 0;
    nums_all_frames_flag = false;
    data_all_frames_flag = false;
}

void ProcCommand::slot_connect(int num)
{
    if (!com_port){
        com_port = new ComPort(QString("%1%2").arg("COM").arg(num));
        connect(com_port,SIGNAL(finish_read()),SLOT(listen_on_off()));
        connect(com_port,SIGNAL(PortError(QByteArray)),SLOT(comPortError(QByteArray)));
        if(com_port->portOpen()){
            emit connection("Connected");
            emit connect_label("onLine");
            slot_status();
        }
        else{
            emit connection("Disconnected");
            emit connect_label("offLine");
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
    emit connect_label("offLine");
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
    switch (read_stage) {
    case 0:
        qDebug()<<read_stage<<"read_stage";
        nums_all_frames(); // вычисление количества кадров анимации в контроллере
        break;
    case 1:
        qDebug()<<read_stage<<"read_stage";
        emit frames_label(num_frames); // отправляем на форму
        data_all_frames(); // чтение данных анимации c контроллерa
        break;
    case 2:
        qDebug()<<read_stage<<"read_stage";
        break;

    default:
        break;
    }
}

void ProcCommand::command_read(QString command)
{
    flag_command = 3;

    sector = command;

    unique_ptr<Operation> read_flash(new ReadFlash());

    read_flash->sendCommandToPort(com_port, command);
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
            return;
        }
        if(data_all_frames_flag){
            data_all_frames();
            return;
        }
    }
    else if(TempReadData.contains("ErCM") || TempReadData.contains("ErCR")){
        qDebug()<<"control_sum_error"<<TempReadData.right(4);
        if(errors<10){
            if(nums_all_frames_flag){
                //nums_all_frames();
            }
            if(data_all_frames_flag){
                //data_all_frames();
            }
        }
        else{
            nums_all_frames_flag = false;
            data_all_frames_flag = false;
            read_stage = 0;
            errors = 0;
        }
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

void ProcCommand::nums_all_frames() // вычисление количества кадров анимации в контроллере
{
    int num_sectors = 0;
    num_frames = 0;
    if(!nums_all_frames_flag){
        nums_all_frames_flag = true;
        command_read("0"); // читаем нулевой сектор
    }
    else{
        nums_all_frames_flag = false;
        num_sectors=TempReadData.mid(0,4).toHex().toInt(0,16);
        num_frames=(num_sectors-512)*12/512; // вычисляем количество кадров

        read_stage = 1; // Этап выполнения алгоритма чтения данных из контроллера

        slot_read();
    }
}

void ProcCommand::data_all_frames() // чтение данных анимации c контроллерa
{
    if(!data_all_frames_flag){
        data_all_frames_flag = true;
        current_frame = 0; //в начале обнуляем счётчик уже считанных кадров
        command_read("0"); // читаем нулевой сектор
    }
    else{
        current_frame+=1; // счётчик уже считанных кадров
        int reading_frames = 0;
        if(num_frames>current_frame){
            if(ctrl_sum_verify(TempReadData.left(TempReadData.size()-4))){

                all_data_from_plc+=TempReadData.left(TempReadData.size()-5); // убираем контрольную сумму

                qDebug()<<TempReadData.toHex();

                for(int i=0;i<12;++i){ // Этот цикл вообще чисто для красоты
                    reading_frames+=1; // Для типа плавного подсёта кадров (потому что читает по секторам, а там по 12 кадров)
                emit num_frame_read(reading_frames);
                }

                QString set_num;
                command_read(set_num.setNum(current_frame)); // читаем n - ый сектор

            }
            else{
                comPortError("control sum is incorrect");
                errors+=1;
                data_all_frames_flag = false;
                read_stage = 0;
                slot_read();
                return;
            }
        }
        else{
            data_all_frames_flag = false;
            read_stage = 0;
            read_stage = 2; // Этап выполнения алгоритма чтения данных из контроллера
            slot_read();
            return;
        }
    }
}
