#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <QWidget>

namespace Ui {
class calibration;
}

class calibration : public QWidget
{
    Q_OBJECT
public:
    explicit calibration(QWidget *parent = nullptr);
    ~calibration();
    void closeEvent(QCloseEvent *);//重写关闭事件
    bool Send_CAL(uint8_t cal_flag,uint8_t cal_step,uint8_t cal_cmd);

    QTimer *timer=nullptr;
private slots:
    void on_gyro_cal_start_clicked();
    void on_acc_cal_start_clicked();
    void on_mag_cal_start_clicked();
    void on_hor_cal_start_clicked();
    void on_bt1_acc_cal_clicked();
    void on_bt2_acc_cal_clicked();
    void on_bt3_acc_cal_clicked();
    void on_bt4_acc_cal_clicked();
    void on_bt5_acc_cal_clicked();
    void on_bt6_acc_cal_clicked();
    void on_get_cal_para_clicked();
    void timer_show();
    void getcheck(uint8_t ch);
    void on_clear_clicked();

    void on_quit_cal_now_clicked();

private:
    Ui::calibration *ui;
signals:
     void exitwindows();//新建一个信号槽
};

#endif // CALIBRATION_H
