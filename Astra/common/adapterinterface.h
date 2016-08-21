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
    explicit AdapterInterface(QString type);
    ~AdapterInterface();

    virtual void portConnect();   
    virtual bool portOpen();
    virtual QByteArray read();
    virtual int write(QByteArray);
    virtual void port(QString);
    QString port_num;
    int type_connection;
    bool port_ok;

protected:
    ComPort * mCom;
    Tcpclient * mTcp;

signals:
    void finish_read();
    void PortError(QByteArray);
    bool ok_connect();

public slots:
    virtual void ok_connect_st();

};



#endif // ADAPTERINTERFACE_H
