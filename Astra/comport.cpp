#include "comport.h"
#include "operation.h"

ComPort::ComPort(int number)
{ 
    QString port_num=QString("%1%2").arg("COM").arg(number);
    serial_port = new QSerialPort(port_num); //для Win
    if(serial_port->open(QIODevice::ReadWrite) == false){
        // Обработка ошибки соединения
    }
    const unsigned int BAUDRATE = 57600;
    serial_port->setBaudRate(BAUDRATE);
    serial_port->setFlowControl(QSerialPort::NoFlowControl);
    serial_port->setParity(QSerialPort::NoParity);
    serial_port->setDataBits(QSerialPort::Data8);
    serial_port->setStopBits(QSerialPort::OneStop);

    connect(serial_port,SIGNAL(readyRead()),this,SLOT(slot_readFromSerialPort()));
}

ComPort::~ComPort()
{
    serial_port->close();
}

void ComPort::slot_readFromSerialPort()
{
    QByteArray data;
    data.append(serial_port->readAll());
    read_data+=data;
    timer.start(10);
}

int ComPort::write(QByteArray data)
{
    QByteArray send_data;
    return serial_port->write(send_data.fromHex(data));
}

bool ComPort::dataRecived()
{
    return read_data.isEmpty();
}

QByteArray ComPort::read()
{
    QByteArray data;
    if (dataRecived()){
        data = read_data;
        read_data.clear();
    }
    return data;
}
