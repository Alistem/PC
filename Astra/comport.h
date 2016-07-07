#ifndef COMPORT_H
#define COMPORT_H

#include <QObject>
#include <QtCore>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>


class ComPort: public QObject
{
    Q_OBJECT
public:
    explicit ComPort(QString port, QObject *parent = 0);
    ~ComPort();

    bool dataRecived();
    QByteArray read();
    int write(QByteArray data);
    bool portOpen();
    bool portReaded();

private:
    QByteArray  read_data;
    QTimer r_timer;
    QSerialPort *serial_port;
    QSerialPortInfo *port_info;
    bool port_open,read_finish;


private slots:
    void readComPort();
    void readFinish();
};

#endif // COMPORT_H
