#ifndef SETPARAMETER_H
#define SETPARAMETER_H

#include <QWidget>


extern bool serialflag;


namespace Ui {
class SetParameter;
}

class SetParameter : public QWidget
{
    Q_OBJECT

public:
    explicit SetParameter(QWidget *parent = nullptr);
    ~SetParameter();
    void closeEvent(QCloseEvent *);//重写关闭事件
    void Send_Cmd_Check(uint8_t cmd,uint8_t data);
    void Send_PID(uint8_t group,uint16_t kp1,uint16_t ki1,uint16_t kd1,
                                uint16_t kp2,uint16_t ki2,uint16_t kd2,
                                uint16_t kp3,uint16_t ki3,uint16_t kd3);
    void Send_Parameter(uint8_t group,
                        uint16_t targeheight,
                        uint16_t safeheight,
                        uint16_t safevbat,
                        uint16_t maxheight,
                        uint16_t maxradius,
                        uint16_t maxupvel,
                        uint16_t maxdownvel,
                        uint16_t maxhorvel);
private:
    Ui::SetParameter *ui;
signals:
     void exitwindows();//新建一个信号槽
private slots:
    void timer_setparameter_show();
    void on_readall_clicked();
    void on_defaultall_clicked();
    void on_writeall_clicked();
    void setparameter_show(uint8_t ch);
    void getcheck(uint8_t ch);
    void on_clear_clicked();
    void on_write_clicked();
    void on_read_clicked();
    void on_factory_clicked();
};

#endif // SETPARAMETER_H
