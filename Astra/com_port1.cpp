#include "com_port1.h"
#include <iostream>
#include <QtCore/QDebug>
#include <QApplication>
#include <QCoreApplication>


QT_USE_NAMESPACE

bool stop = false;

com_port::com_port(QWidget *parent) :
    QWidget(parent)
{
    connect(this, SIGNAL(connect_com()), this, SLOT(COMConnector()));
    connect(this,SIGNAL(disconnect_com()),this,SLOT(COMClose()));
    connect(&m_timer, SIGNAL(timeout()), SLOT(readTimeout()));
    connect(&r_timer, SIGNAL(timeout()), SLOT(Read_finish()));
    connect(&stat_timer_2, SIGNAL(timeout()), SLOT(status_new_two()));
    connect(&reset_timer, SIGNAL(timeout()), SLOT(reset_button_3()));

//=========================Init================================================
init();
}
void com_port::init()
{
    connect_close=1;
    res_stat_plc=0; // запрос статуса плк
    status_butt=0; // запрос статуса плк с кнопки
    res_data_from_plc=0;
    status_but_flag=0;
    read_but_flag=0;
    block_press_status=0;  // блокировка от двойного нажатия
    block_press_read=0; // блокировка от двойного нажатия
    block_press_write=0;
    block_press_reset=0; // блокировка от двойного нажатия
    reset_butt=0;
    res_stat_plc=0;
    i_stat=0;
    i_frames=0;
    reading_frames=0;
    ready=0;
}
//============================================================================
void com_port::COMClose()
{
    if(stop==true){ // защита
    stop = false;
    disconnect(com1,SIGNAL(readyRead()),this,SLOT(ReadCOMPort()));
    disconnect(com1,SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(readError(QSerialPort::SerialPortError)));
    com1->close();
    connect_close=1;
    init();
    emit connect_label("Disconnected");
    }
}

void com_port::COMConnector(){
    emit com_port_num();
    QString port_num=QString("%1%2").arg("COM").arg(num_com_port);
    com1 = new QSerialPort(port_num); //для Win
    connect(com1,SIGNAL(readyRead()),this,SLOT(ReadCOMPort()));
    connect(com1,SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(readError(QSerialPort::SerialPortError)));
    if(com1->open(QIODevice::ReadWrite) == true){
        stop = true;
        emit connect_label("Connected");
        emit error_label("");
        connect_close=0;
    }else
        emit error_label(com1->errorString());
    com1->setBaudRate(57600);
    com1->setFlowControl(QSerialPort::NoFlowControl);
    com1->setParity(QSerialPort::NoParity);
    com1->setDataBits(QSerialPort::Data8);
    com1->setStopBits(QSerialPort::OneStop);
}

void com_port::WriteToCOMPort(){
    if(stop==true){

        QByteArray ba,ba1;
        ba+=post_data; //==========!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!=============
        qint64 num=com1->write(ba1.fromHex(ba)); // тут мы пишем данные в ком-порт и !!! после записи сравниваем количество байт записанных с отправленными. Если всё ок - рапортуем в письменном виде об успешной передаче данных
        if (num == ba.size()) {
            QString errors=QObject::tr("Data successfully sent to port %1").arg(com1->portName());
            emit error_label(errors);
        }   
        post_data.clear();
    }
    //    m_timer.start(5000);
}



//=================================== Чтение из com-port ================================
void com_port::ReadCOMPort(){
    if(stop==true){ // блокировка от чтения вне соединения с ком-портом
//    quint64 dat=com1->bytesAvailable();
    m_readData.append(com1->readAll());
//    if (!m_timer.isActive())
    m_timer.start(10);
    r_timer.start(10); // ждём, чтобы весь пакет записался в буффер. Очень важный таймаут. Если косяки с чтением данных из контроллера, крутить надо именно его
    readData+=m_readData;
    m_readData.clear();
    }
}

void com_port::data_com_port_post(QString post)
{
    post_data=post;
    WriteToCOMPort();
}

void com_port::num_com_port_com(int num)
{
    num_com_port=num;
}

void com_port::readTimeout()
{
    m_timer.stop();
    if (m_readData.isEmpty()) {
       QString errors=QObject::tr("No data was currently available for reading from port %1").arg(com1->portName());
       emit error_label(errors);
    } else {
        QString errors=QObject::tr("Data successfully received from port %1").arg(com1->portName());
        emit error_label(errors);
    }
}

void com_port::readError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        QString errors=QObject::tr("An I/O error occurred while reading the data from port %1, error: %2").arg(com1->portName()).arg(com1->errorString());
        emit error_label(errors);
        }
}

void com_port::Read_finish()
{
    if(connect_close==1) // если отсоединил вручнную от com_port-а, то прекращаем все действия
        return;
    r_timer.stop();
    read_end=1; // флаг для обозначения, что чтение закончилось
    if(reset_butt==1)
        reset_button_3();
    if(res_stat_plc==1) // условие, что функция запроса статуса вкл
        status_new_two();
    if(status_butt==1) // условие, что функция запроса статуса вкл
        status_plc_transmit();
    if(command_and_data_sector==1)// условие, что функция чтения данных запущена
        end_read_data_anim=1; // читаем следующий сектор данных с контроллера
    if(res_data_from_plc==1)// условие, что функция чтения данных запущена
        data_plc_read();  
    if(data_to_plc==1)// условие, что функция чтения данных запущена
        data_plc_write();
}

//===============================================================================================
void com_port::reset_button()//сброс контроллера
{
    if(block_press_reset!=1){// блокировка от двойного нажатия на кнопку
        block_press_reset=1;// блокировка от двойного нажатия на кнопку
        reset_button_2();
    }
    else
        return;
}

void com_port::reset_button_2() // сброс контроллера
{
    if(connect_close==1) // если отсоединил вручнную от com_port-а, то прекращаем все действия
        return;
    reset=1;
    if(ready!=1){
        reset_but_flag=1; // флаг о том, что нажата кнопка reset
        readData.clear();
        ready=0; // Флаг ставится в 1 когда от контроллера получен сигнал "ОкОк"
        //=====================сбрасываем все флаги===================================

        res_stat_plc=0; // запрос статуса плк
        status_butt=0; // запрос статуса плк с кнопки
        res_data_from_plc=0;
        status_but_flag=0;
        read_but_flag=0;
        write_but_flag=0;
        block_press_status=0;
        block_press_read=0;
        block_press_write=0;

        //============================================================================
        post_data.clear();
        post_data="63"; // команда для переключения контроллера в режим прослушивания
        reset_timer.start(100); // таймер нужен для ожидания ответа. Если за время ответа не поступило, то делаем ещё один заход
        read_end=0;
        WriteToCOMPort(); // пишем данные в ком-порт
    }
    else{
        post_data="63ff00000000039f";     
        readData.clear(); // чистим буфер    
        read_end=0;
        reset_butt=1;       
        reset_timer.start(50); // таймер нужен для ожидания ответа. Если за время ответа не поступило, то делаем ещё один заход
        WriteToCOMPort(); // пишем данные в ком-порт

    }
}

void com_port::reset_button_3() // сброс контроллера
{
    //==========================закрытие соединения====================================================

    if(connect_close==1) // если отсоединил вручнную от com_port-а, то прекращаем все действия
        return;
    //=================================================================================================
    if(readData.isEmpty()) //иногда приходит пустой массив, перед массивом с данными(похоже, приходит перевод строки)
        return;
    bool st;
    i_stat+=1; // счётчик попыток установить положительный статус ПЛК
    if(readData.endsWith("ErCM") || readData.endsWith("ErCR")){
        qDebug()<<"may be ErCR"<<readData.right(4);
        ready=0;// Флаг ставится в 1 когда от контроллера получен сигнал "ОкОк"
        reset=0;
        reset_but_flag=0;
        reset_butt=0;
        emit error_label(readData.right(4));
        reset_timer.stop();
        reset_button_2();
        return;
    }
    if(readData.endsWith("OkOk")){ // Если ответ OkOk, то контроллер готов к приёму данных
        ready=1; // готовность к приёму команды // Флаг ставится в 1 когда от контроллера получен сигнал "ОкОк"
        reset_timer.stop();
//        qDebug()<<"OkOk";
        reset_button_2();
        return;
    }
    if(readData.contains("HELLO!")){ // Если ответ OKOB, то статус успешно установлен
//        qDebug()<<readData;
        ready=0; // готовность к приёму команды // Флаг ставится в 1 когда от контроллера получен сигнал "ОкОк"
        reset=0;
        i_stat=0; // счётчик запросов тоже обнуляем
        emit error_label(readData);
        readData.clear(); // чистим буфер
        post_data.clear();
        block_press_reset=0;// блокировка от двойного нажатия на кнопку
        reset_but_flag=0;
        reset_butt=0;
        reset_timer.stop();
        return;
    }
    else{
        if(i_stat<10){ // Если количество попыток запроса меньше 10
            emit status(st=false);//отправка флага о статусе в мэйнвиндоу
            reset_timer.stop();
            reset_button_2(); // Заново пытаемся получить статус
        }
        else{
            reset_butt=0; //если число запросов превысило лимит, то прекращаем попытки
            i_stat=0; // счётчик запросов тоже обнуляем
            ready=0; // Флаг ставится в 1 когда от контроллера получен сигнал "ОкОк"
            reset=0;
            post_data.clear();
            block_press_reset=0;
            reset_but_flag=0;
            reset_timer.stop();
            qDebug()<<"reset are >10!";
            emit status(st=false);//отправка флага о статусе в мэйнвиндоу
            emit error_label("No communication with the controller, most likely");
            return;
        }
    }
}

void com_port::status_new_one()//подготовка контроллера к чтению команды
{
//    qDebug()<<"status_new_one";
    //==========================закрытие соединения====================================================

    if(connect_close==1) // если отсоединил вручнную от com_port-а, то прекращаем все действия
        return;
    //=================================================================================================
    if(reset==1){
        reset_button_2();
        return;
    }
    if(res_stat_plc==0){
        res_stat_plc=1; // ставим запрос статуса в 1, для контроля
        readData.clear();
        ready=0;// Флаг ставится в 1 когда от контроллера получен сигнал "ОкОк"
    }
    post_data.clear();
    post_data="63"; // команда для переключения контроллера в режим прослушивания
    stat_timer_2.start(50); // таймер нужен для ожидания ответа. Если за время ответа не поступило, то делаем ещё один заход
    WriteToCOMPort(); // пишем данные в ком-порт
    read_end=0;

}
void com_port::status_new_two()//подготовка контроллера к чтению команды
{
    //==========================закрытие соединения====================================================

    if(connect_close==1) // если отсоединил вручнную от com_port-а, то прекращаем все действия
        return;
    //=================================================================================================
    i_stat+=1; // счётчик попыток установить положительный статус ПЛК
    bool st;
        if(readData.endsWith("OkOk")){ // Если ответ OkOk, то контроллер готов к приёму данных
//            qDebug()<<"status_new_two"<<"OkOk";
            ready=1; // готовность к приёму команды // Флаг ставится в 1 когда от контроллера получен сигнал "ОкОк"
            res_stat_plc=0;
            stat_timer_2.stop();
            i_stat=0;
            readData.clear(); // чистим буфер
            post_data.clear();
            if(reset_but_flag==1)
                reset_button_2();
            if(status_but_flag==1)
                status_button_2();
            if(read_but_flag==1)
                read_com_port();
            if(write_but_flag==1)
                write_to_com_port();
            return;
        }
        else{
            if(i_stat!=10){ // Если количество попыток запроса меньше 10
                emit status(st=false);//отправка флага о статусе в мэйнвиндоу
                if(ready!=1) // блокировка от отправки запроса при положительном ответе от контроллера
                status_new_one(); // Отправляем флаг в контроллер
            }
            else{
                res_stat_plc=0; //если число запросов превысило лимит, то прекращаем попытки
                i_stat=0; // счётчик запросов тоже обнуляем
                stat_timer_2.stop(); // останавливаем таймер запросов о статусе
                status_controller=0;
                ready=0;// Флаг ставится в 1 когда от контроллера получен сигнал "ОкОк"
                post_data.clear();
                emit status(st=false);//отправка флага о статусе в мэйнвиндоу
                emit error_label("No communication with the controller, most likely");
                return;
            }
        }
}
//==============================================================================================
void com_port::status_button() // нажатие клавиши обрабатываем отдельно
{
    if(block_press_status!=1){// блокировка от двойного нажатия на кнопку
        block_press_status=1;// блокировка от двойного нажатия на кнопку
        status_button_2();
    }
    else
        return;
}

void com_port::status_button_2() // установка статуса контроллера
{
    //==========================закрытие соединения====================================================

    if(connect_close==1) // если отсоединил вручнную от com_port-а, то прекращаем все действия
        return;
    //=================================================================================================

        if(ready!=1){ // Флаг ставится в 1 когда от контроллера получен сигнал "ОкОк"
            status_but_flag=1; // флаг о том, что нажата кнопка запроса статуса
            status_new_one();
        }
        else{
        post_data="63ff00000000009c";
        readData.clear(); // чистим буфер
        read_end=0;
        status_butt=1;
//        stat_but_timer.start(50); // таймер нужен для ожидания ответа. Если за время ответа не поступило, то делаем ещё один заход
        WriteToCOMPort(); // пишем данные в ком-порт
        }
}

void com_port::status_plc_transmit() // проверка статуса плк, подключен он или нет
{
    //==========================закрытие соединения====================================================

    if(connect_close==1) // если отсоединил вручнную от com_port-а, то прекращаем все действия
        return;
    //=================================================================================================

    if(readData.isEmpty()) //иногда приходит пустой массив, перед массивом с данными(похоже, приходит перевод строки)
        return;
    bool st;
    i_stat+=1; // счётчик попыток установить положительный статус ПЛК
    if(readData.endsWith("ErCM") || readData.endsWith("ErCR")){
        qDebug()<<"may be ErCR"<<readData.right(4);
        ready=0;// Флаг ставится в 1 когда от контроллера получен сигнал "ОкОк"
        status_but_flag=0;
        emit error_label(readData.right(4));
        stat_but_timer.stop();
        status_button_2();
        return;
    }
    if(readData.endsWith("OKOB")){ // Если ответ OKOB, то статус успешно установлен
        ready=0; // готовность к приёму команды // Флаг ставится в 1 когда от контроллера получен сигнал "ОкОк"
        emit status(st=true); //отправка флага о статусе в мэйнвиндоу
        status_butt=0; // запрос статуса обнуляем, поскольку результат получен
        i_stat=0; // счётчик запросов тоже обнуляем
        emit error_label("Controller is connected");
        readData.clear(); // чистим буфер
        post_data.clear();
        status_controller=1;
        block_press_status=0;// блокировка от двойного нажатия на кнопку
        status_but_flag=0;
        stat_but_timer.stop();
        reset_button();
        return;
    }
    else{
        if(i_stat!=10){ // Если количество попыток запроса меньше 10
            emit status(st=false);//отправка флага о статусе в мэйнвиндоу
            status_button_2(); // Заново пытаемся получить статус
            stat_but_timer.stop();
        }
        else{
            status_butt=0; //если число запросов превысило лимит, то прекращаем попытки
            i_stat=0; // счётчик запросов тоже обнуляем
//            stat_timer.stop(); // останавливаем таймер запросов о статусе
            status_controller=0;
            ready=0;// Флаг ставится в 1 когда от контроллера получен сигнал "ОкОк"
            post_data.clear();
            block_press_status=0;
            status_but_flag=0;
            stat_but_timer.stop();
            emit status(st=false);//отправка флага о статусе в мэйнвиндоу
            emit error_label("No communication with the controller, most likely");
            return;
        }
    }

}

//==========================================Read Data from controller=============================

void com_port::read_button() // чтение анимации из контроллера
{
    if(block_press_read!=1){
        block_press_read=1;
        read_com_port();
    }
    else
        return;
}

void com_port::read_com_port() // чтение анимации из контроллера
{
    //==========================закрытие соединения====================================================

    if(connect_close==1) // если отсоединил вручнную от com_port-а, то прекращаем все действия
        return;
    //=================================================================================================
    if(ready!=1){
        read_but_flag=1;
        if(sec_resp!=1)
        first_resp=1; // первый запрос (в нулевой кадр) нужен для того, чтобы при обращении
        // к следующим кадрам через эту функцию не было путаницы в командах
        status_new_one();
        readData.clear();
        return;
//    if(res_data_from_plc==0){
//        res_data_from_plc=1; // ставим запрос статуса в 1, для контроля
//        readData.clear();
    }
    else{ // готовность к приёму команды
        if(first_resp==1)
            post_data="63ff00000000019d";
        else
            post_data=command_for_read_frame.toHex();
        readData.clear(); // чистим буфер
        read_end=0;
        res_data_from_plc=1;
        WriteToCOMPort(); // пишем данные в ком-порт
    }
}

void com_port::data_plc_read()
{
    control_buff=readData;
    //==========================закрытие соединения====================================================

    if(connect_close==1) // если отсоединил вручнную от com_port-а, то прекращаем все действия
        return;
    //=================================================================================================
    bool st;
    i_stat+=1; // счётчик попыток установить положительный статус ПЛК

    //===================================Errors========================================================
    if(readData.contains("ErCM") || readData.contains("ErCR")){
        qDebug()<<"data_plc_read-error"<<readData.right(4);
        ready=0;
        read_but_flag=0;
        res_data_from_plc=0;
        OkCR=0;
        emit error_label(readData.right(4));
        read_com_port();
        return;
    }
    //=================================================================================================


    if(readData.endsWith("RROK")){ // Если ответ RROK, Значит приём данных успешно завершён
//        qDebug()<<readData.right(4);
        i_stat=0; // счётчик запросов обнуляем
        ready=0; // флаг готовности к приёму команды обнуляем (но именно здесь, в конце) что значит, что логика выполнена
        res_data_from_plc=0; // запрос статуса обнуляем, поскольку результат получен
        for_frame_data.clear();
        for_frame_data=readData; // копируем из одного буфера в другой
        readData.clear(); // чистим буфер
        post_data.clear();
        read_but_flag=0;
        block_press_read=0; // блокировка от двойного нажатия
        OkCR=0;

        if(for_frame_data.left(4)=="OkCR"){
            for_frame_data.remove(0,4); // режем всякие OkCR-ы
            OkCR=1;
        }
        for_frame_data.chop(4); // режем последние 4 символа (RROK) получаем чистый пакет данных + CRC
        //===================== Verify Control Sum =============================================
        ctrl_sum_verify(for_frame_data); // верификация контрольной суммы пакета
        if(verify_ctrl_sum==0){
            if(ctrl_sum_errors!=10){ // Если количество попыток запроса меньше 10
                qDebug()<<"ctrl_sum_errors";
                read_com_port(); // Заново пытаемся прочесть данные
            }else{
                res_data_from_plc=0; //если число запросов превысило лимит, то прекращаем попытки
                i_stat=0; // счётчик запросов тоже обнуляем
                ctrl_sum_errors=0;
                ready=0;
                read_but_flag=0;
                block_press_read=0; // блокировка от двойного нажатия
                post_data.clear();
                readData.clear();
                status_controller=0;
                OkCR=0;
                emit status(st=false); //отправка флага о статусе в мэйнвиндоу
                emit error_label("No communication with the controller, most likely");
                return;
            }
        }
        //=======================================================================================
        emit data_from_com(for_frame_data);
        //=======================================================================================

        if(first_resp==1){
            plc_data_edit(for_frame_data); // Обработка данных нулевого сектора
            return;
        }

        //=======================================================================================
        if(sec_resp==1){
            all_data_from_plc+=for_frame_data.left(for_frame_data.size()-1); // убираем контрольную сумму
            for(int i=0;i<12;++i) // Этот цикл вообще чисто для красоты
            reading_frames+=1; // Для типа плавного подсёта кадров (потому что читает по секторам, а там по 12 кадров)
            if(end_read_data_anim==1){
                command_and_read_data_sector(); // читаем следующий сектор данных с контроллера
                end_read_data_anim=0;
            }
            emit num_frame_read(reading_frames);
//            qDebug()<<"data_frames"<<all_data_from_plc.toHex();
            return;
        }


//        qDebug()<<for_frame_data.toHex();

    }else{
        if(i_stat<10){ // Если количество попыток запроса меньше 10
//            qDebug()<<"!=RROK"<<readData;
            read_com_port(); // Заново пытаемся получить статус
        }else{
           res_data_from_plc=0; //если число запросов превысило лимит, то прекращаем попытки
           i_stat=0; // счётчик запросов тоже обнуляем
           ctrl_sum_errors=0;
           ready=0;
           read_but_flag=0;
           block_press_read=0; // блокировка от двойного нажатия
           post_data.clear();
           readData.clear();
           status_controller=0;
           OkCR=0;
           emit status(st=false); //отправка флага о статусе в мэйнвиндоу
           emit error_label("No communication with the controller, most likely");
           return;
        }
    }
}

void com_port::ctrl_sum_verify(QByteArray dat) // верификация контрольной суммы пакета
{
    QByteArray ctrl_sum_from_package=dat.right(1);// складываем контрольную сумму в буфер для сравнения
    dat.chop(1); // отрезаем контрольную сумму и получаем чистые данные
    ctrl_sum_xor(dat); // вычисляем контрольную сумму пакета данных
    if(ctrl_sum_from_package==ctrl_sum){
        emit error_label("Ctrl sum is valid");
        verify_ctrl_sum=1;
        ctrl_sum_errors=0;
    }
    else{
        emit error_label("Ctrl sum is not valid");
        verify_ctrl_sum=0;
        ctrl_sum_errors+=1;
    }
}
void com_port::ctrl_sum_xor(QByteArray dat) // вычисление контрольной суммы
{
    int i;
    char LRC=dat.at(0);
    for(i=1;i<dat.size();++i)
    LRC=(LRC^dat.at(i));
    ctrl_sum.resize(1);
    ctrl_sum[0]=LRC;
}

void com_port::plc_data_edit(QByteArray dat)
{
    num_sectors=dat.left(4).toHex().toInt(0,16);
    num_frames=(num_sectors-512)*12/512; // вычисляем количество кадров
    first_resp=0;
    sec_resp=1;
    reading_frames=0;
    command_and_data_sector=1;
    emit frames_label(num_frames); // отправляем на форму
    command_and_read_data_sector();
}

void com_port::command_and_read_data_sector() //
{
    i_frames+=512; // счётчик считывания кадров

    if(readData.contains("OkCR")){
        qDebug()<<"No RROK"<<readData.toHex();
        return;
    }
    QByteArray ba,ba1;
    QString templ,str_i;
    int ii;
    templ="00000000";
    if(i_frames<=(num_sectors-512)){
        str_i.setNum(i_frames,16);
        for(ii=0;ii<str_i.size();++ii){
            templ.replace(templ.size()-1-ii,1,str_i.at(str_i.size()-1-ii));
        }
        str_i=templ;
        templ="00000000";
        post_data="63ff"+str_i+"01";
        ba+=post_data;
        ctrl_sum_xor(ba1.fromHex(ba));
        ba=ba1.fromHex(ba);
        ba+=ctrl_sum;
        command_for_read_frame=ba;
        ba.clear();
//        qDebug()<<command_for_read_frame.toHex();
        read_com_port();
    }
    else{
        i_frames=0;
        sec_resp=0;
        command_and_data_sector=0;
        command_for_read_frame.clear();
        read_but_flag=0;
        all_data_plc=all_data_from_plc;
        all_data_from_plc.clear(); 
//        qDebug()<<all_data_plc.toHex();
        analise_readed_data(all_data_plc);
        reset_button();
        return;
    }
//    qDebug()<<"command_and_read_data_sector is work"<<i_frames;
}

void com_port::analise_readed_data(QByteArray dat) // складывание времён кадров в массив
{
//    qDebug()<<"===================analise_readed_data====================================================";
    int n;
    for(n=0;n<num_frames;++n){ // С помощью цикла выделяем из массива данные отдельно ВРЕМЕНИ и ШИМ

        //===========================times============================================       
        int i=dat.mid(n*34,2).toHex().toInt(0,16);
        times_of_frames+=i; // складываем время всех кадров в один массив
        //===========================SHIM=============================================
        shim_of_frames+=dat.mid(n*34+2,32); // складываем все кадры в массив
        //============================================================================
    }
//    qDebug()<<"Times"<<times_of_frames.toHex();
//    qDebug()<<"SHIM"<<shim_of_frames.toHex();
//  "===================analise_readed_data====================================================";
}

void com_port::data_to_project() // Передача данных анимации из контроллера в программу
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
    data_of_frames.clear();
}

//=================================================================================================


//=============================================Write Data to com-port==========================================

void com_port::data_to_com_port(int times,QByteArray data_current_frame,int current_frame_num,int sum_frames_num)
{
    all_data_to_plc.clear();
    all_time_to_plc.clear();
    all_data_to_plc.append(data_current_frame);
    all_time_to_plc.append(times);
    if(current_frame_num+1==sum_frames_num){
        write_button();
    }

}

void com_port::write_button() // чтение анимации из контроллера
{
    if(block_press_write!=1){
        block_press_write=1;
        write_to_com_port();
    }
    else
        return;
}
void com_port::write_to_com_port() // чтение анимации из контроллера
{
    //==========================закрытие соединения====================================================
    if(connect_close==1) // если отсоединил вручнную от com_port-а, то прекращаем все действия
        return;
    //=================================================================================================
    if(ready!=1){
        write_but_flag=1;
        if(sec_resp!=1)
        first_resp=1; // первый запрос (в нулевой кадр) нужен для того, чтобы при обращении
        // к следующим кадрам через эту функцию не было путаницы в командах
        status_new_one();
        readData.clear();
        return;
    }
    else{ // готовность к приёму команды
        if(first_resp==1){
            post_data="63ff00000000029e";
        }
        else
            post_data=command_for_read_frame.toHex();
        readData.clear(); // чистим буфер
        write_end=0;
        data_to_plc=1;
        WriteToCOMPort(); // пишем данные в ком-порт  
    }
}
void com_port::data_plc_write()
{
    qDebug()<<readData<<"data_plc_write";
    control_buff=readData;
    //==========================закрытие соединения====================================================

    if(connect_close==1) // если отсоединил вручнную от com_port-а, то прекращаем все действия
        return;
    //=================================================================================================
    bool st;
    i_stat+=1; // счётчик попыток установить положительный статус ПЛК

    //===================================Errors========================================================
    if(readData.contains("ErCM") || readData.contains("ErCR")){
        qDebug()<<"data_plc_write-error"<<readData.right(4);
        ready=0;
        write_but_flag=0;
        data_to_plc=0;
        OkWR=0;
        emit error_label(readData.right(4));
        write_to_com_port();
        return;
    }
    //=================================================================================================


    if(readData.endsWith("OkWR")){ // Если ответ OkWR, Значит приём данных успешно завершён
//        qDebug()<<readData.right(4);
        i_stat=0; // счётчик запросов обнуляем
        ready=0; // флаг готовности к приёму команды обнуляем (но именно здесь, в конце) что значит, что логика выполнена
//        res_data_from_plc=0; // запрос статуса обнуляем, поскольку результат получен
//        for_frame_data.clear();
//        for_frame_data=readData; // копируем из одного буфера в другой
//        readData.clear(); // чистим буфер
//        post_data.clear();
//        read_but_flag=0;
//        block_press_read=0; // блокировка от двойного нажатия
//        OkCR=0;

//        if(for_frame_data.left(4)=="OkCR"){
//            for_frame_data.remove(0,4); // режем всякие OkCR-ы
//            OkCR=1;
//        }
//        for_frame_data.chop(4); // режем последние 4 символа (RROK) получаем чистый пакет данных + CRC
//        //===================== Verify Control Sum =============================================
//        ctrl_sum_verify(for_frame_data); // верификация контрольной суммы пакета
//        if(verify_ctrl_sum==0){
//            if(ctrl_sum_errors!=10){ // Если количество попыток запроса меньше 10
//                qDebug()<<"ctrl_sum_errors";
//                read_com_port(); // Заново пытаемся прочесть данные
//            }else{
//                res_data_from_plc=0; //если число запросов превысило лимит, то прекращаем попытки
//                i_stat=0; // счётчик запросов тоже обнуляем
//                ctrl_sum_errors=0;
//                ready=0;
//                read_but_flag=0;
//                block_press_read=0; // блокировка от двойного нажатия
//                post_data.clear();
//                readData.clear();
//                status_controller=0;
//                OkCR=0;
//                emit status(st=false); //отправка флага о статусе в мэйнвиндоу
//                emit error_label("No communication with the controller, most likely");
//                return;
//            }
//        }
//        //=======================================================================================
//        emit data_from_com(for_frame_data);
//        //=======================================================================================

//        if(first_resp==1){
//            plc_data_edit(for_frame_data); // Обработка данных нулевого сектора
//            return;
//        }

//        //=======================================================================================
//        if(sec_resp==1){
//            all_data_from_plc+=for_frame_data.left(for_frame_data.size()-1); // убираем контрольную сумму
//            for(int i=0;i<12;++i) // Этот цикл вообще чисто для красоты
//            reading_frames+=1; // Для типа плавного подсёта кадров (потому что читает по секторам, а там по 12 кадров)
//            if(end_read_data_anim==1){
//                command_and_read_data_sector(); // читаем следующий сектор данных с контроллера
//                end_read_data_anim=0;
//            }
//            emit num_frame_read(reading_frames);
////            qDebug()<<"data_frames"<<all_data_from_plc.toHex();
//            return;
//        }


////        qDebug()<<for_frame_data.toHex();

//    }else{
//        if(i_stat<10){ // Если количество попыток запроса меньше 10
////            qDebug()<<"!=RROK"<<readData;
//            read_com_port(); // Заново пытаемся получить статус
//        }else{
//           res_data_from_plc=0; //если число запросов превысило лимит, то прекращаем попытки
//           i_stat=0; // счётчик запросов тоже обнуляем
//           ctrl_sum_errors=0;
//           ready=0;
//           read_but_flag=0;
//           block_press_read=0; // блокировка от двойного нажатия
//           post_data.clear();
//           readData.clear();
//           status_controller=0;
//           OkCR=0;
//           emit status(st=false); //отправка флага о статусе в мэйнвиндоу
//           emit error_label("No communication with the controller, most likely");
//           return;
//        }
//    }
}
