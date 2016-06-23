#include "reset.h"

Reset::Reset()
{

}

QByteArray Reset::operation(ComPort* port, QString string)
{
    QString listen = "63ff00000000039f";
    QByteArray buffer;
    buffer+=listen;
    port->write(buffer);
}
