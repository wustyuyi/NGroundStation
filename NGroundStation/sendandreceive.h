#ifndef SENDANDRECEIVE_H
#define SENDANDRECEIVE_H

#include <QWidget>
#include "Headfile.h"


extern serial_config config;
extern QSerialPort* serialbasic;


namespace Ui {
class sendandreceive;
}

class sendandreceive : public QWidget
{
    Q_OBJECT
public:
    explicit sendandreceive(QWidget *parent = nullptr);
    ~sendandreceive();
    void closeEvent(QCloseEvent *);//重写关闭事件
//private:
    Ui::sendandreceive *ui;
private slots:
    void init(void);
    void on_bt_clearshow_clicked();
    void timer_serialsendandreceiveEvent();
    void on_bt_sendserialdata_clicked();
    void on_bt_clearstatistics_clicked();

    void on_cb_autosend_stateChanged(int arg1);
    void timer_autosend_event();
    void on_send_period_editingFinished();

    void on_bt_clearsend_clicked();

    void on_bt_send1_clicked();

    void on_bt_send2_clicked();

    void on_bt_send3_clicked();

    void on_bt_send4_clicked();

    void on_bt_send5_clicked();

    void on_opencolse_clicked();
    void serialconfig_update(int index);
    void serialPort_getdata();

    void on_cBox_SerialPortNum_highlighted(int index);

signals:
    void senddata(QString);
    void exitwindows();//新建一个信号槽
};

#endif // SENDANDRECEIVE_H
