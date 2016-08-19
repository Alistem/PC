#ifndef ADAPTERINTERFACE_H
#define ADAPTERINTERFACE_H

#include <QObject>
#include <QtCore>

#include "common/iadapterinterface.h"
//#include "comport.h"
//#include "tcp_client.h"

class AdapterInterface : public IAdapterInterface/*, private ComPort , private Tcpclient*/
{
    Q_OBJECT

public:
    explicit AdapterInterface(QObject *parent = 0);
    ~AdapterInterface();
    QString port;

    virtual bool portConnect();
    virtual QByteArray read();
    virtual int write(const QByteArray &data);
    //virtual void port(QString);

};



#endif // ADAPTERINTERFACE_H
