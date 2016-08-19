#ifndef IADAPTERINTERFACE_H
#define IADAPTERINTERFACE_H

#include <QObject>
#include <QtCore>


class IAdapterInterface: public QObject
{
    Q_OBJECT

  public:
    explicit IAdapterInterface(QObject *parent);
    virtual ~IAdapterInterface();
    virtual bool portConnect();
    virtual QByteArray read();
    virtual int write(QByteArray data);
    virtual void port(QString) = 0;

signals:
    void finish_read();
    void PortError(QByteArray);

private:
    bool port_open;

};

#endif // IADAPTERINTERFACE_H
