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
    void ReadCOMPort();
    void Read_finish();
    int write(QByteArray data);
    QByteArray  m_readData;
protected:
	ComPort();
    ComPort(const ComPort &);
    operator= (const ComPort);
private:
    void connector();
    void close();
    QTimer r_timer;
private:
    QSerialPort *serial_port;
    QSerialPortInfo *port_info;
	
};

#endif // COMPORT_H
