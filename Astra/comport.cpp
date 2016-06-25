#include "comport.h"
#include "operation.h"


ComPort::ComPort(QString port, QObject *obj): QObject(obj), port_open(false)
{
    serial_port = new QSerialPort(port);
    qDebug() << port;
    port_open = serial_port->open(QIODevice::ReadWrite);
    if(portOpen() == false){
        qDebug() << "Error connect";
    }

    serial_port->setBaudRate(QSerialPort::Baud9600);
    serial_port->setFlowControl(QSerialPort::NoFlowControl);
    serial_port->setParity(QSerialPort::NoParity);
    serial_port->setDataBits(QSerialPort::Data8);
    serial_port->setStopBits(QSerialPort::OneStop);

    //connect(serial_port, SIGNAL(readyRead()), this, SLOT(slot_readFromSerialPort()));
}

ComPort::~ComPort()
{
    if (portOpen())
        serial_port->close();
    delete serial_port;
}

void ComPort::slot_readFromSerialPort()
{
    QByteArray data;
    data.append(serial_port->readAll());
    read_data+=data;
    timer.start(10);
    qDebug() << "Data exist!!!";
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
