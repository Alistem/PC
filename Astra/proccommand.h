#ifndef PROCCOMMAND_H
#define PROCCOMMAND_H

#include <QObject>
#include <QTimer>
#include "comport.h"
#include "frameinfo.h"


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
    void connect_label(QString);
    void status(bool);
    void error_label(QString);
    void frames_label(int);
    void num_frame_read(int);
    void data_from_com(QByteArray);
    void times_from_plc1(int,int,int);
    void shim_from_plc1(QByteArray,int);
    void write_tcp(QString);
    void read_tcp(QString);

public slots:
    void slot_connect(QString);
    void slot_disconnect();
    void slot_status();
    void slot_reset();

    void slot_read();
    void slot_write();
    void comPortError(QByteArray com_port_error);
    void listen_on_off();
    void data_to_project();
    void data_from_project(QList<FrameInfo>);
    void connection_type(int);
    void data_from_tcp(QString);

private:
    ComPort *com_port;
    QByteArray BufferReadData,TempReadData,all_data_from_plc,times_of_frames,shim_of_frames;
    int flag_command,read_stage,write_stage,ctrl_sum_errors,num_frames,current_sector,errors,i_write;
    int type_connect;
    bool ctrl_sum_verify(QByteArray);
    QByteArray ctrl_sum_xor(QByteArray);
    QString data_tcp;
    void nums_all_frames();
    void data_all_frames();
    void analise_reading_data();
    QString sector;
    QTimer connect_tm;
    bool nums_all_frames_flag,data_all_frames_flag;
    void command_read(QString command);
    void command_write(QString command);
    void data_to_zero_sector(QList<FrameInfo>);
    void data_to_other_sector(QList<FrameInfo>);
    QList<FrameInfo> animation;
};

#endif // PROCCOMMAND_H
