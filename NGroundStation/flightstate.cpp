#include "Headfile.h"
#include "flightstate.h"
#include "ui_flightstate.h"

QwtPlotCurve *flightstate_ch[3];
double flightstate_ch_val[3][flightstate_wave_num_max];
double flightstate_time[flightstate_wave_num_max]={0};
bool waveshow_flag=true;
int waveclear_cnt=0;
QTimer *flight_show=nullptr;
flightstate::flightstate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::flightstate)
{
    ui->setupUi(this);
    this->setWindowTitle("飞控状态");                                 //设置主界面名称
    setWindowIcon(QIcon(":/image/Logo.png"));                       //设置主界面LOGO
    //setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);  // 禁止最大化按钮
    //setFixedSize(this->width(),this->height());                   // 禁止拖动窗口大小

    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor(233,235,254));//QColor(78,168,243)
    this->setPalette(palette);
    flightstate_ch[0] = new QwtPlotCurve("Pitch");
    flightstate_ch[1] = new QwtPlotCurve("Roll");
    flightstate_ch[2] = new QwtPlotCurve("Yaw");

    flightstate_ch[0]->setPen(QColor(255,0,0));   //设定前景颜色，就是字体颜色
    flightstate_ch[1]->setPen(QColor(0,255,0));     //设定前景颜色，就是字体颜色
    flightstate_ch[2]->setPen(QColor(0,0,255));  //设定前景颜色，就是字体颜

    //设置示波器背景网格
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setPen(Qt::white,1.0,Qt::DashLine );
    grid->enableX(true);
    grid->enableXMin(true);
    grid->enableY(true);
    grid->enableYMin(true);
    grid->setMajorPen(Qt::white,1.0,Qt::SolidLine );
    grid->attach(ui->qwtPlot);

    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,-180.0,360.0);
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,0,flightstate_wave_num_max);
    ui->qwtPlot->setTitle("姿态角曲线");
    ui->qwtPlot->setAxisTitle(QwtPlot::yLeft,"Degree");
    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom,"Sampling Points");

    //ui->qwtPlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);//设置右边标注
    QwtLegend *legend = new QwtLegend;//实例化图例
    legend->setDefaultItemMode( QwtLegendData::Checkable );//图例可以选择,Checkable
    ui->qwtPlot->insertLegend(legend, QwtPlot::RightLegend );//qwtPlot中插入图例
    //连接槽，处理选择事件
    connect(legend,
            SIGNAL(checked(const QVariant &, bool, int)),
            SLOT(showItem( const QVariant &, bool)));//点击图例操作

    new QwtPlotMagnifier(ui->qwtPlot->canvas());//使用滚轮放大/缩小
    new QwtPlotPanner(ui->qwtPlot->canvas());//使用鼠标左键平移

    /*
    //鼠标左键选择区域放大：（右键还原）
    QwtPlotZoomer* zoomer = new QwtPlotZoomer( ui->qwtPlot->canvas() );
    zoomer->setRubberBandPen( QColor( Qt::black ) );
    zoomer->setTrackerPen( QColor( Qt::black ) );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,Qt::RightButton, Qt::ControlModifier );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton );
    */


    for(uint16_t i=0;i<flightstate_wave_num_max;i++)
    {
       flightstate_time[i] = i;
    }


    flight_show = new QTimer(this);
    connect(flight_show, SIGNAL(timeout()), this, SLOT(timer_flight_show()));
    flight_show->start(50);//20HZ
}

flightstate::~flightstate()
{
    delete ui;
}


void flightstate::timer_flight_show()
{

   ui->pitch->setText(QString::number(pitch,10,2));
   ui->roll->setText(QString::number(roll,10,2));
   ui->yaw->setText(QString::number(yaw,10,2));

   ui->alt->setText(QString::number(alt_pos_fus,10,2));
   ui->vz->setText(QString::number(alt_vel_fus,10,2));
   ui->accelz->setText(QString::number(alt_accel_fus));

   ui->vbat->setText(QString::number(vbat,10,2));
   ui->imu_temp->setText(QString::number(imu_temp,10,2));

   if(fly_mode==1) ui->flightmode->setText("姿态");
   else if(fly_mode==2) ui->flightmode->setText("定高");

   if(armed_flag==0) ui->armed->setText("锁定");
   else if(armed_flag==1) ui->armed->setText("解锁");

   if(waveshow_flag==true)
   {
       for(uint16_t i=0;i<3;i++)
           flightstate_ch[i]->setSamples(flightstate_time,flightstate_ch_val[i],waveclear_cnt);//更新每个通道数据
       for(uint16_t i=0;i<3;i++)
           flightstate_ch[i]->attach(ui->qwtPlot);//将通道数据写入qwt
       ui->qwtPlot->replot();
   }

   ui->ax->setText(QString::number(ax));
   ui->ay->setText(QString::number(ay));
   ui->az->setText(QString::number(az));
   ui->gx->setText(QString::number(gx));
   ui->gy->setText(QString::number(gy));
   ui->gz->setText(QString::number(gz));
   ui->mx->setText(QString::number(mx));
   ui->my->setText(QString::number(my));
   ui->mz->setText(QString::number(mz));

   ui->ch0->setText(QString::number(rc[0]));
   ui->ch1->setText(QString::number(rc[1]));
   ui->ch2->setText(QString::number(rc[2]));
   ui->ch3->setText(QString::number(rc[3]));
   ui->ch4->setText(QString::number(rc[4]));
   ui->ch5->setText(QString::number(rc[5]));
   ui->ch6->setText(QString::number(rc[6]));
   ui->ch7->setText(QString::number(rc[7]));

   ui->ch0_pro->setValue(rc[0]);  // 当前进度
   ui->ch1_pro->setValue(rc[1]);  // 当前进度
   ui->ch2_pro->setValue(rc[2]);  // 当前进度
   ui->ch3_pro->setValue(rc[3]);  // 当前进度
   ui->ch4_pro->setValue(rc[4]);  // 当前进度
   ui->ch5_pro->setValue(rc[5]);  // 当前进度
   ui->ch6_pro->setValue(rc[6]);  // 当前进度
   ui->ch7_pro->setValue(rc[7]);  // 当前进度


   ui->baro_alt->setText(QString::number(alt_obs_baro));
   ui->ult_alt->setText(QString::number(alt_obs_ult));
   ui->gps_lng->setText(QString::number(gps_lng,10,6));
   ui->gps_lat->setText(QString::number(gps_lat,10,6));
   ui->gps_alt->setText(QString::number(gps_alt));
   ui->gps_vel_e->setText(QString::number(lng_vel_obs));
   ui->gps_vel_n->setText(QString::number(lat_vel_obs));
   ui->numsv->setText(QString::number(gps_satellite_num));

   ui->gaugePlane->setDegValue(roll);
   ui->gaugePlane->setRollValue(pitch);
   ui->gaugeCompass->setValue(360-yaw);

   ui->rulerLine->setValue(yaw);

   ui->rulerBar->setMinValue(alt_pos_fus-50);
   ui->rulerBar->setMaxValue(alt_pos_fus+50);
   ui->rulerBar->setValue(alt_pos_fus);

   ui->gaugeSpeed->setValue(alt_vel_fus);


   ui->opt_vel_p->setText(QString::number(opt_vel_p));
   ui->opt_vel_r->setText(QString::number(opt_vel_r));
}

void flightstate::on_clearwave_clicked()
{   
    for(uint16_t i=0;i<3;i++)
    {
      for(uint16_t j=0;j<flightstate_wave_num_max;j++)  flightstate_ch_val[i][j]=0;
    }
    waveclear_cnt=0;
}



//点击图例，显示相应的曲线
void flightstate::showItem(const QVariant &itemInfo, bool on)
{
     QwtPlotItem *plotItem =ui->qwtPlot->infoToItem(itemInfo);//获取曲线
     if(plotItem)   plotItem->setVisible(on);//根据图例选择状态，设置曲线隐藏还是显示
     //ui->qwtPlot->replot();

}

void flightstate::on_defaultwave_clicked()
{
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,-180.0,360.0);
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,0,flightstate_wave_num_max);
    //ui->qwtPlot->replot();
}

void flightstate::on_stopwave_clicked()
{
    if(ui->stopwave->text()=="停止显示")
    {
       ui->stopwave->setText("开始显示");
       waveshow_flag=false;
    }
    else
    {
      ui->stopwave->setText("停止显示");
      waveshow_flag=true;
    }
}


//当窗口出发关闭事件后，将信号发射出去
void flightstate::closeEvent(QCloseEvent *)
{
    emit exitwindows();
}
