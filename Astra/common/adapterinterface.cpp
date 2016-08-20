#include "adapterinterface.h"

QT_USE_NAMESPACE

AdapterInterface::AdapterInterface(QString type)
    : IAdapterInterface(type)
{
    type_connection = type.toInt(0);
}

AdapterInterface::~AdapterInterface()
{

}

void AdapterInterface::portConnect()
{
    switch (type_connection) {
    case 1:
        mCom = new ComPort(port_num);
        port_ok = mCom->portOpen();
        qDebug()<<"ComPort"<<port_num<<port_ok;
        break;
    case 2:
        mTcp = new Tcpclient(this);
        port_ok = mTcp->slotConnected();
        qDebug()<<"Tcpclient"<<port_num<<port_ok;
        break;

    default:
        break;
    }
}

bool AdapterInterface::portOpen()
{
    return port_ok;
}

QByteArray AdapterInterface::read()
{
    QByteArray s("re");
    return s;
}

int AdapterInterface::write(QByteArray data)
{
    return data.size();
}

void AdapterInterface::port(QString num)
{
    port_num = num;
}
