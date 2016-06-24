#include <QDebug>
#include "getstatus.h"

GetStatus::GetStatus()
{

}

QByteArray GetStatus::operation(ComPort* port, QString string)
{
    QString listen = "63ff00000000009c";
    QByteArray buffer;
    buffer+=listen;
    uint res = port->write(buffer);

    return buffer;
}
