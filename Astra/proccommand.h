#ifndef PROCCOMMAND_H
#define PROCCOMMAND_H

#include <QObject>
#include <QTimer>
#include "comport.h"


class Gui;

class ProcCommand : public QObject
{
    Q_OBJECT
public:
    explicit ProcCommand(QObject *parent = 0);

signals:
    void signal_status();
    void com_port_num();
    void connection(QString);
    void status(bool);
    void error_label(QString);

public slots:
    void slot_connect(int num);
    void slot_disconnect();
    void slot_status();
    void slot_reset();
    void slot_read();
    void slot_write(QList<QString> animation);
    void comPortError();
    void listen_on_off();

private:
    ComPort *com_port;
    QByteArray BufferReadData,TempReadData;
    int flag_command;

};

#endif // PROCCOMMAND_H
