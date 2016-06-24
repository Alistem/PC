#include "readflash.h"

ReadFlash::ReadFlash()
{

}

QByteArray ReadFlash::operation(ComPort* port, QString string)
{
    QString listen = "63ff00000000019d";
    QByteArray buffer;
    buffer+=listen;
    uint res = port->write(buffer);

    //if (port->dataRecived()){
        buffer = port->read();
    //}

    return buffer;
}
