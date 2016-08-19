#ifndef IADAPTERINTERFACE_H
#define IADAPTERINTERFACE_H

#include <QObject>
#include <QtCore>


class IAdapterInterface: public QObject
{
    Q_OBJECT

  public:
    explicit IAdapterInterface(QObject *parent);
    virtual ~IAdapterInterface() = 0;
    virtual bool portConnect() = 0;
    virtual QByteArray read() = 0;
    virtual int write(QByteArray) = 0;
    virtual void port(QString) = 0;

signals:
    void finish_read();
    void PortError(QByteArray);

private:
    bool port_open;

};

#endif // IADAPTERINTERFACE_H
