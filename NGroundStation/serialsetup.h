#ifndef SERIALSETUP_H
#define SERIALSETUP_H

#include <QWidget>
#include "Headfile.h"



extern serial_config serialconfig;


namespace Ui {
class serialsetup;
}

class serialsetup : public QWidget
{
    Q_OBJECT

public:
    explicit serialsetup(QWidget *parent = nullptr);
    ~serialsetup();
    void closeEvent(QCloseEvent *);//重写关闭事件
    void serialinit(void);
    void serialsetup_enable(bool flag);
//private:
    Ui::serialsetup *ui;
private slots:
    void serialconfig_update(int index);
    void serial_update();
    void on_cBox_SerialPortNum_highlighted(int index);
    void on_cBox_SerialPortBaud_currentIndexChanged(int index);

signals:
     void exitwindows();//新建一个信号槽
};

#endif // SERIALSETUP_H
