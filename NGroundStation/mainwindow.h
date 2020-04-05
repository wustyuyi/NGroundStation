#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Headfile.h"



extern bool serialflag;
extern QSerialPort* serialPort;
extern sendandreceive *serialsendandreceive;
extern serialsetup *usbsetup;
extern waveshow *wave;

extern QString bt_switch_text;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTimer *timer_textshow=nullptr;
    bool threadshow_flag=false;
    //重写关闭事件
    void closeEvent(QCloseEvent *);


    bool serial_init();
    void NCLinkDataPrase(uint8_t data);
    void dataSort(uint8_t *data_buf,uint8_t num);
    void Float2Byte(float *FloatValue, unsigned char *Byte, unsigned char Subscript);
    void Byte2Float(unsigned char *Byte, unsigned char Subscript, float *FloatValue);
private slots:
    void on_bt_serialsetup_clicked();
    void on_bt_switch_clicked();
    void on_bt_sendandreceive_clicked();
    QString GetCPUManID();
    void readdata(QString data);
    void getthreaddata(QByteArray data);


    void NCLinkDataGet();
//private:
    void on_bt_datawave_clicked();
    void on_bt_flightstate_clicked();
    void on_bt_parameter_clicked();
    void redraw();
    void on_bt_remote_clicked();
    void on_bt_calibration_clicked();

public slots:
    void help();
    void surfWeb();
    void blog();
    void store();
    void update();
private:
    QByteArray receivedData;       //接收的数据
    QByteArray sendData;           //发送的数据
    QMutex mutex;
    QMutex mutexdata;
    qint64  byteLen_now;
    qint64  byteLen_read;
signals:
     void redrawparameter(uint8_t);//新建一个信号槽
     void sendcheck(uint8_t);//新建一个信号槽
     void showwave();
};
#endif // MAINWINDOW_H
