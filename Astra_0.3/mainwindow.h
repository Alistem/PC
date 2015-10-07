#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void frame_text(int,QByteArray);
    void frame_text_all();
    void change_bright_btn(int);
    void massive_frame(int);

    void frame_num_lcd();
    void frame_sum_lcd();
    void frames_to_map(int,QByteArray);
    void new_frame();
    void add_frame(int);
    void del_frame(int);
    void rev_ret();
    void rev_ret_time();

    void stylesheet_switch();

    void on_verticalSlider_valueChanged(int value);
    void on_verticalSlider_2_valueChanged(int value);
    void on_verticalSlider_3_valueChanged(int value);
    void on_verticalSlider_4_valueChanged(int value);
    void on_verticalSlider_5_valueChanged(int value);
    void on_verticalSlider_6_valueChanged(int value);
    void on_verticalSlider_7_valueChanged(int value);
    void on_verticalSlider_8_valueChanged(int value);
    void on_verticalSlider_9_valueChanged(int value);
    void on_verticalSlider_10_valueChanged(int value);
    void on_verticalSlider_11_valueChanged(int value);
    void on_verticalSlider_12_valueChanged(int value);
    void on_verticalSlider_13_valueChanged(int value);
    void on_verticalSlider_14_valueChanged(int value);
    void on_verticalSlider_15_valueChanged(int value);
    void on_verticalSlider_16_valueChanged(int value);
    void on_verticalSlider_17_valueChanged(int value);
    void on_verticalSlider_18_valueChanged(int value);
    void on_verticalSlider_19_valueChanged(int value);
    void on_verticalSlider_20_valueChanged(int value);
    void on_verticalSlider_21_valueChanged(int value);
    void on_verticalSlider_22_valueChanged(int value);
    void on_verticalSlider_23_valueChanged(int value);
    void on_verticalSlider_24_valueChanged(int value);
    void on_verticalSlider_25_valueChanged(int value);
    void on_verticalSlider_26_valueChanged(int value);
    void on_verticalSlider_27_valueChanged(int value);
    void on_verticalSlider_28_valueChanged(int value);
    void on_verticalSlider_29_valueChanged(int value);
    void on_verticalSlider_30_valueChanged(int value);
    void on_verticalSlider_31_valueChanged(int value);
    void on_verticalSlider_32_valueChanged(int value);

    void on_slider_time_valueChanged(int value);

    void on_pushButton_111_clicked();
    void on_toolButton_9_clicked();
    void on_toolButton_11_clicked();
    void on_toolButton_15_clicked();
    void on_toolButton_13_clicked();
    void on_pushButton_100_clicked();


    void on_toolButton_12_clicked(bool);
    void on_toolButton_16_clicked();
    void on_toolButton_14_clicked(bool);

    void on_lineEdit_editingFinished();
    void on_lineEdit_2_editingFinished();
    void on_lineEdit_3_editingFinished();
    void on_lineEdit_4_editingFinished();
    void on_lineEdit_5_editingFinished();
    void on_lineEdit_6_editingFinished();
    void on_lineEdit_7_editingFinished();
    void on_lineEdit_8_editingFinished();
    void on_lineEdit_9_editingFinished();
    void on_lineEdit_10_editingFinished();
    void on_lineEdit_11_editingFinished();
    void on_lineEdit_12_editingFinished();
    void on_lineEdit_13_editingFinished();
    void on_lineEdit_14_editingFinished();
    void on_lineEdit_15_editingFinished();
    void on_lineEdit_16_editingFinished();
    void on_lineEdit_17_editingFinished();
    void on_lineEdit_18_editingFinished();
    void on_lineEdit_19_editingFinished();
    void on_lineEdit_20_editingFinished();
    void on_lineEdit_21_editingFinished();
    void on_lineEdit_22_editingFinished();
    void on_lineEdit_23_editingFinished();
    void on_lineEdit_24_editingFinished();
    void on_lineEdit_25_editingFinished();
    void on_lineEdit_26_editingFinished();
    void on_lineEdit_27_editingFinished();
    void on_lineEdit_28_editingFinished();
    void on_lineEdit_29_editingFinished();
    void on_lineEdit_30_editingFinished();
    void on_lineEdit_31_editingFinished();
    void on_lineEdit_32_editingFinished();
    void on_lineEdit_time_editingFinished();


    void animation_init();
    void block_btn_anim(bool);
    void animation_body_one();
    void animation_body();

    void save_animation();
    void open_animation();

    void on_action8_triggered(bool checked);
    void on_action16_triggered(bool checked);
    void on_action24_triggered(bool checked);
    void on_action32_triggered(bool checked);

    void on_pushButton_1_clicked(bool checked);
    void on_pushButton_2_clicked(bool checked);
    void on_pushButton_3_clicked(bool checked);
    void on_pushButton_4_clicked(bool checked);
    void on_pushButton_5_clicked(bool checked);
    void on_pushButton_6_clicked(bool checked);
    void on_pushButton_7_clicked(bool checked);
    void on_pushButton_8_clicked(bool checked);
    void on_pushButton_9_clicked(bool checked);
    void on_pushButton_10_clicked(bool checked);
    void on_pushButton_11_clicked(bool checked);
    void on_pushButton_12_clicked(bool checked);
    void on_pushButton_13_clicked(bool checked);
    void on_pushButton_14_clicked(bool checked);
    void on_pushButton_15_clicked(bool checked);
    void on_pushButton_16_clicked(bool checked);
    void on_pushButton_17_clicked(bool checked);
    void on_pushButton_18_clicked(bool checked);
    void on_pushButton_19_clicked(bool checked);
    void on_pushButton_20_clicked(bool checked);
    void on_pushButton_21_clicked(bool checked);
    void on_pushButton_22_clicked(bool checked);
    void on_pushButton_23_clicked(bool checked);
    void on_pushButton_24_clicked(bool checked);
    void on_pushButton_25_clicked(bool checked);
    void on_pushButton_26_clicked(bool checked);
    void on_pushButton_27_clicked(bool checked);
    void on_pushButton_28_clicked(bool checked);
    void on_pushButton_29_clicked(bool checked);
    void on_pushButton_30_clicked(bool checked);
    void on_pushButton_31_clicked(bool checked);
    void on_pushButton_32_clicked(bool checked);

    void on_action_red_triggered();
    void on_action_green_triggered();
    void on_action_blue_triggered();
    void on_com_port_Button_clicked();
    void com_port_window_status(bool);


private:
    Ui::MainWindow *ui;
    int ch_num,color_button;
    QString init_style,init_style_red,init_style_green,init_style_blue;
    //==================Animation==============
    QList<QByteArray> frames_list; // массив кадров (каждый элемент которого есть - один кадр (со значени¤ми ¤ркости))
    QByteArray frame; // массив с данными текущего кадра (по всем каналам);
    double time_current;
    QList<double> frames_time;

    //=========================================


    QString back_color,back_color_red,back_color_green,back_color_blue;
    QString btn_off;
    int frame_time;
    int num_frame;
    int num_sum;
    int num_channels; // ол-во контроллеров в св¤зке (суммарное количество каналов)
    int num_of_ch; // кол-во каналов (на будущее, дл¤ расширени¤)
    int on_gradient_one;
    int on_gradient_two;
    int on_gradient_three;
    int on_gradient_four;
    //==================
    int off_gradient_one;
    int off_gradient_two;
    int off_gradient_three;
    int off_gradient_four;
    bool all_frames;
    QString frame_string,frames_string_sum;

    bool entr_bright,entr_tme;

    //===================Animation=============
    bool block_anim,anim_stop,anim_pause,repeat,anim_start;
    QTimer *timer;
    //==================com-port-==============
    bool com_port_window;
};

#endif // MAINWINDOW_H
