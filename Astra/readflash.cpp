#include "readflash.h"

ReadFlash::ReadFlash()
{

}

QByteArray ReadFlash::operation(ComPort* port, QString string)
{
    QString listen = "63ff00000000019d";
    QByteArray buffer;
    buffer+=listen;
    port->write(buffer);
}
