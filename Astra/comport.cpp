#include "ComPort.h"
#include "operation.h"
#include <QtCore>

QT_USE_NAMESPACE

ComPort::ComPort(QString port, QObject *parent): port_open(false),
    QObject(parent)
{
    read_finish=false;

    serial_port = new QSerialPort(port);

    port_open = serial_port->open(QIODevice::ReadWrite);

    connect(serial_port,SIGNAL(readyRead()),SLOT(readComPort()));
    connect(&r_timer, SIGNAL(timeout()),SLOT(readFinish()));

    serial_port->setBaudRate(QSerialPort::Baud57600);
    serial_port->setFlowControl(QSerialPort::NoFlowControl);
    serial_port->setParity(QSerialPort::NoParity);
    serial_port->setDataBits(QSerialPort::Data8);
    serial_port->setStopBits(QSerialPort::OneStop);

    if(portOpen() == false)
        qDebug() << "Error connect";
    else
        qDebug() << port;
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
    //QByteArray data;
    //if (dataRecived()){
    //    data = read_data;
    //    read_data.clear();
    //}
    //data = serial_port->readAll();
    read_finish=false;
    return read_data;
}

void ComPort::readComPort()
{
    QByteArray m_readData;
    m_readData.append(serial_port->readAll());
    read_data+=m_readData;
    m_readData.clear();
    r_timer.start(50);
}

void ComPort::readFinish()
{
    r_timer.stop();
    read_finish=true;
    qDebug()<<"Read_finish";
}

bool ComPort::portOpen()
{
    return port_open;
}
bool ComPort::portReaded()
{
    return read_finish;
}

