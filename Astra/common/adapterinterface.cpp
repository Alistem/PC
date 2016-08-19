#include "adapterinterface.h"

QT_USE_NAMESPACE

AdapterInterface::AdapterInterface(QObject *parent)
    : IAdapterInterface(parent)
{
    QString str;
    mCom = new ComPort(str,this);
    mTcp = new Tcpclient(this);
}

AdapterInterface::~AdapterInterface()
{

}

bool AdapterInterface::portConnect()
{
    bool s = true;
 return  s;
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

}
