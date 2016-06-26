#ifndef COMPORT_H
#define COMPORT_H

#include <QtCore>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>

class ComPort
{
public:
    explicit ComPort(QString port);
	~ComPort();

    bool dataRecived();
    QByteArray read();
    int write(QByteArray data);
    bool portOpen();

private:
    QByteArray  read_data;
    QTimer timer;
    QSerialPort *serial_port;
    QSerialPortInfo *port_info;
    bool port_open;
};

#endif // COMPORT_H
