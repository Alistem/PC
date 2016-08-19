#ifndef ADAPTERINTERFACE_H
#define ADAPTERINTERFACE_H

#include <QObject>
#include <QtCore>

#include "common/iadapterinterface.h"
#include "comport.h"
#include "tcp_client.h"

class ComPort;
class Tcpclient;
class AdapterInterface : public IAdapterInterface
{
    Q_OBJECT

public:
    explicit AdapterInterface(QObject *parent/*, ComPort *pCom = 0, Tcpclient *pTcp = 0*/);
    ~AdapterInterface();

    virtual bool portConnect();
    virtual QByteArray read();
    virtual int write(QByteArray);
    virtual void port(QString);

protected:
    ComPort * mCom;
    Tcpclient * mTcp;

};



#endif // ADAPTERINTERFACE_H
