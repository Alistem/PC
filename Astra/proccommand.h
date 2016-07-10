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
    void frames_label(int);

public slots:
    void slot_connect(int num);
    void slot_disconnect();
    void slot_status();
    void slot_reset();
    void command_read(QString command);
    void slot_read();
    void slot_write(QList<QString> animation);
    void comPortError(QByteArray com_port_error);
    void listen_on_off();

private:
    ComPort *com_port;
    QByteArray BufferReadData,TempReadData;
    int flag_command,ctrl_sum_errors;
    bool ctrl_sum_verify(QByteArray);
    QByteArray ctrl_sum_xor(QByteArray);
    int nums_all_frames();
    QString sector;
    bool nums_all_frames_flag;

};

#endif // PROCCOMMAND_H
