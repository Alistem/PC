#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>

class Tcpclient : public QObject {
    Q_OBJECT
public:
    explicit Tcpclient(QObject *parent);
    virtual ~Tcpclient();
    QByteArray read();
    int write(QByteArray);

signals:
    void finish_read();
    void info(QString);
    void ok_connect_tcp();

public slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError err);
    void slotSendToServer(QByteArray);
    void slotConnected();
    void readFinish();

private:
    QTimer r_timer;
    QTcpSocket *m_pTcpSocket;
    QHostAddress s_address;
    QByteArray read_data;
    quint16 m_nNextBlockSize;
    quint16 port;
    bool connect_st;
    int timeout;

};

#endif // TCP_CLIENT_H
