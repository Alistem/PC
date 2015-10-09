﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "com_port_main.h"
#include <QDebug>
#include <QTimer>
#include <QFileDialog>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //================================Init==============================

    num_of_ch=32; // кол-во каналов (на будущее, для расширения)


    color_button=1; // кнопочки красного цвета
    stylesheet_switch(); // Меняем цвет кнопок при инициализации на красный (по-умолчанию)

    ui->textEdit->hide();
    ui->textEdit_2->hide();

    ui->lineEdit->setText("0");
    ui->lineEdit_2->setText("0");
    ui->lineEdit_3->setText("0");
    ui->lineEdit_4->setText("0");
    ui->lineEdit_5->setText("0");
    ui->lineEdit_6->setText("0");
    ui->lineEdit_7->setText("0");
    ui->lineEdit_8->setText("0");
    ui->lineEdit_9->setText("0");
    ui->lineEdit_10->setText("0");
    ui->lineEdit_11->setText("0");
    ui->lineEdit_12->setText("0");
    ui->lineEdit_13->setText("0");
    ui->lineEdit_14->setText("0");
    ui->lineEdit_15->setText("0");
    ui->lineEdit_16->setText("0");
    ui->lineEdit_17->setText("0");
    ui->lineEdit_18->setText("0");
    ui->lineEdit_19->setText("0");
    ui->lineEdit_20->setText("0");
    ui->lineEdit_21->setText("0");
    ui->lineEdit_22->setText("0");
    ui->lineEdit_23->setText("0");
    ui->lineEdit_24->setText("0");
    ui->lineEdit_25->setText("0");
    ui->lineEdit_26->setText("0");
    ui->lineEdit_27->setText("0");
    ui->lineEdit_28->setText("0");
    ui->lineEdit_29->setText("0");
    ui->lineEdit_30->setText("0");
    ui->lineEdit_31->setText("0");
    ui->lineEdit_32->setText("0");
    ui->lineEdit_time->setText("0.01");
//=====================================================================
    ui->action8->triggered(true); // Один контроллер (при инициализации)
    ui->action8->setChecked(true);// Один контроллер (при инициализации)
    //====================================================================

    back_color="QPushButton{border-image: url(:/bkg); background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.63, fx:0.5, fy:0.5, stop:0.225989 rgba(255, 40, 200, 255), stop:0.463277 rgba(220, 42, 100, 255), stop:0.564246 rgba(130, 0, 0, 255), stop:0.672 rgba(75, 0, 0, 255), stop:0.701 rgba(255, 255, 255, 0));}";
    btn_off="QPushButton{border-image: url(:/bkg); background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.63, fx:0.5, fy:0.5, stop:0.379 rgba(180, 40, 10, 255), stop:0.52 rgba(160, 42, 0, 255), stop:0.633 rgba(130, 0, 0, 255), stop:0.672 rgba(100, 1, 1, 255), stop:0.7 rgba(255, 255, 255, 0));}";
    on_gradient_one=255; //заливка кнопочек. красивая, в четыре градиента
    on_gradient_two=220;
    on_gradient_three=130;
    on_gradient_four=75;
    //==================
    off_gradient_one=140;
    off_gradient_two=110;
    off_gradient_three=70;
    off_gradient_four=40;

    num_frame=1;
    num_sum=1;
    new_frame(); 
    frame_num_lcd();
    frame_sum_lcd();
    frame=frames_list[num_frame-1];
    all_frames=0;
    entr_bright=0;
    repeat=0;
    anim_stop=0;
    anim_pause=0; // стоп анимации ставим в ноль
    anim_start=0;
    block_anim=0;
    com_port_window=false; // при инициализации задаём флаг окна передачи данных в ноль. Окно не создано

    timer = new QTimer(this); // таймер для анимации
    connect(timer, SIGNAL(timeout()), SLOT(animation_body())); // коннект для таймера анимации
    connect(ui->action_save_animation,SIGNAL(triggered()),SLOT(save_animation())); // сохранение анимации
    connect(ui->action_open_animation,SIGNAL(triggered()),SLOT(open_animation())); // открытие анимации

}

//========================================================================================================


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::stylesheet_switch()
{
    init_style_red="QPushButton{border-image: url(:/led_my_super); background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.63, fx:0.5, fy:0.5, stop:0.225989 rgba(120, 0, 0, 255), stop:0.463277 rgba(100, 0, 0, 255), stop:0.564246 rgba(70, 0, 0, 255), stop:0.672 rgba(40, 0, 0, 255), stop:0.7 rgba(255, 255, 255, 0));}";
    init_style_green="QPushButton{border-image: url(:/led_my_super); background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.63, fx:0.5, fy:0.5, stop:0.225989 rgba(0, 120, 0, 255), stop:0.463277 rgba(0, 100, 0, 255), stop:0.564246 rgba(0, 70, 0, 255), stop:0.672 rgba(0, 40, 0, 255), stop:0.7 rgba(255, 255, 255, 0));}";
    init_style_blue="QPushButton{border-image: url(:/led_my_super); background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.63, fx:0.5, fy:0.5, stop:0.225989 rgba(0, 50, 120, 255), stop:0.463277 rgba(0, 50, 100, 255), stop:0.564246 rgba(0, 50, 70, 255), stop:0.672 rgba(0, 50, 40, 255), stop:0.7 rgba(255, 255, 255, 0));}";
    if (color_button==1){
        init_style=init_style_red;
    }
    else{
        if (color_button==2){
            init_style=init_style_green;
        }
        else{
            if (color_button==3){
                init_style=init_style_blue;
            }
        }
    }
    ui->pushButton_1->setStyleSheet(init_style);
    ui->pushButton_2->setStyleSheet(init_style);
    ui->pushButton_3->setStyleSheet(init_style);
    ui->pushButton_4->setStyleSheet(init_style);
    ui->pushButton_5->setStyleSheet(init_style);
    ui->pushButton_6->setStyleSheet(init_style);
    ui->pushButton_7->setStyleSheet(init_style);
    ui->pushButton_8->setStyleSheet(init_style);
    ui->pushButton_9->setStyleSheet(init_style);
    ui->pushButton_10->setStyleSheet(init_style);
    ui->pushButton_11->setStyleSheet(init_style);
    ui->pushButton_12->setStyleSheet(init_style);
    ui->pushButton_13->setStyleSheet(init_style);
    ui->pushButton_14->setStyleSheet(init_style);
    ui->pushButton_15->setStyleSheet(init_style);
    ui->pushButton_16->setStyleSheet(init_style);
    ui->pushButton_17->setStyleSheet(init_style);
    ui->pushButton_18->setStyleSheet(init_style);
    ui->pushButton_19->setStyleSheet(init_style);
    ui->pushButton_20->setStyleSheet(init_style);
    ui->pushButton_21->setStyleSheet(init_style);
    ui->pushButton_22->setStyleSheet(init_style);
    ui->pushButton_23->setStyleSheet(init_style);
    ui->pushButton_24->setStyleSheet(init_style);
    ui->pushButton_25->setStyleSheet(init_style);
    ui->pushButton_26->setStyleSheet(init_style);
    ui->pushButton_27->setStyleSheet(init_style);
    ui->pushButton_28->setStyleSheet(init_style);
    ui->pushButton_29->setStyleSheet(init_style);
    ui->pushButton_30->setStyleSheet(init_style);
    ui->pushButton_31->setStyleSheet(init_style);
    ui->pushButton_32->setStyleSheet(init_style);
}

void MainWindow::on_action_red_triggered()
{
    color_button=1;
    stylesheet_switch();
}
void MainWindow::on_action_green_triggered()
{
    color_button=2;
    stylesheet_switch();
}
void MainWindow::on_action_blue_triggered()
{
    color_button=3;
    stylesheet_switch();
}

void MainWindow::change_bright_btn(int bright) // слот для изменения яркости элементов
{
    double value_bright=bright/255.0;
    //============= первый градиент ==========
    double grad_one=((on_gradient_one-off_gradient_one)*value_bright)+120;
    QString grad_one_str(QString::number(grad_one));
    //============= второй градиент ==========
    double grad_two=((on_gradient_two-off_gradient_two)*value_bright)+100;
    QString grad_two_str(QString::number(grad_two));
    //============= третий градиент ==========
    double grad_three=((on_gradient_three-off_gradient_three)*value_bright)+70;
    QString grad_three_str(QString::number(grad_three));
    //============= четвёртый градиент =======
    double grad_four=((on_gradient_four-off_gradient_four)*value_bright)+40;
    QString grad_four_str(QString::number(grad_four));
    //========================================
    back_color_red="QPushButton{border-image: url(:/led_my_super);}""QPushButton{background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.63, fx:0.5, fy:0.5, stop:0.225989 rgba("+grad_one_str+", 10, 0, 255), stop:0.463277 rgba("+grad_two_str+", 42, 22, 255), stop:0.564246 rgba("+grad_three_str+", 16, 16, 255), stop:0.672 rgba("+grad_four_str+", 1, 1, 255), stop:0.701 rgba(255, 255, 255, 0));}";
    back_color_green="QPushButton{border-image: url(:/led_my_super);}""QPushButton{background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.63, fx:0.5, fy:0.5, stop:0.225989 rgba(10,"+grad_one_str+", 0, 255), stop:0.463277 rgba(42,"+grad_two_str+", 22, 255), stop:0.564246 rgba(16,"+grad_three_str+", 16, 255), stop:0.672 rgba(1,"+grad_four_str+", 1, 255), stop:0.701 rgba(255, 255, 255, 0));}";
    back_color_blue="QPushButton{border-image: url(:/led_my_super);}""QPushButton{background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.63, fx:0.5, fy:0.5, stop:0.225989 rgba(0, 50,"+grad_one_str+",  255), stop:0.463277 rgba(0, 50,"+grad_two_str+", 255), stop:0.564246 rgba(0, 50,"+grad_three_str+", 255), stop:0.672 rgba(0, 50,"+grad_four_str+", 255), stop:0.701 rgba(255, 255, 255, 0));}";

    if (color_button==1){
        back_color=back_color_red;
    }
    else{
        if (color_button==2){
            back_color=back_color_green;
        }
        else{
            if (color_button==3){
                back_color=back_color_blue;
            }
        }
    }
//    back_color="QPushButton{border-image: url(:/led_my_super);}""QPushButton{background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.63, fx:0.5, fy:0.5, stop:0.225989 rgba("+grad_one_str+", 72, 0, 255), stop:0.463277 rgba("+grad_two_str+", 42, 22, 255), stop:0.564246 rgba("+grad_three_str+", 16, 16, 255), stop:0.672 rgba("+grad_four_str+", 1, 1, 255), stop:0.701 rgba(255, 255, 255, 0));}";

    if(ch_num==1)
        ui->pushButton_1->setStyleSheet(back_color);
    if(ch_num==2)
        ui->pushButton_2->setStyleSheet((back_color));
    if(ch_num==3)
        ui->pushButton_3->setStyleSheet((back_color));
    if(ch_num==4)
        ui->pushButton_4->setStyleSheet((back_color));
    if(ch_num==5)
        ui->pushButton_5->setStyleSheet((back_color));
    if(ch_num==6)
        ui->pushButton_6->setStyleSheet((back_color));
    if(ch_num==7)
        ui->pushButton_7->setStyleSheet((back_color));
    if(ch_num==8)
        ui->pushButton_8->setStyleSheet((back_color));
    if(ch_num==9)
        ui->pushButton_9->setStyleSheet((back_color));
    if(ch_num==10)
        ui->pushButton_10->setStyleSheet((back_color));
    if(ch_num==11)
        ui->pushButton_11->setStyleSheet((back_color));
    if(ch_num==12)
        ui->pushButton_12->setStyleSheet((back_color));
    if(ch_num==13)
        ui->pushButton_13->setStyleSheet((back_color));
    if(ch_num==14)
        ui->pushButton_14->setStyleSheet((back_color));
    if(ch_num==15)
        ui->pushButton_15->setStyleSheet((back_color));
    if(ch_num==16)
        ui->pushButton_16->setStyleSheet((back_color));
    if(ch_num==17)
        ui->pushButton_17->setStyleSheet((back_color));
    if(ch_num==18)
        ui->pushButton_18->setStyleSheet((back_color));
    if(ch_num==19)
        ui->pushButton_19->setStyleSheet((back_color));
    if(ch_num==20)
        ui->pushButton_20->setStyleSheet((back_color));
    if(ch_num==21)
        ui->pushButton_21->setStyleSheet((back_color));
    if(ch_num==22)
        ui->pushButton_22->setStyleSheet((back_color));
    if(ch_num==23)
        ui->pushButton_23->setStyleSheet((back_color));
    if(ch_num==24)
        ui->pushButton_24->setStyleSheet((back_color));
    if(ch_num==25)
        ui->pushButton_25->setStyleSheet((back_color));
    if(ch_num==26)
        ui->pushButton_26->setStyleSheet((back_color));
    if(ch_num==27)
        ui->pushButton_27->setStyleSheet((back_color));
    if(ch_num==28)
        ui->pushButton_28->setStyleSheet((back_color));
    if(ch_num==29)
        ui->pushButton_29->setStyleSheet((back_color));
    if(ch_num==30)
        ui->pushButton_30->setStyleSheet((back_color));
    if(ch_num==31)
        ui->pushButton_31->setStyleSheet((back_color));
    if(ch_num==32)
        ui->pushButton_32->setStyleSheet((back_color));
}

void MainWindow::on_lineEdit_editingFinished()
{
//    if(ui->lineEdit->text().toInt()>255)
//        ui->lineEdit->setStyleSheet("QLineEdit{background-color:rgb(255, 170, 127);alternate-background-color:rgb(255, 170, 127)}");
//    else
//        ui->lineEdit->setStyleSheet("QLineEdit{background-color:rgb(255, 255, 255)}");

    if(entr_bright==1 || ui->lineEdit->text().toInt()>255){
        entr_bright=0;
               return;
    }

    int bright=ui->lineEdit->text().toInt();
    entr_bright=1;
    ui->verticalSlider->setValue(bright);
    return;

}
void MainWindow::on_lineEdit_2_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_2->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_2->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_2->setValue(b);
    return;
}
void MainWindow::on_lineEdit_3_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_3->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_3->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_3->setValue(b);
    return;
}
void MainWindow::on_lineEdit_4_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_4->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_4->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_4->setValue(b);
    return;
}
void MainWindow::on_lineEdit_5_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_5->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_5->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_5->setValue(b);
    return;
}
void MainWindow::on_lineEdit_6_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_6->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_6->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_6->setValue(b);
    return;
}
void MainWindow::on_lineEdit_7_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_7->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_7->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_7->setValue(b);
    return;
}
void MainWindow::on_lineEdit_8_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_8->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_8->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_8->setValue(b);
    return;
}
void MainWindow::on_lineEdit_9_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_9->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_9->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_9->setValue(b);
    return;
}
void MainWindow::on_lineEdit_10_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_10->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_10->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_10->setValue(b);
    return;
}
void MainWindow::on_lineEdit_11_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_11->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_11->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_11->setValue(b);
    return;
}
void MainWindow::on_lineEdit_12_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_12->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_12->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_12->setValue(b);
    return;
}
void MainWindow::on_lineEdit_13_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_13->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_13->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_13->setValue(b);
    return;
}
void MainWindow::on_lineEdit_14_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_14->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_14->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_14->setValue(b);
    return;
}
void MainWindow::on_lineEdit_15_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_15->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_15->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_15->setValue(b);
    return;
}
void MainWindow::on_lineEdit_16_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_16->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_16->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_16->setValue(b);
    return;
}
void MainWindow::on_lineEdit_17_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_17->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_17->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_17->setValue(b);
    return;
}
void MainWindow::on_lineEdit_18_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_18->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_18->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_18->setValue(b);
    return;
}
void MainWindow::on_lineEdit_19_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_19->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_19->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_19->setValue(b);
    return;
}
void MainWindow::on_lineEdit_20_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_20->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_20->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_20->setValue(b);
    return;
}
void MainWindow::on_lineEdit_21_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_21->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_21->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_21->setValue(b);
    return;
}
void MainWindow::on_lineEdit_22_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_22->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_22->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_22->setValue(b);
    return;
}
void MainWindow::on_lineEdit_23_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_23->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_23->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_23->setValue(b);
    return;
}
void MainWindow::on_lineEdit_24_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_24->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_24->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_24->setValue(b);
    return;
}
void MainWindow::on_lineEdit_25_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_25->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_25->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_25->setValue(b);
    return;
}
void MainWindow::on_lineEdit_26_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_26->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_26->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_26->setValue(b);
    return;
}
void MainWindow::on_lineEdit_27_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_27->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_27->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_27->setValue(b);
    return;
}
void MainWindow::on_lineEdit_28_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_28->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_28->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_28->setValue(b);
    return;
}
void MainWindow::on_lineEdit_29_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_29->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_29->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_29->setValue(b);
    return;
}
void MainWindow::on_lineEdit_30_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_30->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_30->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_30->setValue(b);
    return;
}
void MainWindow::on_lineEdit_31_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_31->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_31->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_31->setValue(b);
    return;
}
void MainWindow::on_lineEdit_32_editingFinished()
{
    if(entr_bright==1 || ui->lineEdit_32->text().toInt()>255){
        entr_bright=0;
        return;
    }
    int bright=ui->lineEdit_32->text().toInt();
    int b=bright;
    entr_bright=1;
    ui->verticalSlider_32->setValue(b);
    return;
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    ch_num=1;
    change_bright_btn(value);
//    QString str=QString("%1").arg(value);
//    ui->textBrowser->setText(str);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit->setText(bright);
    }
    else{
        ui->verticalSlider->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_2_valueChanged(int value)
{
    ch_num=2;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_2->setText(bright);
    }
    else{
        ui->verticalSlider_2->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_3_valueChanged(int value)
{
    ch_num=3;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_3->setText(bright);
    }
    else{
        ui->verticalSlider_3->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_4_valueChanged(int value)
{
    ch_num=4;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_4->setText(bright);
    }
    else{
        ui->verticalSlider_4->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_5_valueChanged(int value)
{
    ch_num=5;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_5->setText(bright);
    }
    else{
        ui->verticalSlider_5->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_6_valueChanged(int value)
{
    ch_num=6;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_6->setText(bright);
    }
    else{
        ui->verticalSlider_6->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_7_valueChanged(int value)
{
    ch_num=7;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_7->setText(bright);
    }
    else{
        ui->verticalSlider_7->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_8_valueChanged(int value)
{
    ch_num=8;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_8->setText(bright);
    }
    else{
        ui->verticalSlider_8->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_9_valueChanged(int value)
{
    ch_num=9;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_9->setText(bright);
    }
    else{
        ui->verticalSlider_9->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_10_valueChanged(int value)
{
    ch_num=10;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_10->setText(bright);
    }
    else{
        ui->verticalSlider_10->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_11_valueChanged(int value)
{
    ch_num=11;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_11->setText(bright);
    }
    else{
        ui->verticalSlider_11->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_12_valueChanged(int value)
{
    ch_num=12;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_12->setText(bright);
    }
    else{
        ui->verticalSlider_12->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_13_valueChanged(int value)
{
    ch_num=13;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_13->setText(bright);
    }
    else{
        ui->verticalSlider_13->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_14_valueChanged(int value)
{
    ch_num=14;
   change_bright_btn(value);
   massive_frame(value);

   int gbg=value;
   QString bright=QString::number(gbg);
   if(entr_bright==0){
       ui->lineEdit_14->setText(bright);
   }
   else{
       ui->verticalSlider_14->setFocus();
       entr_bright=0;
   }
}
void MainWindow::on_verticalSlider_15_valueChanged(int value)
{
    ch_num=15;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_15->setText(bright);
    }
    else{
        ui->verticalSlider_15->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_16_valueChanged(int value)
{
    ch_num=16;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_16->setText(bright);
    }
    else{
        ui->verticalSlider_16->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_17_valueChanged(int value)
{
    ch_num=17;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_17->setText(bright);
    }
    else{
        ui->verticalSlider_17->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_18_valueChanged(int value)
{
    ch_num=18;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_18->setText(bright);
    }
    else{
        ui->verticalSlider_18->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_19_valueChanged(int value)
{
    ch_num=19;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_19->setText(bright);
    }
    else{
        ui->verticalSlider_19->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_20_valueChanged(int value)
{
    ch_num=20;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_20->setText(bright);
    }
    else{
        ui->verticalSlider_20->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_21_valueChanged(int value)
{
    ch_num=21;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_21->setText(bright);
    }
    else{
        ui->verticalSlider_21->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_22_valueChanged(int value)
{
    ch_num=22;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_22->setText(bright);
    }
    else{
        ui->verticalSlider_22->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_23_valueChanged(int value)
{
    ch_num=23;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_23->setText(bright);
    }
    else{
        ui->verticalSlider_23->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_24_valueChanged(int value)
{
    ch_num=24;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_24->setText(bright);
    }
    else{
        ui->verticalSlider_24->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_25_valueChanged(int value)
{
    ch_num=25;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_25->setText(bright);
    }
    else{
        ui->verticalSlider_25->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_26_valueChanged(int value)
{
    ch_num=26;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_26->setText(bright);
    }
    else{
        ui->verticalSlider_26->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_27_valueChanged(int value)
{
    ch_num=27;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_27->setText(bright);
    }
    else{
        ui->verticalSlider_27->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_28_valueChanged(int value)
{
    ch_num=28;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_28->setText(bright);
    }
    else{
        ui->verticalSlider_28->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_29_valueChanged(int value)
{
    ch_num=29;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_29->setText(bright);
    }
    else{
        ui->verticalSlider_29->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_30_valueChanged(int value)
{
    ch_num=30;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_30->setText(bright);
    }
    else{
        ui->verticalSlider_30->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_31_valueChanged(int value)
{
    ch_num=31;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_31->setText(bright);
    }
    else{
        ui->verticalSlider_31->setFocus();
        entr_bright=0;
    }
}
void MainWindow::on_verticalSlider_32_valueChanged(int value)
{
    ch_num=32;
    change_bright_btn(value);
    massive_frame(value);

    int gbg=value;
    QString bright=QString::number(gbg);
    if(entr_bright==0){
        ui->lineEdit_32->setText(bright);
    }
    else{
        ui->verticalSlider_32->setFocus();
        entr_bright=0;
    }
}

void MainWindow::on_pushButton_1_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit->setText("0");
    }
    ui->lineEdit->editingFinished();
}
void MainWindow::on_pushButton_2_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_2->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_2->setText("0");
    }
    ui->lineEdit_2->editingFinished();
}
void MainWindow::on_pushButton_3_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_3->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_3->setText("0");
    }
    ui->lineEdit_3->editingFinished();
}
void MainWindow::on_pushButton_4_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_4->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_4->setText("0");
    }
    ui->lineEdit_4->editingFinished();
}
void MainWindow::on_pushButton_5_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_5->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_5->setText("0");
    }
    ui->lineEdit_5->editingFinished();
}
void MainWindow::on_pushButton_6_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_6->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_6->setText("0");
    }
    ui->lineEdit_6->editingFinished();
}
void MainWindow::on_pushButton_7_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_7->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_7->setText("0");
    }
    ui->lineEdit_7->editingFinished();
}
void MainWindow::on_pushButton_8_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_8->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_8->setText("0");
    }
    ui->lineEdit_8->editingFinished();
}
void MainWindow::on_pushButton_9_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_9->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_9->setText("0");
    }
    ui->lineEdit_9->editingFinished();
}
void MainWindow::on_pushButton_10_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_10->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_10->setText("0");
    }
    ui->lineEdit_10->editingFinished();
}
void MainWindow::on_pushButton_11_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_11->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_11->setText("0");
    }
    ui->lineEdit_11->editingFinished();
}
void MainWindow::on_pushButton_12_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_12->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_12->setText("0");
    }
    ui->lineEdit_12->editingFinished();
}
void MainWindow::on_pushButton_13_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_13->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_13->setText("0");
    }
    ui->lineEdit_13->editingFinished();
}
void MainWindow::on_pushButton_14_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_14->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_14->setText("0");
    }
    ui->lineEdit_14->editingFinished();
}
void MainWindow::on_pushButton_15_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_15->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_15->setText("0");
    }
    ui->lineEdit_15->editingFinished();
}
void MainWindow::on_pushButton_16_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_16->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_16->setText("0");
    }
    ui->lineEdit_16->editingFinished();
}
void MainWindow::on_pushButton_17_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_17->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_17->setText("0");
    }
    ui->lineEdit_17->editingFinished();
}
void MainWindow::on_pushButton_18_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_18->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_18->setText("0");
    }
    ui->lineEdit_18->editingFinished();
}
void MainWindow::on_pushButton_19_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_19->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_19->setText("0");
    }
    ui->lineEdit_19->editingFinished();
}
void MainWindow::on_pushButton_20_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_20->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_20->setText("0");
    }
    ui->lineEdit_20->editingFinished();
}
void MainWindow::on_pushButton_21_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_21->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_21->setText("0");
    }
    ui->lineEdit_21->editingFinished();
}
void MainWindow::on_pushButton_22_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_22->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_22->setText("0");
    }
    ui->lineEdit_22->editingFinished();
}
void MainWindow::on_pushButton_23_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_23->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_23->setText("0");
    }
    ui->lineEdit_23->editingFinished();
}
void MainWindow::on_pushButton_24_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_24->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_24->setText("0");
    }
    ui->lineEdit_24->editingFinished();
}
void MainWindow::on_pushButton_25_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_25->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_25->setText("0");
    }
    ui->lineEdit_25->editingFinished();
}
void MainWindow::on_pushButton_26_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_26->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_26->setText("0");
    }
    ui->lineEdit_26->editingFinished();
}
void MainWindow::on_pushButton_27_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_27->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_27->setText("0");
    }
    ui->lineEdit_27->editingFinished();
}
void MainWindow::on_pushButton_28_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_28->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_28->setText("0");
    }
    ui->lineEdit_28->editingFinished();
}
void MainWindow::on_pushButton_29_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_29->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_29->setText("0");
    }
    ui->lineEdit_29->editingFinished();
}
void MainWindow::on_pushButton_30_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_30->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_30->setText("0");
    }
    ui->lineEdit_30->editingFinished();
}
void MainWindow::on_pushButton_31_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_31->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_31->setText("0");
    }
    ui->lineEdit_31->editingFinished();
}
void MainWindow::on_pushButton_32_clicked(bool checked)
{
    if(checked==1){
        entr_bright=0;
        ui->lineEdit_32->setText("255");
    }
    else{
        entr_bright=0;
        ui->lineEdit_32->setText("0");
    }
    ui->lineEdit_32->editingFinished();
}



//=====================================Frames_massive_control=================================

void MainWindow::massive_frame(int value)
{
    frame[ch_num-1]=value; // массив с данными текущего кадра (по всем каналам)
    frame_text(num_frame,frame);// вывод текущего кадра в TextEdit
}


void MainWindow::frame_text(int value, QByteArray fra) // Текущий кадр в 16-тиричном формате (In the textEdit)
{
    QByteArray ba;
    ba[0]=value;
    QString str("0000"),str1,str2;
    str1=ba.toHex();
    str.replace((str.size()-str1.size()),str1.size(),str1);// номер кадра в 16-тиричном формате
    QString buff;
    buff=fra.toHex();
    for(int i=0;i<num_of_ch*2;i+=2){
        str2+="0x"+buff.mid(i,2);
        if(i<num_of_ch*2-3)
            str2+=",";
        else{
            str2+=";";
            str2+=" ";
            str2+=str;
            str2+="\r";
        }
    }

    frame_string=str2;
    if(all_frames==0)  // Условие, что не работает слот frame_text_all()
    ui->textEdit->setText(frame_string);
}
void MainWindow::frame_text_all() // Все кадры в 16-тиричном формате (In the textEdit_2)
{
    QString sum;
    all_frames=1; // Условие, для работы слота frame_text (для вычисления кадра) этого слота
    for(int i=0;i<frames_list.size();++i){
        QByteArray buff=frames_list.at(i);
        frame_text(i+1,buff);
        sum+=frame_string;
    }
    frames_string_sum=sum;
    ui->textEdit_2->setText(sum);
    all_frames=0;
}

void MainWindow::new_frame() // пишем новый кадр в мап
{
    QByteArray buff;
    double time_buff;
    buff.fill(0,num_of_ch);
    time_buff=0.01;
    frames_list.append(buff);
    frames_time.append(time_buff);
    frame_text(num_frame-1,buff);
    frame_text_all();
}
void MainWindow::add_frame(int num) // вставляем новый кадр в мапе (врезаем посреди других кадров)
{
    QByteArray buff;
    double time_buff;
    buff.fill(0,num_of_ch);
    time_buff=0.01;
    frames_list.insert((num-1),buff);
    frames_time.insert((num-1),time_buff);
}
void MainWindow::del_frame(int num) // удаляем кадр
{
    frames_list.removeAt(num-1); // ШИМ
    frames_time.removeAt(num-1); // Время
    if(frames_list.size()==0)
        new_frame();

}

void MainWindow::frames_to_map(int num, QByteArray fr_data) // пишем кадр в мап
{
    frames_list[num-1]=fr_data;
}

void MainWindow::rev_ret_time() // обратная связь для времени
{
    time_current=frames_time[num_frame-1]*100;
    int num=time_current;
    entr_tme=0;
    ui->slider_time->setValue(num);
}

void MainWindow::rev_ret() // обратная связь
{
    frame=frames_list.at(num_frame-1);
    for(int i=0;i<frame.size();++i){
        QByteArray ba=frame.mid(i,1);
        int k=ba.toHex().toInt(0,16);
        if(i==0)
            ui->verticalSlider->setValue(k);
        if(i==1)
            ui->verticalSlider_2->setValue(k);
        if(i==2)
            ui->verticalSlider_3->setValue(k);
        if(i==3)
            ui->verticalSlider_4->setValue(k);
        if(i==4)
            ui->verticalSlider_5->setValue(k);
        if(i==5)
            ui->verticalSlider_6->setValue(k);
        if(i==6)
            ui->verticalSlider_7->setValue(k);
        if(i==7)
            ui->verticalSlider_8->setValue(k);
        if(i==8)
            ui->verticalSlider_9->setValue(k);
        if(i==9)
            ui->verticalSlider_10->setValue(k);
        if(i==10)
            ui->verticalSlider_11->setValue(k);
        if(i==11)
            ui->verticalSlider_12->setValue(k);
        if(i==12)
            ui->verticalSlider_13->setValue(k);
        if(i==13)
            ui->verticalSlider_14->setValue(k);
        if(i==14)
            ui->verticalSlider_15->setValue(k);
        if(i==15)
            ui->verticalSlider_16->setValue(k);
        if(i==16)
            ui->verticalSlider_17->setValue(k);
        if(i==17)
            ui->verticalSlider_18->setValue(k);
        if(i==18)
            ui->verticalSlider_19->setValue(k);
        if(i==19)
            ui->verticalSlider_20->setValue(k);
        if(i==20)
            ui->verticalSlider_21->setValue(k);
        if(i==21)
            ui->verticalSlider_22->setValue(k);
        if(i==22)
            ui->verticalSlider_23->setValue(k);
        if(i==23)
            ui->verticalSlider_24->setValue(k);
        if(i==24)
            ui->verticalSlider_25->setValue(k);
        if(i==25)
            ui->verticalSlider_26->setValue(k);
        if(i==26)
            ui->verticalSlider_27->setValue(k);
        if(i==27)
            ui->verticalSlider_28->setValue(k);
        if(i==28)
            ui->verticalSlider_29->setValue(k);
        if(i==29)
            ui->verticalSlider_30->setValue(k);
        if(i==30)
            ui->verticalSlider_31->setValue(k);
        if(i==31)
            ui->verticalSlider_32->setValue(k);
    }
}

void MainWindow::frame_num_lcd()
{
    ui->num_frame_lcd->display(num_frame);
}
void MainWindow::frame_sum_lcd()
{
    num_sum=frames_list.size();
    ui->lcd_sum->display(num_sum);
}

//===================================Channels_numbers=================================
void MainWindow::on_action8_triggered(bool checked)
{
    num_channels=8;
    if(checked==true){
    ui->action16->setChecked(false);
    ui->action24->setChecked(false);
    ui->action32->setChecked(false);
//============================================
    ui->verticalSlider_9->setEnabled(false);
    ui->verticalSlider_10->setEnabled(false);
    ui->verticalSlider_11->setEnabled(false);
    ui->verticalSlider_12->setEnabled(false);
    ui->verticalSlider_13->setEnabled(false);
    ui->verticalSlider_14->setEnabled(false);
    ui->verticalSlider_15->setEnabled(false);
    ui->verticalSlider_16->setEnabled(false);
    ui->verticalSlider_17->setEnabled(false);
    ui->verticalSlider_18->setEnabled(false);
    ui->verticalSlider_19->setEnabled(false);
    ui->verticalSlider_20->setEnabled(false);
    ui->verticalSlider_21->setEnabled(false);
    ui->verticalSlider_22->setEnabled(false);
    ui->verticalSlider_23->setEnabled(false);
    ui->verticalSlider_24->setEnabled(false);
    ui->verticalSlider_25->setEnabled(false);
    ui->verticalSlider_26->setEnabled(false);
    ui->verticalSlider_27->setEnabled(false);
    ui->verticalSlider_28->setEnabled(false);
    ui->verticalSlider_29->setEnabled(false);
    ui->verticalSlider_30->setEnabled(false);
    ui->verticalSlider_31->setEnabled(false);
    ui->verticalSlider_32->setEnabled(false);
    ui->groupBox_3->hide();
    ui->groupBox_4->hide();
    ui->groupBox_5->hide();
    }
    else{
        ui->action8->triggered(true);
        ui->action8->setChecked(true);
    }
}
void MainWindow::on_action16_triggered(bool checked)
{
    num_channels=16;
    if(checked==true){
    ui->action8->setChecked(false);
    ui->action24->setChecked(false);
    ui->action32->setChecked(false);
    //============================================
    ui->verticalSlider_9->setEnabled(true);
    ui->verticalSlider_10->setEnabled(true);
    ui->verticalSlider_11->setEnabled(true);
    ui->verticalSlider_12->setEnabled(true);
    ui->verticalSlider_13->setEnabled(true);
    ui->verticalSlider_14->setEnabled(true);
    ui->verticalSlider_15->setEnabled(true);
    ui->verticalSlider_16->setEnabled(true);
    //----------------------------------------
    ui->verticalSlider_17->setEnabled(false);
    ui->verticalSlider_18->setEnabled(false);
    ui->verticalSlider_19->setEnabled(false);
    ui->verticalSlider_20->setEnabled(false);
    ui->verticalSlider_21->setEnabled(false);
    ui->verticalSlider_22->setEnabled(false);
    ui->verticalSlider_23->setEnabled(false);
    ui->verticalSlider_24->setEnabled(false);
    ui->verticalSlider_25->setEnabled(false);
    ui->verticalSlider_26->setEnabled(false);
    ui->verticalSlider_27->setEnabled(false);
    ui->verticalSlider_28->setEnabled(false);
    ui->verticalSlider_29->setEnabled(false);
    ui->verticalSlider_30->setEnabled(false);
    ui->verticalSlider_31->setEnabled(false);
    ui->verticalSlider_32->setEnabled(false);
    //=============================================
    ui->groupBox_3->show();
    //====================
    ui->groupBox_4->hide();
    ui->groupBox_5->hide();
    }
    else{
        ui->action16->triggered(true);
        ui->action16->setChecked(true);
    }
}
void MainWindow::on_action24_triggered(bool checked)
{
    num_channels=24;
    if(checked==true){
    ui->action8->setChecked(false);
    ui->action16->setChecked(false);
    ui->action32->setChecked(false);
    //============================================
    ui->verticalSlider_9->setEnabled(true);
    ui->verticalSlider_10->setEnabled(true);
    ui->verticalSlider_11->setEnabled(true);
    ui->verticalSlider_12->setEnabled(true);
    ui->verticalSlider_13->setEnabled(true);
    ui->verticalSlider_14->setEnabled(true);
    ui->verticalSlider_15->setEnabled(true);
    ui->verticalSlider_16->setEnabled(true);
    ui->verticalSlider_17->setEnabled(true);
    ui->verticalSlider_18->setEnabled(true);
    ui->verticalSlider_19->setEnabled(true);
    ui->verticalSlider_20->setEnabled(true);
    ui->verticalSlider_21->setEnabled(true);
    ui->verticalSlider_22->setEnabled(true);
    ui->verticalSlider_23->setEnabled(true);
    ui->verticalSlider_24->setEnabled(true);
    //=-----------=-============================---
    ui->verticalSlider_25->setEnabled(false);
    ui->verticalSlider_26->setEnabled(false);
    ui->verticalSlider_27->setEnabled(false);
    ui->verticalSlider_28->setEnabled(false);
    ui->verticalSlider_29->setEnabled(false);
    ui->verticalSlider_30->setEnabled(false);
    ui->verticalSlider_31->setEnabled(false);
    ui->verticalSlider_32->setEnabled(false);
    //=============================================
    ui->groupBox_3->show();
    ui->groupBox_4->show();
    //====================
    ui->groupBox_5->hide();
    }
    else{
        ui->action24->triggered(true);
        ui->action24->setChecked(true);
    }
}
void MainWindow::on_action32_triggered(bool checked)
{
    num_channels=32;
    if(checked==true){
    ui->action8->setChecked(false);
    ui->action16->setChecked(false);
    ui->action24->setChecked(false);
    //============================================
    ui->verticalSlider_9->setEnabled(true);
    ui->verticalSlider_10->setEnabled(true);
    ui->verticalSlider_11->setEnabled(true);
    ui->verticalSlider_12->setEnabled(true);
    ui->verticalSlider_13->setEnabled(true);
    ui->verticalSlider_14->setEnabled(true);
    ui->verticalSlider_15->setEnabled(true);
    ui->verticalSlider_16->setEnabled(true);
    ui->verticalSlider_17->setEnabled(true);
    ui->verticalSlider_18->setEnabled(true);
    ui->verticalSlider_19->setEnabled(true);
    ui->verticalSlider_20->setEnabled(true);
    ui->verticalSlider_21->setEnabled(true);
    ui->verticalSlider_22->setEnabled(true);
    ui->verticalSlider_23->setEnabled(true);
    ui->verticalSlider_24->setEnabled(true);
    ui->verticalSlider_25->setEnabled(true);
    ui->verticalSlider_26->setEnabled(true);
    ui->verticalSlider_27->setEnabled(true);
    ui->verticalSlider_28->setEnabled(true);
    ui->verticalSlider_29->setEnabled(true);
    ui->verticalSlider_30->setEnabled(true);
    ui->verticalSlider_31->setEnabled(true);
    ui->verticalSlider_32->setEnabled(true);
    //=============================================
    ui->groupBox_3->show();
    ui->groupBox_4->show();
    ui->groupBox_5->show();
    }
    else{
        ui->action32->triggered(true);
        ui->action32->setChecked(true);
    }
}

//====================================Animation_Control===============================


void MainWindow::on_pushButton_111_clicked() // Create Frame
{
    frames_to_map(num_frame,frame); //запись данных кадра в мап    
    num_frame+=1;
    if(num_frame<num_sum)
        add_frame(num_frame); // вставка нового кадра
    else
        new_frame(); // добавление нового кадра в конец списка
    frame_text_all();
    frame_num_lcd();
    frame_sum_lcd();
    rev_ret_time(); //Обратная связь со времением
    rev_ret(); //Обратная связь с движками
}
void MainWindow::on_toolButton_9_clicked() // Кадр назад
{
    frames_to_map(num_frame,frame); //запись данных кадра в мап
    frame_text_all();
    if(num_frame!=1){
        num_frame-=1;
    }
    else
        return;
    frame_num_lcd();
    frame_sum_lcd();
    rev_ret_time(); //Обратная связь со времением
    rev_ret(); //Обратная связь с движками
}
void MainWindow::on_toolButton_11_clicked() //Кадр вперёд
{
    frames_to_map(num_frame,frame); //запись данных кадра в мап
    frame_text_all();
    if(num_frame==frames_list.size())
        return;
    num_frame+=1;
    frame_num_lcd();
    frame_sum_lcd();
    rev_ret_time(); //Обратная связь со времением
    rev_ret(); //Обратная связь с движками

}
void MainWindow::on_toolButton_15_clicked() //В начало
{
    frames_to_map(num_frame,frame); //запись данных кадра в мап
    frame_text_all();
    if(num_frame!=1){
        num_frame=1;
    }
    else
        return;
    frame_num_lcd();
    frame_sum_lcd();
    rev_ret_time(); //Обратная связь со времением
    rev_ret(); //Обратная связь с движками
}
void MainWindow::on_toolButton_13_clicked() // В конец
{
    frames_to_map(num_frame,frame); //запись данных кадра в мап
    frame_text_all();
    num_frame=frames_list.size();
    frame_num_lcd();
    frame_sum_lcd();
    rev_ret_time(); //Обратная связь со времением
    rev_ret(); //Обратная связь с движками
}
void MainWindow::on_pushButton_100_clicked() //Удалить кадр
{
    del_frame(num_frame);
    frame_text_all();
    if(num_frame==frames_list.size()+1 && num_frame!=1){
        num_frame-=1;
    }
    frame_num_lcd();
    frame_sum_lcd();
    rev_ret_time(); //Обратная связь со времением
    rev_ret(); //Обратная связь с движками
}



//======================Animation====================================================
void MainWindow::block_btn_anim(bool block)
{
    if(block){
        ui->pushButton_inv->setEnabled(false);
        ui->pushButton_111->setEnabled(false);
        ui->pushButton_100->setEnabled(false);
        ui->toolButton_9->setEnabled(false);
        ui->toolButton_11->setEnabled(false);
        ui->toolButton_13->setEnabled(false);
        ui->toolButton_15->setEnabled(false);
        ui->lineEdit_time->setEnabled(false);
        ui->slider_time->setEnabled(false);
        ui->toolButton_all_time->setEnabled(false);
    }
    else{
        ui->pushButton_inv->setEnabled(true);
        ui->pushButton_111->setEnabled(true);
        ui->pushButton_100->setEnabled(true);
        ui->toolButton_9->setEnabled(true);
        ui->toolButton_11->setEnabled(true);
        ui->toolButton_13->setEnabled(true);
        ui->toolButton_15->setEnabled(true);
        ui->lineEdit_time->setEnabled(true);
        ui->slider_time->setEnabled(true);
        ui->toolButton_all_time->setEnabled(true);
    }

}

void MainWindow::animation_init() // действия перед началом анимации
{
    block_anim=true; // блокировка записи кадров при анимации и переключении кадров
    anim_stop=0;
    anim_pause=0; // стоп анимации ставим в ноль
    block_btn_anim(1); //блокировка кнопок навигации и редактирования анимации во время воспроизведения
}

void MainWindow::animation_body_one() //первый кадр анимации**********************************
{
    if (anim_stop==1)  //останов анимации по нажатию "стоп" (предохранитель)
        return;
    num_frame=1; //номер первого запрашиваемого кадра
    if(num_frame<=frames_list.size()-1){
        frame_num_lcd();
        frame_text_all();
//        block_anim=true; // блокировка записи кадров при анимации и переключении кадров
        rev_ret_time(); //Обратная связь со времением
        rev_ret(); //Обратная связь с движками
//        block_anim=false; // блокировка записи кадров при анимации и переключении кадров
        int time_frame=time_current*10;
//===========таймер анимации========
        timer->start(time_frame);
//===========прогресс анимации========
        int p=100.0*(num_frame)/frames_list.size();
        ui->progressBar->setValue(p);
//=====================================
    }
    else{
        ui->toolButton_16->click();
        return;
    }
}

void MainWindow::animation_body() //все кадры анимации (кроме первого)*************************
{
    if (anim_pause==1){ //останов анимации по нажатию "pause"
        block_anim=false; // блокировка записи кадров при анимации и переключении кадров
        timer->stop();
        return;
    }

    if (anim_stop==1) //останов анимации по нажатию "стоп"
        return;

    if(num_frame<=frames_list.size()-1){
        num_frame+=1;
        block_anim=true; // блокировка записи кадров при анимации и переключении кадров
        frame_num_lcd();


//        block_anim=true; // блокировка записи кадров при анимации и переключении кадров
        rev_ret_time(); //Обратная связь со времением
        rev_ret(); //переключение элементов в соответствии с вызываемым кадром анимации
//        block_anim=false; // блокировка записи кадров при анимации и переключении кадров
        int time_frame=time_current*10;
//===========таймер анимации==============
        timer->start(time_frame);
// ===========для прогресс-бара=============
        int p=100.0*(num_frame)/frames_list.size();
        ui->progressBar->setValue(p);
//============================================
    }
    else{
        if (repeat==1){
            animation_init(); //инициализация анимации
            animation_body_one();
            return;
        }
        else{
            ui->toolButton_16->click();
            anim_stop=1; //ставим флаг для прекращения анимации
            return;
        }
    }
}

void MainWindow::on_toolButton_12_clicked(bool checked) //старт анимации***********************
{
    if (checked){
        if(anim_start==1)  //если анимация уже включена, то ничего не делаем
            return;
        if(anim_pause==1){  //если анимация на паузе
            if(num_frame>1 && num_frame!=frames_list.size()-1){ // защита от out of range при прокрутке на последний и первый кадр при снятии с паузы
            num_frame-=1;
            }
            anim_pause=0; //ставим флаг для паузы анимации
            animation_init(); //инициализация анимации
            animation_body();
            return;
        }

        anim_stop=0; //ставим флаг для прекращения анимации
        animation_init(); //инициализация анимации
        animation_body_one(); // запуск анимации
    }
        else {
        if (anim_stop==0){
        anim_pause=1; //ставим флаг для паузы анимации
        block_anim=false; // блокировка записи кадров при анимации и переключении кадров
        block_btn_anim(0); //блокировка кнопок навигации и редактирования анимации во время воспроизведения
        }
    }
}

void MainWindow::on_toolButton_16_clicked() //Stop animation***********************************
{
    timer->stop();
    anim_stop=1; //ставим флаг для прекращения анимации
    anim_start=0; //ставим готовность для включения анимации
    frames_to_map(num_frame,frame); //запись данных кадра в мап
    frame_text_all();
    block_btn_anim(0); //блокировка кнопок навигации и редактирования анимации во время воспроизведения
    if (anim_pause==0)
    ui->toolButton_12->setChecked(false); // отжимаем кнопку "старт"
    anim_pause=0;
    num_frame=1; //скидываем номер кадра на первый
    frame_num_lcd();

    ui->progressBar->setValue(num_frame); // прогресс-бар ставим в ноль
//    block_anim=true; // блокировка записи кадров при анимации и переключении кадров
    rev_ret(); //переключение элементов в соответствии с вызываемым кадром анимации
    rev_ret_time(); //Обратная связь со времением
//    block_anim=false; // блокировка записи кадров при анимации и переключении кадров
}


void MainWindow::on_toolButton_14_clicked(bool checked) // повтор анимации по кругу
{
    if (checked){
        repeat=1;
    }
    else
        repeat=0;
}

//====================================Files_operation======================================

void MainWindow::save_animation() // сохранение анимации в файл
{
    QByteArray buff_all;

    frames_to_map(num_frame,frame); // Пишем текущий кадр в массив
    frame_text_all(); // пишем все кадры в QList

    for(int i=0;i<frames_list.size();++i){ // цикл для извлечения значений всех кадров в буффер
        double time_d=frames_time.at(i)*100;
        int times_int=time_d;
        QByteArray ba,ba1;
        ba1.setNum(times_int, 16);
        ba=QByteArray::fromHex(ba1);
        ba1=ba;
        ba.fill(0,2);
        ba.replace(ba.size()-ba1.size(),ba1.size(),ba1);
        buff_all.append(ba);
        buff_all+=frames_list.at(i);
    }


// times all than frames all!!!!!
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
                                                    tr("Animation Files (*.amt)"));
    if (fileName != "") {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
            return;// сообщение об ошибке
        else {
            file.write(buff_all);
            file.close();
        }
    }
}

void MainWindow::open_animation() // открытие анимации из файла
{
    QByteArray buffer_array_all;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                                                    tr("Animation Files (*.amt)"));
    if (fileName != ""){
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
            return;// сообщение об ошибке
        else{
            buffer_array_all = file.readAll();
            file.close();
        }
    }
    else
        return; // Если передумали открывать файл, то прерываем дальнейшее выполнение операции

//===============================Чистим массивы для нового проекта=============================

    frame.clear(); // очищаем содержимое буфера текущего кадра
    int size=frames_list.size();
    for(int i=size-1;i>=0;--i){
        frames_list.removeAt(i); //чистим массив кадров (удаляем все к чертям)
        frames_time.removeAt(i); //чистим массив времени кадров
    }
//=============================================================================================

    int frames=0; //счётчик кадров
    for(int i=0;i<buffer_array_all.size();i+=num_of_ch+2){
        QByteArray buffer_frame,buffer_time;
        buffer_frame=buffer_array_all.mid(i,num_of_ch+2); // массив с данными текущего кадра (по всем каналам)
        buffer_time=buffer_frame.left(2);
        buffer_frame.remove(0,2);
        frames_list.append(buffer_frame);
        int boo=buffer_time.toHex().toInt(0,16);
        double time_double=boo*0.01;
        frames_time.append(time_double);
        frames+=1;
    }
    frame_sum_lcd();
    num_frame=1;
    frame_num_lcd();
    frame=frames_list.at(num_frame-1);
    rev_ret(); //обратная связь с движка
    rev_ret_time();
}

//=======================Блок времени=============================

void MainWindow::on_slider_time_valueChanged(int value) //передаёт значение со слайдера в окно длинны кадра
{  
    double sl= value*0.01;// float value   
    QString time_string=QString::number(sl,'f',2); //=============конвертируем из double в string c точностью 2 знака после запятой
    if (block_anim==false || anim_pause==1){// изменения вносятся в мап, только если анимация совсем выключена, либо стоит на паузе
        time_current=sl;
        frames_time[num_frame-1]=sl; //отправка времени кадра в QList
    }
    if(entr_tme==0){ //блокировка slider-lineEdit
        ui->lineEdit_time->setText(time_string);
    }
    else{
        entr_tme=0;
        return;
    }  
}
void MainWindow::on_lineEdit_time_editingFinished() //передаёт значение в QList и слайдер, перемещает на него фокус
{
    entr_tme=1;
    double tme_double=ui->lineEdit_time->text().toDouble()*100;
    int tme_sli=tme_double;
    ui->slider_time->setValue(tme_sli);
    ui->lineEdit_time->clearFocus(); // Не
}

void MainWindow::on_toolButton_all_time_clicked() // Запись текущего времени на все кадры
{
    for(int i=0;i<frames_time.size();++i){
        frames_time[i]=time_current;
    }
}

//===============================================     Работа с ком-портом    =================================

void MainWindow::on_com_port_Button_clicked()
{
    if(com_port_window==false){
        com_port_w *com_port = new com_port_w;
        com_port->show();
        connect(com_port,SIGNAL(flag_close_win(bool)),this,SLOT(com_port_window_status(bool)));
        com_port_window=true;
    }
    else
        return;
}
void MainWindow::com_port_window_status(bool flag)
{
    com_port_window=flag;
    //----------------------------flag----------------------------
}


