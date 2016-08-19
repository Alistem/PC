#include "reset.h"

Reset::Reset()
{

}

QByteArray Reset::operation(IAdapterInterface *port, QString string)
{
    QString listen = "63ff00000000039f";
    QByteArray buffer;
    buffer+=listen;
    uint res = port->write(buffer);

    return buffer;
}
