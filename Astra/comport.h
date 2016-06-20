#ifndef COMPORT_H
#define COMPORT_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class ComPort
{
public:
    explicit ComPort(int number);
	~ComPort();
    QByteArray read();
    int write(QByteArray data);
protected:
	ComPort();
    ComPort(const ComPort &);
    operator= (const ComPort);
private:
    void connector();
    void close();

private:
    QSerialPort *serial_port;
    QSerialPortInfo *port_info;
	
};

#endif // COMPORT_H
