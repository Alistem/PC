#include "reset.h"

Reset::Reset()
{

}

void Reset::sendCommandToPort(ComPort *serial_port, QString data)
{
    QString listen = "63";
    QByteArray buffer;
    buffer+=listen;
    serial_port->write(buffer);
}
