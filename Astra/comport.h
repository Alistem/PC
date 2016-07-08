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
    bool read_finish;

private:
    QByteArray  read_data,port_error;
    QTimer r_timer;
    QSerialPort *serial_port;
    QSerialPortInfo *port_info;
    bool port_open;


private slots:
    void readComPort();
    void readFinish();
    void readError(QSerialPort::SerialPortError serialPortError);

signals:
    void finish_read();
    void PortError(QByteArray);

};

#endif // COMPORT_H
