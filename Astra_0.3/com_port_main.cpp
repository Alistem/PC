#include "com_port_main.h"
#include "ui_com_port1.h"
#include <QtCore/QDebug>
#include <QFileDialog>
#include <QBuffer>
#include <QMessageBox>
#include <iostream>
#include "com_port1.h"
//#include <QApplication>
//#include <QCloseEvent>

QT_USE_NAMESPACE

com_port_w::com_port_w(QWidget *parent) :
    QWidget(parent),
  ui(new Ui::com_port)
{
    ui->setupUi(this);
    com_port *com_port1 = new com_port(this); // создаём объект класса com_port. Для реализации ком-порта
    com_port1->hide(); // скрываем объект с формы, а то он меню загораживает
    ui->progressBar_frames->hide();
    ui->connectButton->setDefault(true);

    connect(this->ui->connectButton,SIGNAL(clicked()),com_port1,SIGNAL(connect_com()));
    connect(this->ui->disconnectButton,SIGNAL(clicked()),com_port1,SIGNAL(disconnect_com()));
    connect(this->ui->writeButton,SIGNAL(clicked()),this,SLOT(data_com_port_for_post()));
    connect(this->ui->readButton,SIGNAL(clicked()),com_port1,SLOT(read_button()));
    connect(this->ui->Status_PLC,SIGNAL(clicked()),com_port1,SLOT(status_button()));
    connect(this->ui->reset,SIGNAL(clicked()),com_port1,SLOT(reset_button()));
    connect(this,SIGNAL(disconnect()),com_port1,SIGNAL(disconnect_com()));
    connect(this,SIGNAL(data_com_port_ext(QString)),com_port1,SLOT(data_com_port_post(QString)));
    connect(com_port1,SIGNAL(connect_label(QString)),this->ui->connect_color,SLOT(setText(QString)));
    connect(com_port1,SIGNAL(connect_label(QString)),this,SLOT(connect_status(QString)));
    connect(com_port1,SIGNAL(data_from_com(QByteArray)),this,SLOT(read_data(QByteArray)));
    connect(com_port1,SIGNAL(com_port_num()),this,SLOT(com_port_num_res()));
    connect(this,SIGNAL(num_com_port(int)),com_port1,SLOT(num_com_port_com(int)));
    connect(com_port1,SIGNAL(error_label(QString)),this,SLOT(error_label_main(QString))); 
    connect(com_port1,SIGNAL(status(bool)),this,SLOT(status_plc(bool)));
    connect(com_port1,SIGNAL(frames_label(int)),this,SLOT(frames_label_main(int)));
    connect(com_port1,SIGNAL(num_frame_read(int)),this,SLOT(num_readed_frames(int)));
    connect(this,SIGNAL(import_data_to_project()),com_port1,SLOT(data_to_project()));
    //===========================Данные с контроллера====================================
    connect(com_port1,SIGNAL(times_from_plc1(double,int,int)),this,SIGNAL(times_from_plc(double,int,int)));
    connect(com_port1,SIGNAL(data_from_plc1(QByteArray,int)),this,SIGNAL(data_from_plc(QByteArray,int)));
    connect(com_port1,SIGNAL(shim_from_plc1(QByteArray,int)),this,SIGNAL(shim_from_plc(QByteArray,int)));


    back_color_on="QLabel{background-color: rgb(0, 255, 0);}";
    back_color_off="QLabel{background-color: rgb(255, 0, 0);}";
    back_color_none="QLabel{background-color: rgb(255, 255, 255,0);}";

}

com_port_w::~com_port_w()
{
    delete ui;
}

//void com_port_w::on_write_line_textChanged(const QString &arg1)
//{
////    hex=arg1;
////    ui->lineEdit_2->setText(str);
////    ui->lineEdit_3->setText(QString("%1").arg(iVal));
//}
//void com_port_w::on_lineEdit_textChanged(const QString &arg1)
//{
//    QString str,str1;
//    int iVal=0;
//    bool ok;
//    str=arg1;
//    iVal = str.toInt(&ok,16);
//    str1.setNum(iVal,16);
//    hex=str;
//    hex_int=iVal;
////    ui->read_line_2->setText(str);
////    ui->lineEdit_3->setText(QString("%1").arg(iVal));
//}
//void com_port_w::on_pushButton_clicked()
//{
//    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
//                                                    tr("Animation Files (*.nmt)"/*"Text Files (*.txt);;C++ Files (*.cpp *.h)"*/));
//    QFile file(fileName);
//    if (!file.open(QIODevice::WriteOnly))
//      return;
//    else{
//        cc=QByteArray::fromHex(hex.toAscii());
//        file.write(ba);
//        file.close();
//    }
//    std::cout<<ba.data()<<std::endl;
//return;
//}
//void com_port_w::on_pushButton_2_clicked()
//{
//    unsigned char eee;
//    QByteArray ba1;
//    ba1=QByteArray::fromHex(hex.toAscii());
//    for (int i=0; i<ba1.size(); ++i){
//        eee = static_cast<unsigned char>(ba1[i]);
//        ba.append(eee);
//    }

//        std::cout<<ba1.data()<<std::endl;
////        one(ba);
//}

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

void com_port_w::data_com_port_for_post()
{
//    QString str=ui->write_line->text();
//    emit data_com_port_ext(str);
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
void com_port_w::closeEvent(QCloseEvent *ev)
 {
    bool flag=false;
    emit flag_close_win(flag);
    emit disconnect();
//     ev->accept();
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
//    ui->label_num_frames_2->setText(str);
}

void com_port_w::num_readed_frames(int num_frame)
{
    QString str;
    str.setNum(num_frame);
    ui->num_current_frame->setText(str);
    ui->label_num_frames_2->setText(frames_reading);
    ui->progressBar_frames->show();
    ui->progressBar_frames->setValue(100*num_frame/int_num_frames_summ);
//    ui->label_num_frames_2->setText(str);
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
    import_from_plc->setButtonText(QMessageBox::No,tr("Не, пожалуй, сначала сохраню проект"));
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
