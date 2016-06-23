#ifndef COMPORT_H
#define COMPORT_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QtCore>

class ComPort : public QObject
{
public:
    explicit ComPort(int number);
	~ComPort();

    bool dataRecived();
    QByteArray read();
    int write(QByteArray data);

protected:
	ComPort();
    ComPort(const ComPort &);
    operator= (const ComPort);

private slots:
    void slot_readFromSerialPort();
    void connector();
    void close();

private:
    QByteArray  read_data;
    QTimer timer;
    QSerialPort *serial_port;
    QSerialPortInfo *port_info;	
};

#endif // COMPORT_H
