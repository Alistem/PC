#ifndef COMPORT_H
#define COMPORT_H

#include <QtCore>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>

class ComPort : public QObject
{
public:
    ComPort(QString port, QObject *obj = 0);
	~ComPort();

    bool dataRecived();
    QByteArray read();
    int write(QByteArray data);
    bool portOpen();

public slots:
    void slot_readFromSerialPort();

private:
    QByteArray  read_data;
    QTimer timer;
    QSerialPort *serial_port;
    QSerialPortInfo *port_info;
    bool port_open;
};

#endif // COMPORT_H
