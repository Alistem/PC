#include <QDebug>
#include <memory>

#include "proccommand.h"
#include "getstatus.h"
#include "reset.h"
#include "readflash.h"
#include "writeflash.h"
#include "frameinfo.h"

using namespace std;


ProcCommand::ProcCommand(QObject *parent) : QObject(parent), com_port(NULL)
{
    flag_command = 0;
    read_stage = 0;
    write_stage = 0;
    i_write = 0;
    errors = 0;
    nums_all_frames_flag = false;
    data_all_frames_flag = false;
    connect(&connect_tm, SIGNAL(timeout()),SLOT(slot_disconnect()));
}

void ProcCommand::slot_connect(QString num)
{
    if (!com_port){
        com_port = new ComPort(num);
        connect(com_port,SIGNAL(finish_read()),SLOT(listen_on_off()));
        connect(com_port,SIGNAL(PortError(QByteArray)),SLOT(comPortError(QByteArray)));
        if(com_port->portOpen()){
            slot_status();
        }
        else{
            emit connect_label("Нет связи");
            slot_disconnect();
        }
    }
}

void ProcCommand::slot_disconnect()
{
    if (com_port)
        delete com_port;
    com_port = NULL;
    emit connect_label("Нет связи");
}

void ProcCommand::slot_status()
{
    flag_command = 1;

    emit status(false);

    connect_tm.start(20);

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
        analise_reading_data();
        slot_reset();
        break;
    default:
        break;
    }
}

void ProcCommand::command_read(QString command)
{
    sector = command;

    unique_ptr<Operation> read_flash(new ReadFlash());

    read_flash->sendCommandToPort(com_port, command);
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
            command_write(sector);
            sector.clear();
            break;
        default:
            break;
        }
    }

    if(TempReadData.endsWith("OKOB")){
        emit status(true);
        emit connect_label("На связи");
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
    else if(TempReadData.left(4) == ("OkCR")){
//    qDebug()<<TempReadData;
    }
    else if(TempReadData.contains("HELLO")){
        //emit error_label(TempReadData);
    }
    else if(TempReadData.left(4)==("OkWR")){
        slot_write();
    }
    else if(TempReadData.left(4)==("WWOK")){
        qDebug()<<TempReadData;
        slot_write();
    }

}

bool ProcCommand::ctrl_sum_verify(QByteArray dat) // верификация контрольной суммы пакета
{
    QByteArray ctrl_sum;
    QByteArray ctrl_sum_from_package=dat.right(1);// складываем контрольную сумму в буфер для сравнения
    dat.chop(1); // отрезаем контрольную сумму и получаем чистые данные
    ctrl_sum = ctrl_sum_xor(dat); // вычисляем контрольную сумму пакета данных
    //qDebug()<<ctrl_sum.toHex()<<"ctrl_sum"<<ctrl_sum_from_package.toHex()<<"ctrl_sum_from_package";
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
        current_sector = 1; //в начале обнуляем счётчик уже считанных кадров
        command_read("1"); // читаем нулевой сектор
    }
    else{
        current_sector+=1; // счётчик уже считанных кадров

        int reading_frames = 0;

        if(num_frames>current_sector*12-24){

            if(ctrl_sum_verify(TempReadData.left(TempReadData.size()-4))){

                all_data_from_plc+=TempReadData.left(TempReadData.size()-5); // убираем контрольную сумму

                emit data_from_com(TempReadData.left(TempReadData.size()-5));

                for(int i=0;i<12;++i){ // Этот цикл вообще чисто для красоты
                    reading_frames=i+(current_sector-2)*12+1; // Для типа плавного подсёта кадров (потому что читает по секторам, а там по 12 кадров)
                    emit num_frame_read(reading_frames);
                }
            }
            else{
                comPortError("control sum is incorrect");
                errors+=1;
                if(errors<10){
                    current_sector-=1;
                    //data_all_frames_flag = false;
                    slot_read();
                    return;
                }
                else{
                    data_all_frames_flag = false;
                    read_stage = 0; // Этап выполнения алгоритма чтения данных из контроллера
                    errors = 0;
                    return;
                }
            }

            QString set_num;
            command_read(set_num.setNum(current_sector)); // читаем n - ый сектор
        }
        else{
            emit error_label("Анимация считана успешно!");
            data_all_frames_flag = false;
            read_stage = 2; // Этап выполнения алгоритма чтения данных из контроллера
            slot_read();
            return;
        }
    }
}

void ProcCommand::analise_reading_data() // вычисление контрольной суммы
{
    int n;
    for(n=0;n<num_frames;++n){ // С помощью цикла выделяем из массива данные отдельно ВРЕМЕНИ и ШИМ

        //===========================times============================================
        int i=all_data_from_plc.mid(n*34,2).toHex().toInt(0,16);
        times_of_frames+=i; // складываем время всех кадров в один массив
        //===========================SHIM=============================================
        shim_of_frames+=all_data_from_plc.mid(n*34+2,32); // складываем все кадры в массив
        //============================================================================
    }

    read_stage = 0; // Этап выполнения алгоритма чтения данных из контроллера
    //===================analise_readed_data========================================
}

void ProcCommand::data_to_project() // Передача данных анимации из контроллера в программу
{
    QByteArray buff_shim;
    int time_int;

    for(int n=0;n<num_frames;++n){ // отправляем данные покадрово (хотя, конечно надо бы сделать отправку сразу всего, а на том конце разбирать)
        //================================Times=================================
        time_int=times_of_frames.at(n); // Берём время одного кадра
        emit times_from_plc1(time_int,n,num_frames);
        //================================SHIM=================================
        buff_shim=shim_of_frames.mid(n*32,32);
        emit shim_from_plc1(buff_shim,n);
        buff_shim.clear();
    }
    shim_of_frames.clear();
    times_of_frames.clear();
}


//===================================Write Data to com-port==============================

void ProcCommand::data_from_project(QList<FrameInfo> animations){
    animation = animations;
    slot_write();
    write_stage = 0;
}

void ProcCommand::slot_write()
{
    flag_command = 4;

    switch (write_stage){
    case 0:
        qDebug()<<write_stage<<"write_stage";

        if(TempReadData.left(4)==("OkWR")){
            qDebug()<<"TempReadData.left(4)==(OkWR)";
            data_to_zero_sector(animation); // вычисление количества кадров анимации в контроллере  
        }
        else{
            command_write("0");
           qDebug()<<"command_write(0)";
        }

        break;
    case 1:
        qDebug()<<write_stage<<"write_stage";

        if(TempReadData.left(4)==("OkWR"))
            data_to_other_sector(animation); // вычисление количества кадров анимации в контроллере
        else{
            QString buffer(QString::number(i_write));
            command_write(buffer);
        }

        break;
    case 2:
        qDebug()<<write_stage<<"write_stage";
        write_stage = 0;
        emit error_label("Анимация загружена успешно!");
        slot_reset();
        break;
    default:
        break;
    }
}

void ProcCommand::command_write(QString command)
{
    sector = command;

    unique_ptr<Operation> write_flash(new WriteFlash());

    write_flash->sendCommandToPort(com_port, command);
}

void ProcCommand::comPortError(QByteArray com_port_error)
{
    qDebug()<<com_port_error;
}

void ProcCommand::data_to_zero_sector(QList<FrameInfo> animation)
{
    QByteArray ba,ba1,data;
    QString templ("00000000"),str_i;
    int ii;

    FrameInfo sum_num = animation.at(i_write);
    qDebug()<<i_write;
    int sectors = sum_num.fsum_num/12+1;
    qDebug()<<sectors<<"sectors";
    str_i.setNum(sectors*512,16);
    for(ii=0;ii<str_i.size();++ii)
        templ.replace(templ.size()-1-ii,1,str_i.at(str_i.size()-1-ii));
    str_i=templ;
    ba += str_i;
    ba=ba1.fromHex(ba);
    data.resize(404);
    data.fill(0);
    ba += data;
    ba += ctrl_sum_xor(ba);
    write_stage = 1;
    i_write = 1;
    command_write(ba.toHex());
}

void ProcCommand::data_to_other_sector(QList<FrameInfo> animation)
{
    QByteArray packet;
        for(int k=0;k<12;k+=1){

            if(animation.size() < (i_write-1)*12+k+1)
                break;

            QByteArray ba,ba1;
            QString str("0000"),str1;

            FrameInfo current_data = animation.at((i_write-1)*12+k);
            str1.setNum(current_data.ftime,16);
            for(int i=str1.size()-1;i>=0;--i){
                str.replace(str.size()-i-1,1,str1.at(i));
            }
            ba+=str;
            ba=ba1.fromHex(ba);
            ba+=current_data.flist;
            packet+=ba;
        }
        if(packet.size() < 408){
            int h=408-packet.size();
            for(int i=0;i<h;++i)
                packet.append(1);
        }
        packet += ctrl_sum_xor(packet);
        qDebug()<<packet.toHex();

        if(i_write == animation.size()/12+1){
            write_stage = 2;
        }
        i_write += 1;
        command_write(packet.toHex());
}
