#include "comport.h"
#include "operation.h"
#include <QtCore>



ComPort::ComPort(QString port): port_open(false)
{
    serial_port = new QSerialPort(port);
    //qDebug() << port;
    port_open = serial_port->open(QIODevice::ReadWrite);
    if(portOpen() == false){
        qDebug() << "Error connect";
        //connect(serial_port,SIGNAL(readyRead()),this,SLOT(slot_reset()));
    }

    serial_port->setBaudRate(QSerialPort::Baud57600);
    serial_port->setFlowControl(QSerialPort::NoFlowControl);
    serial_port->setParity(QSerialPort::NoParity);
    serial_port->setDataBits(QSerialPort::Data8);
    serial_port->setStopBits(QSerialPort::OneStop);
}

ComPort::~ComPort()
{
    if (portOpen())
        serial_port->close();
    delete serial_port;
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
    //if (dataRecived()){
    //    data = read_data;
    //    read_data.clear();
    //}
    data = serial_port->readAll();
    return data;
}

bool ComPort::portOpen()
{
    return port_open;
}
