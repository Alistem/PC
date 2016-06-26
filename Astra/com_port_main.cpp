#include "com_port_main.h"
#include "ui_com_port1.h"
#include <QtCore/QDebug>
#include <QFileDialog>
#include <QBuffer>
#include <QMessageBox>
#include <iostream>
#include "com_port1.h"
#include "proccommand.h"
#include "mainwindow.h"
//#include <QApplication>
#include <QCloseEvent>

QT_USE_NAMESPACE

com_port_w::com_port_w(QWidget *parent)
    : QWidget(parent)
    , proccommand(new ProcCommand(this))
    , ui(new Ui::com_port)
{
    ui->setupUi(this);
    com_port *com_port1 = new com_port(this); // создаём объект класса com_port. Для реализации ком-порта
    com_port1->hide(); // скрываем объект с формы, а то он меню загораживает
    ui->progressBar_frames->hide();
    ui->connectButton->setDefault(true);
    connect(this->ui->connectButton,SIGNAL(clicked()),this,SLOT(connect_to_proccommand()));
    connect(this->ui->disconnectButton,SIGNAL(clicked()),proccommand,SLOT(slot_disconnect()));
    //=====================================Write Data===================================================
    connect(this,SIGNAL(data_to_astra(QList<FrameInfo>)),proccommand,SLOT(slot_write(QList<FrameInfo>)));
    //=========================================================================================
    connect(this->ui->writeButton,SIGNAL(clicked()),this,SIGNAL(res_data_to_plc()));
    connect(this->ui->readButton,SIGNAL(clicked()),proccommand,SLOT(slot_read()));
    connect(this->ui->Status_PLC,SIGNAL(clicked()),proccommand,SLOT(slot_status()));
    connect(this->ui->reset,SIGNAL(clicked()),proccommand,SLOT(slot_reset()));
    connect(this,SIGNAL(disconnect()),proccommand,SLOT(slot_disconnect()));
    connect(this,SIGNAL(data_com_port_ext(QString)),com_port1,SLOT(data_com_port_post(QString)));
    connect(com_port1,SIGNAL(connect_label(QString)),this->ui->connect_color,SLOT(setText(QString)));
    connect(com_port1,SIGNAL(connect_label(QString)),this,SLOT(connect_status(QString)));
    connect(com_port1,SIGNAL(data_from_com(QByteArray)),this,SLOT(read_data(QByteArray)));
    connect(com_port1,SIGNAL(com_port_num()),this,SLOT(com_port_num_res()));
    connect(this,SIGNAL(num_com_proccommand(int)),proccommand,SLOT(slot_connect(int)));
    connect(com_port1,SIGNAL(error_label(QString)),this,SLOT(error_label_main(QString))); 
    connect(com_port1,SIGNAL(status(bool)),this,SLOT(status_plc(bool)));
    connect(com_port1,SIGNAL(frames_label(int)),this,SLOT(frames_label_main(int)));
    connect(com_port1,SIGNAL(num_frame_read(int)),this,SLOT(num_readed_frames(int)));
    connect(this,SIGNAL(import_data_to_project()),com_port1,SLOT(data_to_project()));
    //===========================Данные с контроллера в проект====================================
    connect(com_port1,SIGNAL(times_from_plc1(int,int,int)),this,SIGNAL(times_from_plc(int,int,int)));
    connect(com_port1,SIGNAL(shim_from_plc1(QByteArray,int)),this,SIGNAL(shim_from_plc(QByteArray,int)));


    back_color_on="QLabel{background-color: rgb(0, 255, 0);}";
    back_color_off="QLabel{background-color: rgb(255, 0, 0);}";
    back_color_none="QLabel{background-color: rgb(255, 255, 255,0);}";

}

com_port_w::~com_port_w()
{
    delete ui;
}

void com_port_w::one(QByteArray basta)
{
        for (int i=0; i<basta.length(); ++i) {
            printf("%c - %x\n", basta.at(i), basta.at(i));
        }
        printf("\n");
//    qDebug()<<cc.at(0);
//    QString strq=cc;
return;
}

void com_port_w::connect_status(QString status)
{
    if (status=="Connected"){
        ui->progressBar_frames->hide();
        ui->connect_color->setStyleSheet(back_color_on);
        ui->connectButton->setEnabled(false);// блокировка кнопки
        ui->comboBox->setEnabled(false); // блокировка комбобокса
    }
    if (status=="Disconnected"){
        ui->progressBar_frames->hide();
        ui->connect_color->setStyleSheet(back_color_off);
        ui->status_controller->setStyleSheet(back_color_off);
        ui->connectButton->setEnabled(true); // блокировка кнопки
        ui->comboBox->setEnabled(true); // блокировка комбобокса
        ui->label_num_frames->setText("");
        ui->label_num_frames_2->setText("");
        ui->num_current_frame->setText("");
    }
    if (status!="Disconnected" && status!="Connected"){
        ui->progressBar_frames->hide();
        ui->connect_color->setStyleSheet(back_color_none);
        ui->status_controller->setStyleSheet(back_color_none);
        ui->label_num_frames->setText("");
        ui->label_num_frames_2->setText("");
        ui->num_current_frame->setText("");
    }
}

void com_port_w::status_plc(bool status_plc)
{
    if(status_plc==true)
        ui->status_controller->setStyleSheet(back_color_on);
    else
       ui->status_controller->setStyleSheet(back_color_off);
}

void com_port_w::read_data(QByteArray recieve_data)
{
//    QString str=recieve_data;
    QString str(recieve_data.toHex());
    ui->textEdit->setText(str);
}

void com_port_w::com_port_num_res()
{
    int num=ui->comboBox->currentIndex();
    emit num_com_port(num);
}

void com_port_w::connect_to_proccommand()
{
    int num=ui->comboBox->currentIndex();
    emit num_com_proccommand(num);
}

void com_port_w::closeEvent(QCloseEvent *ev)
 {
    bool flag=false;
    emit flag_close_win(flag);
    emit disconnect();
     ev->accept();
 }
void com_port_w::error_label_main(QString error)
{
    ui->errors_label->setText(error);
}

void com_port_w::frames_label_main(int frames)
{
    int_num_frames_summ=frames;
    frames_reading.setNum(frames);
    ui->label_num_frames->setText(frames_reading);
}

void com_port_w::num_readed_frames(int num_frame)
{
    QString str;
    str.setNum(num_frame);
    ui->num_current_frame->setText(str);
    ui->label_num_frames_2->setText(frames_reading);
    ui->progressBar_frames->show();
    ui->progressBar_frames->setValue(100*num_frame/int_num_frames_summ);
}
void com_port_w::on_pushButton_clicked() // запись данных из контроллера в проект
{
    QMessageBox *import_from_plc= new QMessageBox(this);
    import_from_plc->setWindowTitle(tr("Внимание!"));
    import_from_plc->setIcon(QMessageBox::Warning);
    import_from_plc->setText(tr("<b>Данные в программе будут перезаписаны</b>"));
    import_from_plc->setInformativeText(tr("Вы уверены, что хотите перезаписать данные в проекте <b>без сохранения?</b>"));
    import_from_plc->setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Escape);
    import_from_plc->setDefaultButton(QMessageBox::No);
    import_from_plc->setButtonText(QMessageBox::Yes,tr("Да, я понимаю"));
    import_from_plc->setButtonText(QMessageBox::No,tr("Нет, пожалуй, сначала сохраню проект"));
    int n = import_from_plc->exec();
    delete import_from_plc;
    if (n == QMessageBox::Yes)
    {
        emit import_data_to_project();
    }
}

void com_port_w::on_readButton_clicked(bool checked)
{
    if(checked){
        ui->Status_PLC->setEnabled(false);
        ui->pushButton->setEnabled(false);
        ui->writeButton->setEnabled(false);
    }
    else
        ui->Status_PLC->setEnabled(true);
        ui->pushButton->setEnabled(true);
        ui->writeButton->setEnabled(true);
}
void com_port_w::on_writeButton_clicked(bool checked)
{
    if(checked){
        ui->Status_PLC->setEnabled(false);
        ui->pushButton->setEnabled(false);
        ui->readButton->setEnabled(false);
    }
    else
        ui->Status_PLC->setEnabled(true);
        ui->pushButton->setEnabled(true);
        ui->readButton->setEnabled(true);
}
