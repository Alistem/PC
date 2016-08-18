#include "tcp_client.h"

Tcpclient::Tcpclient(QObject *parent) :
    QObject(parent), m_nNextBlockSize(0)
{

    s_address = "10.10.33.1";
    port = 1024;

    m_pTcpSocket = new QTcpSocket(this);
    m_pTcpSocket->connectToHost(s_address,port);

    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,         SLOT(slotError(QAbstractSocket::SocketError)));

    connect(ui->message_send, SIGNAL(returnPressed(QByteArray)),SLOT(slotSendToServer(QByteArray)));


}

Tcpclient::~Tcpclient()
{
    if(connect)
        m_pTcpSocket->close();
    delete m_pTcpSocket;
}

void Tcpclient::slotSendToServer(QByteArray buffer){
    m_pTcpSocket->write(buffer);
    buffer.clear();
}

void Tcpclient::slotConnected() {
    connect = true;
    emit info("Connected");
}

void Tcpclient::slotReadyRead() {
    QByteArray buff;

    for (;;) {
        if (!m_nNextBlockSize) {
            if (m_pTcpSocket->bytesAvailable() < sizeof(quint8))
                return;
            m_nNextBlockSize = m_pTcpSocket->bytesAvailable();
        }
        qDebug()<<"m_nNextBlockSize"<<m_nNextBlockSize;

        if (m_pTcpSocket->bytesAvailable() < m_nNextBlockSize)
            return;

        buff=m_pTcpSocket->readAll();

        qDebug()<<"buff"<<buff.toHex();

        ui->messages->append(QString::fromUtf8(buff.toHex()));

        m_nNextBlockSize = 0;
    }
}

void Tcpclient::slotError(QAbstractSocket::SocketError err){
    QString strError =
            "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                             "The host was not found." :
                             err == QAbstractSocket::RemoteHostClosedError ?
                                 "The remote host is closed." :
                                 err == QAbstractSocket::ConnectionRefusedError ?
                                     "The connection was refused." :
                                     QString(m_pTcpSocket->errorString())
                                     );
    ui->messages->append(strError);
}
