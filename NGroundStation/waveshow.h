#ifndef WAVESHOW_H
#define WAVESHOW_H

#include <QWidget>
#include "Headfile.h"

#define MAX_DATA_CH 27 //一屏显示的最大数据点数
#define MAX_DATA_SIZE 100 //一屏显示的最大数据点数
#define MAX_CH_LEN (50000)//40000  20000//20000*5
extern QwtPlotCurve *ch_curve[MAX_DATA_CH];
extern double ch_time[MAX_DATA_SIZE];
extern double ch_val[MAX_DATA_CH][MAX_DATA_SIZE];
extern int ch_val_cnt[MAX_DATA_CH];
extern int iActDataLen;
extern bool showflag;
typedef struct
{
   QVector<double> xdata,ydata;
   uint64_t cnt;
   bool update_flag;
}qwtdata;

extern qwtdata qwt_ch_val[MAX_DATA_CH];

//extern QVector<double> xdata,ydata;

namespace Ui {
class waveshow;
}

class waveshow : public QWidget
{
    Q_OBJECT

public:
    explicit waveshow(QWidget *parent = nullptr);
    ~waveshow();
    void closeEvent(QCloseEvent *);//重写关闭事件

    Ui::waveshow *ui;
    void wave_init();
    void show_redraw(int present);
    void receiveMessage(const QString& info);
    void progress(int present);

private slots:
   void timerwave();
   void showItemChecked(const QVariant &itemInfo, bool on);
   void select_axis_zoomer( QRectF );
   void showwave();
   void readcsvfile();
//  Ui::waveshow *ui;
   void on_bt_zoominx_clicked();

   void on_bt_zoominy_clicked();
   void on_bt_zoomouty_clicked();
   void on_bt_zoomoutx_clicked();
   void on_bt_zoominy_zero_clicked();
   void on_bt_clearshow_clicked();
   void on_bt_startshow_clicked();
   void on_bt_export_clicked();

   void on_bt_import_clicked();

private:
   QMutex mutexwave;

signals:
     void exitwindows();//新建一个信号槽

};

#endif // WAVESHOW_H
