#include "iadapterinterface.h"


QT_USE_NAMESPACE

IAdapterInterface::IAdapterInterface(QObject *parent): port_open(false),
    QObject(parent)
{

}

IAdapterInterface::~IAdapterInterface()
{

}
