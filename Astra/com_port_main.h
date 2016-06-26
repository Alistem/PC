#ifndef COM_PORT_MAIN_H
#define COM_PORT_MAIN_H

#include <QDialog>

#include "ui_com_port1.h"
#include "proccommand.h"
#include "frameinfo.h"

class com_port_w : public QWidget
{
    Q_OBJECT
    
public:

    explicit com_port_w(QWidget *parent = 0);
    ~com_port_w();

signals:
    void com_connect();
    void com_disconnect();
    void data_com_port_ext(QString);
    void num_com_port(int);
    void disconnect();
    void flag_close_win(bool);
    void import_data_to_project();
    void times_from_plc(int,int,int);
    void shim_from_plc(QByteArray,int);
    void res_data_to_plc();
    void data_to_astra(QList<FrameInfo>);
    void num_com_proccommand(int);
    
private slots:

    void one(QByteArray basta);
    void connect_status(QString status);
    void connect_to_proccommand();
    void com_port_num_res();
    void closeEvent(QCloseEvent *ev);
    void error_label_main(QString error);
    void read_data(QByteArray recieve_data);
    void status_plc(bool status_plc);
    void frames_label_main(int frames);
    void num_readed_frames(int num_frame);

    void on_pushButton_clicked();

    void on_readButton_clicked(bool checked);

    void on_writeButton_clicked(bool checked);


private:
    Ui::com_port *ui;
//    QString hex;
    QString hex_0x0,frames_reading;
    QString back_color_on;
    QString back_color_off;
    QString back_color_none;
    int hex_int,int_num_frames_summ;
    QByteArray cc,ba;
    QList<QByteArray> frames_list_to_com_port; // массив кадров
    QList<int> frames_time_to_com_port; //массив времён кадров
    ProcCommand *proccommand;
};

#endif // COM_PORT_MAIN_H
