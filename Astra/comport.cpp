#include "comport.h"
#include "operation.h"

ComPort::ComPort(int number)
{ 
    QString port_num=QString("%1%2").arg("COM").arg(number);
    serial_port = new QSerialPort(port_num); //��� Win
    if(serial_port->open(QIODevice::ReadWrite) == false){
        // ��������� ������ ����������
    }
    const unsigned int BAUDRATE = 57600;
    serial_port->setBaudRate(BAUDRATE);
    serial_port->setFlowControl(QSerialPort::NoFlowControl);
    serial_port->setParity(QSerialPort::NoParity);
    serial_port->setDataBits(QSerialPort::Data8);
    serial_port->setStopBits(QSerialPort::OneStop);

    connect(serial_port,SIGNAL(readyRead()),this,SLOT(ReadCOMPort()));
    connect(serial_port,SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(readError(QSerialPort::SerialPortError)));
    connect(&r_timer, SIGNAL(timeout()), SLOT(Read_finish()));
}

ComPort::~ComPort()
{
    serial_port->close();
}

void ComPort::ReadCOMPort()
{
    QByteArray data;
    data.append(serial_port->readAll());
    m_readData+=data;
    r_timer.start(10);
}

void ComPort::Read_finish()
{

//    if(connect_close==1) // ���� ���������� �������� �� com_port-�, �� ���������� ��� ��������
//        return;
//    r_timer.stop();
//    read_end=1; // ���� ��� �����������, ��� ������ �����������
//    if(reset_butt==1)
//        reset_button_3();
//    if(res_stat_plc==1) // �������, ��� ������� ������� ������� ���
//        status_new_two();
//    if(status_butt==1) // �������, ��� ������� ������� ������� ���
//        status_plc_transmit();
//    if(command_and_data_sector==1)// �������, ��� ������� ������ ������ ��������
//        end_read_data_anim=1; // ������ ��������� ������ ������ � �����������
//    if(res_data_from_plc==1)// �������, ��� ������� ������ ������ ��������
//        data_plc_read();
//    if(data_to_plc==1){// �������, ��� ������� ������ ������ ��������
//        data_plc_write();
//    }
}

int ComPort::write(QByteArray data)
{
    QByteArray send_data;
    return serial_port->write(send_data.fromHex(data));
}
