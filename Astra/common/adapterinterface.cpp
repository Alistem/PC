#include "adapterinterface.h"

QT_USE_NAMESPACE

AdapterInterface::AdapterInterface(QString type)
    : IAdapterInterface(type),port_ok(0)
{
    type_connection = type.toInt(0);
}

AdapterInterface::~AdapterInterface()
{

}

void AdapterInterface::port(QString num)
{
    port_num = num;
}

void AdapterInterface::portConnect()
{
    switch (type_connection) {
    case 1:
        mCom = new ComPort(port_num);
        //port_ok = mCom->portOpen();
        connect(mCom,SIGNAL(ok_com()),SLOT(ok_connect_st()));
        qDebug()<<"ComPort"<<port_num<<port_ok;
        break;
    case 2:
        mTcp = new Tcpclient(this);
        connect(mTcp,SIGNAL(ok_connect_tcp()),SLOT(ok_connect_st()));
        qDebug()<<"Tcpclient"<<port_num<<port_ok; 
        break;

    default:
        break;
    }
}

void AdapterInterface::ok_connect_st()
{
    port_ok = 1;
    emit ok_connect();
    qDebug()<<"AdapterInterface::ok_connect_st()";
}

bool AdapterInterface::portOpen()
{
    return port_ok;
}

QByteArray AdapterInterface::read()
{
    QByteArray buffer;

    switch (type_connection) {
    case 1:
        buffer = mCom->read();
        break;
    case 2:
        buffer = mTcp->read();
        break;

    default:
        break;
    }
    return buffer;
}

int AdapterInterface::write(QByteArray data)
{
    switch (type_connection) {
    case 1:
        mCom->write(data);
        break;
    case 2:
        mTcp->write(data);
        break;

    default:
        break;
    }
    return data.size();
}


