#ifndef REMOTECTRL_H
#define REMOTECTRL_H

#include <QWidget>

namespace Ui {
class remotectrl;
}

class remotectrl : public QWidget
{
    Q_OBJECT

public:
    explicit remotectrl(QWidget *parent = nullptr);
    ~remotectrl();
    void closeEvent(QCloseEvent *);//重写关闭事件
    void Send_RC(uint16_t ch0,uint16_t ch1,uint16_t ch2,uint16_t ch3,uint16_t ch4,uint16_t ch5,uint16_t ch6,uint16_t ch7,uint8_t unlock_flag,uint8_t takeoff_flag);
    void Send_DIS(uint8_t move_flag,uint8_t move_order,uint16_t move_distance);
private:
    Ui::remotectrl *ui;
signals:
     void exitwindows();//新建一个信号槽
private slots:
     void on_checkBox_stateChanged(int arg1);
     void timer_event();
     void sort_data12(double angle,double r);
     void sort_data34(double angle,double r);
     void sort_data_bt_unlock(bool flag);
     void sort_data_bt_takeoff(bool flag);
     void getcheck(uint8_t ch);
     void on_clear_clicked();
     void on_bt_send1_clicked();
     void on_bt_send2_clicked();
     void on_bt_send3_clicked();
     void on_bt_send4_clicked();
     void on_bt_send5_clicked();
     void on_bt_send6_clicked();
     void on_manualsend_clicked();
};

#endif // REMOTECTRL_H
