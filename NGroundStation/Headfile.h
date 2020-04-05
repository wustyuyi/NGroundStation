#ifndef HEADFILE_H
#define HEADFILE_H

#define ABS(X)  (((X)>0)?(X):-(X))
#define MAX(a,b)  ((a)>(b)?(a):(b))
#define MIN(a,b)  ((a)>(b)?(b):(a))

#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>


#include <QTime>
#include <QTimer>
#include <qvector.h>
#include <QtCore/qglobal.h>
#include <QtSerialPort/QSerialPort>         //提供访问串行端口的功能
#include <QtSerialPort/QSerialPortInfo>     //提供关于现有串行端口的信息。
#include <QMessageBox>
//#include <QDebug>
#include <qdatetime.h>
#include <QProcess>
#include <qmutex.h>
#include <q3dbars.h>

typedef  struct{
    int baudrate;
    int parity;
    int dataBit;
    int stopBit;
    QString serialname;
}serial_config;

//#include <qwt.h>
#include <Qwt/qwt.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot.h>
#include <qwt_legend.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <QtMath>
#include <qwt_plot_zoomer.h>
#include <qwt_scale_draw.h>
#include <qwt_picker_machine.h>
#include <QPainter>
#include <qthread.h>

#include "gaugecompass.h"
#include "gaugeplane.h"
#include "gaugespeed.h"
#include "rulerdouble.h"
#include "rulerbar.h"
#include "customrocker.h"
#include "xslider.h"


#include "serialsetup.h"
#include "ui_serialsetup.h"
#include "sendandreceive.h"
#include "ui_sendandreceive.h"
#include "waveshow.h"
#include "flightstate.h"
#include "serialportthread.h"
#include "setparameter.h"
#include "remotectrl.h"
#include "calibration.h"

#endif // HEADFILE_H
