#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <QTcpSocket>
#include <QHostAddress>

class Tcpclient : public QObject {
    Q_OBJECT
public:
    explicit Tcpclient(QObject *parent);
    virtual ~Tcpclient();

signals:
    void message(QByteArray);
    void info(QString);

public slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError err);
    void slotSendToServer(QByteArray);
    void slotConnected();

private:

    QTcpSocket *m_pTcpSocket;
    QHostAddress s_address;
    quint16 m_nNextBlockSize;
    quint16 port;
    bool connect;

};

#endif // TCP_CLIENT_H
