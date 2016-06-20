#ifndef COMPORT_H
#define COMPORT_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class ComPort
{
public:
    explicit ComPort(int numer);
	~ComPort();
	void read();
    void write();
protected:
	ComPort();
private:
    void connector();
    void close();

private:
    QSerialPort *serial_port;
    QSerialPortInfo *port_info;
	
};

#endif // COMPORT_H
