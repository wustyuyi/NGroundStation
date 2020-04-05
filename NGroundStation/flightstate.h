#ifndef FLIGHTSTATE_H
#define FLIGHTSTATE_H

#include <Headfile.h>
#include <QWidget>


#define flightstate_wave_num_max  200
extern QwtPlotCurve *flightstate_ch[3];
extern double flightstate_ch_val[3][flightstate_wave_num_max];
extern double flightstate_time[flightstate_wave_num_max];
extern int waveclear_cnt;

namespace Ui {
class flightstate;
}

class flightstate : public QWidget
{
    Q_OBJECT
public:
    explicit flightstate(QWidget *parent = nullptr);
    ~flightstate();
    //重写关闭事件
    void closeEvent(QCloseEvent *);
//private:
    Ui::flightstate *ui;
private slots:
    void timer_flight_show();
    void on_clearwave_clicked();
    void showItem(const QVariant &itemInfo, bool on);
    void on_defaultwave_clicked();
    void on_stopwave_clicked();
signals:
     void exitwindows();//新建一个信号槽
};

#endif // FLIGHTSTATE_H
