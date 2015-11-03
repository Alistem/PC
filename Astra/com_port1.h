#ifndef COM_PORT1_H
#define COM_PORT1_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>

class com_port : public QWidget
{
    Q_OBJECT
public:
    explicit com_port(QWidget *parent = 0);

private:
    QSerialPort *com1;
    QSerialPortInfo *port_info;
    QString post_data;
    int num_com_port;
    QByteArray  m_readData,readData;
    QByteArray  all_data_from_plc;
    QByteArray for_frame_data,command_for_read_frame,command_for_write_frame;

    QList <int> all_time_to_plc;
    QList <QByteArray>all_data_to_plc;
    QTimer m_timer,r_timer,stat_timer,stat_timer_2,stat_timer_read_data,stat_but_timer;
    QTimer reset_timer;
    bool connect_close;
    bool read_end,write_end,res_stat_plc,status_butt,res_data_from_plc,data_to_plc,status_controller,ready;
    bool block_press_read,block_press_write,block_press_status,verify_ctrl_sum,status_but_flag,read_but_flag,write_but_flag;
    bool block_press_reset,reset_but_flag,reset_butt,reset;
    bool first_resp,sec_resp,command_and_data_sector,OkCR,OkWR;
    int i_stat,i_write,ctrl_sum_errors,i_frames,num_frames;
    int num_sectors,reading_frames,end_read_data_anim;
    QByteArray ctrl_sum,frames_from_plc,control_buff,all_data_plc;
    QByteArray times_of_frames,data_of_frames,shim_of_frames;
    
signals:
    void connect_com();
    void disconnect_com();
    void read_com();
    void write_com();
    void connect_label(QString);
    void data_from_com(QByteArray);
    void com_port_num();
    void error_label(QString);
    void frames_label(int);
    void status(bool);
    void num_frame_read(int);
    void times_from_plc1(int,int,int);
    void shim_from_plc1(QByteArray,int);


public slots:
    void ReadCOMPort();
    void WriteToCOMPort();
    void COMConnector();
    void COMClose();
    void data_com_port_post(QString post);
    void num_com_port_com(int num);
//    void serialReceived();

private slots:
    void init();
    void readTimeout();
    void readError(QSerialPort::SerialPortError serialPortError);
    void Read_finish();
    void status_plc_transmit();
    void data_plc_read();
    void data_plc_write();
    void read_com_port();
    void read_button();
    void write_to_com_port();
    void write_button();
    void status_button();
    void status_button_2();
    void reset_button();
    void reset_button_2();
    void reset_button_3();
    void status_new_one();
    void status_new_two();
    void ctrl_sum_xor(QByteArray dat);
    void ctrl_sum_verify(QByteArray dat);
    void plc_data_edit(QByteArray dat);
    void command_and_read_data_sector();
    void analise_readed_data(QByteArray dat);
    void data_to_project();
    void data_to_com_port(int,QByteArray,int,int);
    void packet_to_plc();
    void first_sector_data();
    void other_sector_data();
    void command_write_sector();
};

#endif // COM_PORT1_H
