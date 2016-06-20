#include "comport.h"

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
}

ComPort::~ComPort()
{
    serial_port->close();
}

QByteArray ComPort::read()
{
    QByteArray data;
    data.append(serial_port->readAll());
    return data;
}

int ComPort::write(QByteArray data)
{
    QByteArray send_data;
    return serial_port->write(send_data.fromHex(data));
}
