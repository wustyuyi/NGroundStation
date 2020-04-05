#include "waveshow.h"
#include "ui_waveshow.h"
#include "Headfile.h"




QwtPlotCurve *ch_curve[MAX_DATA_CH];
double ch_time[MAX_DATA_SIZE]={0};
double ch_val[MAX_DATA_CH][MAX_DATA_SIZE] = {{0}};
int ch_val_cnt[MAX_DATA_CH];
int iActDataLen = 0;
#define line_point_size (1.0)
uint64_t Zoom_X_Scale=MAX_DATA_SIZE;
qwtdata qwt_ch_val[MAX_DATA_CH];
QTimer *wavetimer=nullptr;
uint64_t ch_max_cnt=0;
bool showflag=true;

/*
QString data_number[MAX_DATA_CH]=
{



};
*/

waveshow::waveshow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::waveshow)
{
    ui->setupUi(this);
    this->setWindowTitle("波形显示");                        //设置主界面名称
    setWindowIcon(QIcon(":/image/wave.png"));               //设置主界面LOGO

    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor(233,235,254));
    this->setPalette(palette);

    ch_curve[0]  = new QwtPlotCurve("Pitch 俯仰角");//0
    ch_curve[1]  = new QwtPlotCurve("Roll 横滚角");//1
    ch_curve[2]  = new QwtPlotCurve("Yaw 偏航角");//2
    ch_curve[3]  = new QwtPlotCurve("lat_pos_fus N向位置估计");//3
    ch_curve[4]  = new QwtPlotCurve("lng_pos_fus E向位置估计");//4
    ch_curve[5]  = new QwtPlotCurve("alt_pos_fus U位置估计");//5
    ch_curve[6]  = new QwtPlotCurve("lat_vel_fus N向速度");//6
    ch_curve[7]  = new QwtPlotCurve("lng_vel_fus E向速度");//7
    ch_curve[8]  = new QwtPlotCurve("alt_vel_fus U向速度");//8
    ch_curve[9]  = new QwtPlotCurve("lat_accel_fus N向加速度");//9
    ch_curve[10] = new QwtPlotCurve("lng_accel_fus E向加速度");//10
    ch_curve[11] = new QwtPlotCurve("alt_accel_fus U向加速度");//11
    ch_curve[12] = new QwtPlotCurve("Pitch_Gyro 俯仰角速度");//12
    ch_curve[13] = new QwtPlotCurve("Roll_Gyro 横滚角速度");//13
    ch_curve[14] = new QwtPlotCurve("Yaw_Gyro 偏航角速度");//14
    ch_curve[15] = new QwtPlotCurve("lat_pos_obs N向位置观测");//15
    ch_curve[16] = new QwtPlotCurve("lng_pos_obs E向位置观测");//16
    ch_curve[17] = new QwtPlotCurve("lat_vel_obs N向速度观测");//17
    ch_curve[18] = new QwtPlotCurve("lng_vel_obs E向速度观测");//18
    ch_curve[19] = new QwtPlotCurve("alt_obs_baro U向气压计位置");//19
    ch_curve[20] = new QwtPlotCurve("alt_obs_ult U向超声波位置");//20
    ch_curve[21] = new QwtPlotCurve("user1 用户数据1");
    ch_curve[22] = new QwtPlotCurve("user2 用户数据2");
    ch_curve[23] = new QwtPlotCurve("user3 用户数据3");
    ch_curve[24] = new QwtPlotCurve("user4 用户数据4");
    ch_curve[25] = new QwtPlotCurve("user5 用户数据5");
    ch_curve[26] = new QwtPlotCurve("user6 用户数据6");


    ch_curve[0]->setPen(QColor(255,165,0),line_point_size);//设置曲线颜色 粗细
    ch_curve[1]->setPen(QColor(255,0,0),line_point_size);//设置曲线颜色 粗细
    ch_curve[2]->setPen(QColor(255,20,147),line_point_size);//设置曲线颜色 粗细

    ch_curve[3]->setPen(QColor(255,0,255),line_point_size);//设置曲线颜色 粗细
    ch_curve[4]->setPen(QColor(224,102,255),line_point_size);//设置曲线颜色 粗细
    ch_curve[5]->setPen(QColor(145,44,238),line_point_size);//设置曲线颜色 粗细

    ch_curve[6]->setPen(QColor(60, 235, 254),line_point_size);//设置曲线颜色 粗细
    ch_curve[7]->setPen(QColor(234, 60, 239),line_point_size);//设置曲线颜色 粗细
    ch_curve[8]->setPen(QColor(60,237,204),line_point_size);//设置曲线颜色 粗细

    ch_curve[9]->setPen(QColor(60,62,255),line_point_size);//设置曲线颜色 粗细
    ch_curve[10]->setPen(QColor(60,180,159),line_point_size);//设置曲线颜色 粗细
    ch_curve[11]->setPen(QColor(50,255,125),line_point_size);//设置曲线颜色 粗细

    ch_curve[12]->setPen(QColor(255,140,105),line_point_size);//设置曲线颜色 粗细
    ch_curve[13]->setPen(QColor(184,134,11),line_point_size);//设置曲线颜色 粗细
    ch_curve[14]->setPen(QColor(107,142,35),line_point_size);//设置曲线颜色 粗细

    ch_curve[15]->setPen(QColor(55,215,0),line_point_size);//设置曲线颜色 粗细
    ch_curve[16]->setPen(QColor(255,0,255),line_point_size);//设置曲线颜色 粗细
    ch_curve[17]->setPen(QColor(160,32,240),line_point_size);//设置曲线颜色 粗细

    ch_curve[18]->setPen(QColor(60, 200, 100),line_point_size);//设置曲线颜色 粗细
    ch_curve[19]->setPen(QColor(100, 60, 150),line_point_size);//设置曲线颜色 粗细
    ch_curve[20]->setPen(QColor(60, 200, 200),line_point_size);//设置曲线颜色 粗细

    ch_curve[21]->setPen(QColor(100, 150, 200),line_point_size);//设置曲线颜色 粗细
    ch_curve[22]->setPen(QColor(60, 150, 150),line_point_size);//设置曲线颜色 粗细
    ch_curve[23]->setPen(QColor(60, 100, 100),line_point_size);//设置曲线颜色 粗细

    ch_curve[24]->setPen(QColor(155,215,50),line_point_size);//设置曲线颜色 粗细
    ch_curve[25]->setPen(QColor(155,0,200),line_point_size);//设置曲线颜色 粗细
    ch_curve[26]->setPen(QColor(155,155,155),line_point_size);//设置曲线颜色 粗细


    for (uint16_t i=0;i<MAX_DATA_CH;i++)
    {
       ch_curve[i]->setStyle(QwtPlotCurve::Lines);//Steps  Lines  Sticks NoCurve
       ch_curve[i]->setPaintAttribute(QwtPlotCurve::MinimizeMemory, true);//  FilterPoints
       ch_curve[i]->setCurveAttribute(QwtPlotCurve::Inverted, true);
       //ch_curve[i]->setRenderHint(QwtPlotItem::RenderAntialiased,true);//线条光滑化
       ch_curve[i]->setLegendAttribute(ch_curve[i]->LegendShowLine);//显示图例的标志，这里显示线的颜色。
       ch_curve[i]->setVisible(false);
    }

    //设置示波器背景网格
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setPen(Qt::white,1.0,Qt::DashLine);
    grid->enableX(false);
    grid->enableXMin(true);

    grid->enableY(true);
    grid->enableYMin(true);
    grid->setMajorPen(Qt::white,1.0,Qt::SolidLine);
    grid->attach(ui->qwtPlot);
    //grid->setVisible(false);

    ui->qwtPlot->setAxisScale(QwtPlot::yLeft  ,-500.0,500.0);
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,0.0,MAX_DATA_SIZE);
    ui->qwtPlot->setTitle("数据波形");
    //ui->qwtPlot->setAxisTitle(QwtPlot::yLeft,"Degree");
    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom,"Sampling Ponits");

    QwtLegend *legendwave = new QwtLegend;//实例化图例
    legendwave->setDefaultItemMode( QwtLegendData::Checkable );//图例可以选择,Checkable
    ui->qwtPlot->insertLegend(legendwave, QwtPlot::RightLegend );//qwtPlot中插入图例
    //连接槽，处理选择事件
    connect(legendwave,SIGNAL(checked(const QVariant &,bool,int)),SLOT(showItemChecked(const QVariant &,bool)));//点击图例操作

    //一个选择器，十字线，以xBottom和yLeft坐标
    QwtPlotPicker *picker= new QwtPlotPicker(QwtPlot::xBottom,
                                             QwtPlot::yLeft,
                                             QwtPlotPicker::CrossRubberBand,
                                             QwtPicker::AlwaysOn,
                                             ui->qwtPlot->canvas());
    picker->setStateMachine(new QwtPickerDragPointMachine());//拖拽点起作用
    picker->setRubberBandPen(QPen(QColor(Qt::blue)));
    picker->setTrackerMode( QwtPicker::AlwaysOn );         //跟踪模式
    picker->setTrackerPen(QColor(Qt::red));                //跟踪显示字体颜色


    //new QwtPlotMagnifier(ui->qwtPlot->canvas());//使用滚轮放大/缩小
    //new QwtPlotPanner(ui->qwtPlot->canvas());   //使用鼠标左键平移
    //QwtPlotPanner *panner = new QwtPlotPanner(ui->qwtPlot->canvas());//默认的左键移动功能
    //panner->setMouseButton( Qt::RightButton ); //设置哪个按钮移动画布,如果不设置(注册掉当前行)按钮默认为左键为移动

    QwtPlotPanner *panner=new QwtPlotPanner(ui->qwtPlot->canvas());
    panner->setMouseButton(Qt::MidButton);//鼠标中键按下平移拖动

    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier(ui->qwtPlot->canvas());//默认的滑轮及右键缩放功能  图形的整体缩放
    magnifier->setMouseButton(Qt::NoButton);//设置无按钮缩放，只有滑轮为缩放画布
    magnifier->setAxisEnabled(QwtPlot::xTop, false);
    //magnifier->setAxisEnabled(QwtPlot::xBottom, false);
    //magnifier->setAxisEnabled(QwtPlot::yLeft, false);

    QwtPlotZoomer *zoomer = new QwtPlotZoomer(ui->qwtPlot->canvas());                            //选择图形局部放大
    zoomer->setRubberBandPen(QColor( Qt::blue ) );//勾选扩大区域边界颜色
    zoomer->setMousePattern(QwtEventPattern::MouseSelect1, Qt::LeftButton, Qt::ControlModifier);//crtl+左键 放大	 右键恢复原样
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2, Qt::RightButton,Qt::AltModifier);    //ctrl+右键==回复到原始状态
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3, Qt::LeftButton,Qt::AltModifier);     //右键==恢复到上一次扩大之前的状态
    zoomer->setZoomBase(true);

    connect(zoomer,SIGNAL(zoomed(QRectF)),this, SLOT(select_axis_zoomer(QRectF)));
    ui->qwtPlot->setAutoReplot(true);


    for(uint16_t i=0;i<MAX_DATA_SIZE;i++)
    {
       ch_time[i]=i;
       ch_val[0][i] = 200*qSin((double)i/ 100 * 2 * 3.1415926);
       ch_val[1][i] = 180*qSin((double)i/ 100 * 2 * 3.1415926+3.1415926/3);
       ch_val[2][i] = 160*qSin((double)i/ 100 * 2 * 3.1415926+3.1415926*2/3);
       ch_val[3][i] = 140*qSin((double)i/ 100 * 2 * 3.1415926+3.1415926);
       ch_val[4][i] = 120*qSin((double)i/ 100 * 2 * 3.1415926);
       ch_val[5][i] = 100*qSin((double)i/ 100 * 2 * 3.1415926+3.1415926/3);
       ch_val[6][i] = 80*qSin((double)i/ 100 * 2 * 3.1415926+3.1415926*2/3);
       ch_val[7][i] = 60*qSin((double)i/ 100 * 2 * 3.1415926+3.1415926);
       ch_val[8][i] = 40*qSin((double)i/ 100 * 2 * 3.1415926);
       ch_val[9][i] = 20*qSin((double)i/ 100 * 2 * 3.1415926+3.1415926/3);
       ch_val[10][i] = 200*qSin((double)i/ 50 * 2 * 3.1415926+3.1415926*2/3);
       ch_val[11][i] = 180*qSin((double)i/ 50 * 2 * 3.1415926+3.1415926);
       ch_val[12][i] = 160*qSin((double)i/ 50 * 2 * 3.1415926);
       ch_val[13][i] = 140*qSin((double)i/ 50 * 2 * 3.1415926+3.1415926/3);
       ch_val[14][i] = 120*qSin((double)i/ 50 * 2 * 3.1415926+3.1415926*2/3);
       ch_val[15][i] = 100*qSin((double)i/ 50 * 2 * 3.1415926+3.1415926);
       ch_val[16][i] = 80*qSin((double)i/ 50 * 2 * 3.1415926);
       ch_val[17][i] = 60*qSin((double)i/ 50 * 2 * 3.1415926+3.1415926/3);
       ch_val[18][i] = 40*qSin((double)i/ 50 * 2 * 3.1415926+3.1415926*2/3);
       ch_val[19][i] = 20*qSin((double)i/ 50 * 2 * 3.1415926+3.1415926);
       ch_val[20][i] = 200*qSin((double)i/ 200 * 2 * 3.1415926+3.1415926*2/3);

       ch_val[21][i] = 150*qSin((double)i/ 50 * 2 * 3.1415926+3.1415926);
       ch_val[22][i] = 100*qSin((double)i/ 50 * 2 * 3.1415926);
       ch_val[23][i] = 50*qSin((double)i/ 50 * 2 * 3.1415926+3.1415926/3);
       ch_val[24][i] = 40*qSin((double)i/ 50 * 2 * 3.1415926+3.1415926*2/3);
       ch_val[25][i] = 20*qSin((double)i/ 20 * 2 * 3.1415926+3.1415926);
       ch_val[26][i] = 200*qSin((double)i/ 20 * 2 * 3.1415926+3.1415926*2/3);
    }

    for (uint16_t i=0;i<MAX_DATA_CH;i++)
    {
        qwt_ch_val[i].xdata.resize(MAX_CH_LEN);
        qwt_ch_val[i].ydata.resize(MAX_CH_LEN);
        qwt_ch_val[i].xdata[0]=0;
        qwt_ch_val[i].ydata[0]=0;
        //qwt_ch_val[i].update_flag=false;
        qwt_ch_val[i].cnt=0;
    }

    for (uint16_t i=0;i<MAX_DATA_CH;i++)
    {
        qwt_ch_val[i].xdata.resize(0);
        qwt_ch_val[i].ydata.resize(0);
    }

    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,0.0,MAX_DATA_SIZE);
    //ui->qwtPlot->setAxisAutoScale(QwtPlot::xBottom,true);
    //ui->qwtPlot->setAxisAutoScale(QwtPlot::yLeft,true);

    wavetimer = new QTimer(this);
    connect(wavetimer, SIGNAL(timeout()), this, SLOT(timerwave()));
    wavetimer->start(5);//50----20hz
}

waveshow::~waveshow()
{
    delete ui;
}


void waveshow::timerwave()
{
/*
    ch_curve[0]->setSamples(ch_time,ch_val[0],ch_val_cnt[0]);//更新每个通道数据
    ch_curve[0]->attach(ui->qwtPlot);//将通道数据写入qwt

    ch_curve[1]->setSamples(ch_time,ch_val[1],ch_val_cnt[1]);//更新每个通道数据
    ch_curve[1]->attach(ui->qwtPlot);//将通道数据写入qwt

    ch_curve[2]->setSamples(ch_time,ch_val[2],ch_val_cnt[2]);//更新每个通道数据
    ch_curve[2]->attach(ui->qwtPlot);//将通道数据写入qwt

    ch_curve[5]->setSamples(ch_time,ch_val[5],ch_val_cnt[5]);//更新每个通道数据
    ch_curve[5]->attach(ui->qwtPlot);//将通道数据写入qwt

    ch_curve[8]->setSamples(ch_time,ch_val[8],ch_val_cnt[8]);//更新每个通道数据
    ch_curve[8]->attach(ui->qwtPlot);//将通道数据写入qwt

    ch_curve[11]->setSamples(ch_time,ch_val[11],ch_val_cnt[11]);//更新每个通道数据
    ch_curve[11]->attach(ui->qwtPlot);//将通道数据写入qwt

    ui->qwtPlot->replot();
    //qDebug() <<"show";

    int i = 0;
    int j = 0;
    if(iActDataLen<MAX_DATA_SIZE)  iActDataLen++;
    else
    {
        for(i=0;i<MAX_DATA_CH;i++)    ch_val[i][MAX_DATA_SIZE-1]=ch_val[i][0];//赋最新数据

        for(i=0;i<(MAX_DATA_CH);i++)
        {
          for(j=0;j<(MAX_DATA_SIZE-1);j++) ch_val[i][j] =  ch_val[i][j+1];
        }
    }
    for(i=0;i<MAX_DATA_CH;i++)    ch_curve[i]->setSamples(ch_time,ch_val[i],iActDataLen);//更新每个通道数据
    for(i=0;i<MAX_DATA_CH;i++)    ch_curve[i]->attach(ui->qwtPlot);//将通道数据写入qwt
    ui->qwtPlot->replot();


    ch_curve[0]->setSamples(xdata,ydata);//更新每个通道数据
    ch_curve[0]->attach(ui->qwtPlot);//将通道数据写入qwt
    ui->qwtPlot->replot();
*/
 /*
    ch_curve[0]->setSamples(qwt_ch_val[0].xdata,qwt_ch_val[0].ydata);//更新每个通道数据
    ch_curve[0]->attach(ui->qwtPlot);//将通道数据写入qwt

    ch_curve[1]->setSamples(qwt_ch_val[1].xdata,qwt_ch_val[1].ydata);//更新每个通道数据
    ch_curve[1]->attach(ui->qwtPlot);//将通道数据写入qwt

    ch_curve[2]->setSamples(qwt_ch_val[2].xdata,qwt_ch_val[2].ydata);//更新每个通道数据
    ch_curve[2]->attach(ui->qwtPlot);//将通道数据写入qwt


    ch_curve[5]->setSamples(qwt_ch_val[5].xdata,qwt_ch_val[5].ydata);//更新每个通道数据
    ch_curve[5]->attach(ui->qwtPlot);//将通道数据写入qwt

    ch_curve[8]->setSamples(qwt_ch_val[8].xdata,qwt_ch_val[8].ydata);//更新每个通道数据
    ch_curve[8]->attach(ui->qwtPlot);//将通道数据写入qwt

    ch_curve[11]->setSamples(qwt_ch_val[11].xdata,qwt_ch_val[11].ydata);//更新每个通道数据
    ch_curve[11]->attach(ui->qwtPlot);//将通道数据写入qwt
///////////////////////////////////////////////////////////////////////////////////////
    ch_curve[3]->setSamples(qwt_ch_val[3].xdata,qwt_ch_val[3].ydata);//更新每个通道数据
    ch_curve[3]->attach(ui->qwtPlot);//将通道数据写入qwt

    ch_curve[4]->setSamples(qwt_ch_val[4].xdata,qwt_ch_val[4].ydata);//更新每个通道数据
    ch_curve[4]->attach(ui->qwtPlot);//将通道数据写入qwt

    ch_curve[6]->setSamples(qwt_ch_val[6].xdata,qwt_ch_val[6].ydata);//更新每个通道数据
    ch_curve[6]->attach(ui->qwtPlot);//将通道数据写入qwt

    ch_curve[7]->setSamples(qwt_ch_val[7].xdata,qwt_ch_val[7].ydata);//更新每个通道数据
    ch_curve[7]->attach(ui->qwtPlot);//将通道数据写入qwt


    ch_curve[9]->setSamples(qwt_ch_val[9].xdata,qwt_ch_val[9].ydata);//更新每个通道数据
    ch_curve[9]->attach(ui->qwtPlot);//将通道数据写入qwt

    ch_curve[10]->setSamples(qwt_ch_val[10].xdata,qwt_ch_val[10].ydata);//更新每个通道数据
    ch_curve[10]->attach(ui->qwtPlot);//将通道数据写入qwt

    ch_curve[12]->setSamples(qwt_ch_val[12].xdata,qwt_ch_val[12].ydata);//更新每个通道数据
    ch_curve[12]->attach(ui->qwtPlot);//将通道数据写入qwt

    ch_curve[13]->setSamples(qwt_ch_val[13].xdata,qwt_ch_val[13].ydata);//更新每个通道数据
    ch_curve[13]->attach(ui->qwtPlot);//将通道数据写入qwt

    ch_curve[14]->setSamples(qwt_ch_val[14].xdata,qwt_ch_val[14].ydata);//更新每个通道数据
    ch_curve[14]->attach(ui->qwtPlot);//将通道数据写入qwt

////////////////////////////////////////////////////////////////////
    ch_curve[15]->setSamples(qwt_ch_val[15].xdata,qwt_ch_val[15].ydata);//更新每个通道数据
    ch_curve[15]->attach(ui->qwtPlot);//将通道数据写入qwt

    ch_curve[16]->setSamples(qwt_ch_val[16].xdata,qwt_ch_val[16].ydata);//更新每个通道数据
    ch_curve[16]->attach(ui->qwtPlot);//将通道数据写入qwt

    ch_curve[17]->setSamples(qwt_ch_val[17].xdata,qwt_ch_val[17].ydata);//更新每个通道数据
    ch_curve[17]->attach(ui->qwtPlot);//将通道数据写入qwt

    ch_curve[18]->setSamples(qwt_ch_val[18].xdata,qwt_ch_val[18].ydata);//更新每个通道数据
    ch_curve[18]->attach(ui->qwtPlot);//将通道数据写入qwt
///////////////////////////////////////////////////////////////////
    ch_curve[19]->setSamples(qwt_ch_val[19].xdata,qwt_ch_val[19].ydata);//更新每个通道数据
    ch_curve[19]->attach(ui->qwtPlot);//将通道数据写入qwt

    ch_curve[20]->setSamples(qwt_ch_val[20].xdata,qwt_ch_val[20].ydata);//更新每个通道数据
    ch_curve[20]->attach(ui->qwtPlot);//将通道数据写入qwt
    */
   showwave();
}

void waveshow::showwave()
{
    if(showflag==true)
    {
        for(uint16_t i=0;i<MAX_DATA_CH;i++)
        {
            if(qwt_ch_val[i].update_flag==true)
            {
                ch_curve[i]->setSamples(qwt_ch_val[i].xdata,qwt_ch_val[i].ydata);//更新每个通道数据
                ch_curve[i]->attach(ui->qwtPlot);//将通道数据写入qwt
            }
            ui->qwtPlot->replot();
        }

        ch_max_cnt=0;
        for(uint16_t i=0;i<MAX_DATA_CH;i++)
        {
          if(qwt_ch_val[i].update_flag)
          {
            ch_max_cnt=MAX(ch_max_cnt,qwt_ch_val[i].cnt);
            //qwt_ch_val[i].update_flag=false;
          }
        }

        static double scale_temp=0;
        QwtInterval tempIntervalX = ui->qwtPlot->axisScaleDiv(QwtPlot::xBottom).interval();
        double Xmax = tempIntervalX.maxValue();
        double Xmin = tempIntervalX.minValue();
        double scale=Xmax-Xmin;


        if(ch_max_cnt<Xmax)
        {
          ui->qwtPlot->setAxisScale(QwtPlot::xBottom,0,Zoom_X_Scale,(int64_t)(scale/5));
        }
        else
        {
          ui->qwtPlot->setAxisScale(QwtPlot::xBottom,ch_max_cnt-Zoom_X_Scale,ch_max_cnt,(int64_t)(scale/5));
        }


        if(ch_max_cnt>MAX_CH_LEN)
        {
          on_bt_clearshow_clicked();
        }
    }
    else
    {
        /*
        for(uint16_t m=0;m<MAX_DATA_CH;m++)
        {
            qwt_ch_val[m].xdata.resize(0);
            qwt_ch_val[m].ydata.resize(0);
            //qwt_ch_val[m].xdata[0]=0;
            //qwt_ch_val[m].ydata[0]=0;
            qwt_ch_val[m].update_flag=false;
            qwt_ch_val[m].cnt=0;
            ch_curve[m]->setSamples(qwt_ch_val[m].xdata,qwt_ch_val[m].ydata);//更新每个通道数据
            ch_curve[m]->attach(ui->qwtPlot);//将通道数据写入qwt
        }
        ui->qwtPlot->replot();
        */
    }
}


void waveshow::wave_init()
{
    //wavetimer->start(1);
}


void waveshow::receiveMessage(const QString& info)
{
  //ui->textEdit->insertPlainText(info+'\n');
  //ui->textEdit->moveCursor(QTextCursor::End);

  //int a=(int)QThread::currentThreadId();
  //QString LogInfo=QString::number(a,10);
  //qDebug() << "waveshow " <<"threadID : "<<LogInfo+"waveshow";

}


void waveshow::progress(int present)
{
  //ui->textEdit->insertPlainText(QString::number(present,10)+'\n');
  //ui->textEdit->moveCursor(QTextCursor::End);
}

//点击图例，显示相应的曲线
void waveshow::showItemChecked(const QVariant &itemInfo, bool on)
{
     QwtPlotItem *plotItem =ui->qwtPlot->infoToItem(itemInfo);//获取曲线
     if(plotItem)   plotItem->setVisible(on);//根据图例选择状态，设置曲线隐藏还是显示
     ui->qwtPlot->replot();
     //qDebug() <<itemInfo.data();
}

void waveshow::show_redraw(int present)
{

    //QDateTime local(QDateTime::currentDateTime());
    //QString localTime = local.toString("yyyy-MM-dd:hh:mm:ss:zzz");
    //qDebug() << localTime;
/*
    int i = 0;
    int j = 0;
    if(iActDataLen<MAX_DATA_SIZE)  iActDataLen++;
    else
    {
        for(i=0;i<MAX_DATA_CH;i++)    ch_val[i][MAX_DATA_SIZE-1]=ch_val[i][0];//赋最新数据

        for(i=0;i<(MAX_DATA_CH);i++)
        {
          for(j=0;j<(MAX_DATA_SIZE-1);j++) ch_val[i][j] =  ch_val[i][j+1];
        }
    }
    for(i=0;i<MAX_DATA_CH;i++)    ch_curve[i]->setSamples(ch_time,ch_val[i],iActDataLen);//更新每个通道数据
    for(i=0;i<MAX_DATA_CH;i++)    ch_curve[i]->attach(ui->qwtPlot);//将通道数据写入qwt
    //ui->qwtPlot->replot();
*/
}




void waveshow::on_bt_zoominx_clicked()
{
   /*
   const QwtScaleDiv xs=ui->qwtPlot->axisScaleDiv(QwtPlot::xBottom);
   const QwtScaleDiv ys=ui->qwtPlot->axisScaleDiv(QwtPlot::yLeft);
   ui->qwtPlot->setAxisScale(QwtPlot::xBottom,	 //固定放大x轴
                            0,
                            2*xs.upperBound());

   ui->qwtPlot->setAxisScale(QwtPlot::yLeft,	//固定y轴
                            ys.lowerBound(),
                            ys.upperBound());
   */
    QwtInterval tempIntervalX = ui->qwtPlot->axisScaleDiv(QwtPlot::xBottom).interval();
    double Xmax = tempIntervalX.maxValue();
    double Xmin = tempIntervalX.minValue();
    double scale=Xmax-Xmin;

    if(showflag==true)
    {
        Zoom_X_Scale=uint64_t(scale*0.50f);
        if(Zoom_X_Scale<=5)  Zoom_X_Scale=5;
        ui->qwtPlot->setAxisScale(QwtPlot::xBottom,Xmin,Xmax+Zoom_X_Scale,(int64_t)(Zoom_X_Scale/5));
    }
    else
    {
        Zoom_X_Scale=uint64_t(scale*0.50f);
        if(Zoom_X_Scale<=5)  Zoom_X_Scale=5;
        ui->qwtPlot->setAxisScale(QwtPlot::xBottom,Xmax-Zoom_X_Scale,Xmax,(int64_t)(Zoom_X_Scale/5));
    }
    ui->qwtPlot->replot();


    QString str="X_Scale:"+QString::number(Zoom_X_Scale);
    ui->qwtshow->setPlainText(str);
}

void waveshow::on_bt_zoomoutx_clicked()
{
    /*
    const QwtScaleDiv xs=ui->qwtPlot->axisScaleDiv(QwtPlot::xBottom);
    const QwtScaleDiv ys=ui->qwtPlot->axisScaleDiv(QwtPlot::yLeft);
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,	 //固定放大x轴
                             0,
                             0.5*xs.upperBound());
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,	//固定y轴
                             ys.lowerBound(),
                             ys.upperBound());
    */
    QwtInterval tempIntervalX = ui->qwtPlot->axisScaleDiv(QwtPlot::xBottom).interval();
    double Xmax = tempIntervalX.maxValue();
    double Xmin = tempIntervalX.minValue();
    double scale=Xmax-Xmin;


    if(showflag==true)
    {
        Zoom_X_Scale=uint64_t(scale*2.00f);
        if(Zoom_X_Scale>=100000)  Zoom_X_Scale=100000;
        ui->qwtPlot->setAxisScale(QwtPlot::xBottom,Xmin,Xmax+Zoom_X_Scale,(int64_t)(Zoom_X_Scale/5));
    }
    else
    {
        Zoom_X_Scale=uint64_t(scale*2.00f);
        if(Zoom_X_Scale>=100000)  Zoom_X_Scale=100000;
        ui->qwtPlot->setAxisScale(QwtPlot::xBottom,Xmax-Zoom_X_Scale,Xmax,(int64_t)(Zoom_X_Scale/5));
    }
    ui->qwtPlot->replot();

    QString str="X_Scale:"+QString::number(Zoom_X_Scale);
    ui->qwtshow->setPlainText(str);
}





void waveshow::on_bt_zoominy_clicked()
{
    QwtInterval tempIntervalY = ui->qwtPlot->axisScaleDiv(QwtPlot::yLeft).interval();
    double Ymax = tempIntervalY.maxValue();
    double Ymin = tempIntervalY.minValue();
    double scale=Ymax-Ymin;
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,Ymin+0.25*scale,Ymax-0.25*scale);
    ui->qwtPlot->replot();
}

void waveshow::on_bt_zoomouty_clicked()
{
    QwtInterval tempIntervalY = ui->qwtPlot->axisScaleDiv(QwtPlot::yLeft).interval();
    double Ymax = tempIntervalY.maxValue();
    double Ymin = tempIntervalY.minValue();
    double scale=Ymax-Ymin;
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,Ymin-0.25*scale,Ymax+0.25*scale);
    ui->qwtPlot->replot();
}



void waveshow::select_axis_zoomer( QRectF )
{
    const QwtScaleDiv xs = ui->qwtPlot->axisScaleDiv( QwtPlot::xBottom );
    const QwtScaleDiv ys = ui->qwtPlot->axisScaleDiv( QwtPlot::yLeft );
    ui->qwtPlot->setAxisScale( QwtPlot::xBottom,	 //固定x轴
                  xs.lowerBound(),
                  xs.upperBound() );
    ui->qwtPlot->setAxisScale( QwtPlot::yLeft,	     //固定y轴
                  ys.lowerBound(),
                  ys.upperBound());

    ui->qwtPlot->replot();
}



void waveshow::on_bt_zoominy_zero_clicked()
{
    QwtInterval tempIntervalY = ui->qwtPlot->axisScaleDiv(QwtPlot::yLeft).interval();
    double Ymax = tempIntervalY.maxValue();
    double Ymin = tempIntervalY.minValue();
    double scale=Ymax-Ymin;
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,-0.5*scale,0.5*scale);
    ui->qwtPlot->replot();
}

void waveshow::on_bt_clearshow_clicked()
{
    for(uint16_t m=0;m<MAX_DATA_CH;m++)
    {
        qwt_ch_val[m].xdata.resize(0);
        qwt_ch_val[m].ydata.resize(0);
        //qwt_ch_val[m].xdata[0]=0;
        //qwt_ch_val[m].ydata[0]=0;
        qwt_ch_val[m].update_flag=false;
        qwt_ch_val[m].cnt=0;
        ch_curve[m]->setSamples(qwt_ch_val[m].xdata,qwt_ch_val[m].ydata);//更新每个通道数据
        ch_curve[m]->attach(ui->qwtPlot);//将通道数据写入qwt
    }
    ui->qwtPlot->replot();

    Zoom_X_Scale=MAX_DATA_SIZE;
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,0.0,MAX_DATA_SIZE);
    ui->qwtPlot->replot();
}


/*
void waveshow::on_bt_showall_clicked()
{
    if(ui->bt_showall->text()=="显示全部曲线")
    {
        for (uint16_t i=0;i<MAX_DATA_CH;i++)
        {
           ch_curve[i]->setVisible(true);
        }
        ui->bt_showall->setText("关闭全部曲线");
    }
    else
    {
        for (uint16_t i=0;i<MAX_DATA_CH;i++)
        {
           ch_curve[i]->setVisible(false);
        }
        ui->bt_showall->setText("显示全部曲线");
    }
    ui->qwtPlot->replot();
}
*/

//当窗口出发关闭事件后，将信号发射出去
void waveshow::closeEvent(QCloseEvent *)
{
    emit exitwindows();
}

void waveshow::on_bt_startshow_clicked()
{
    if(ui->bt_startshow->text()=="停止显示")
    {
        showflag=false;
        ui->bt_startshow->setText("开始显示");
        //Zoom_X_Scale=MAX_DATA_SIZE;
        ui->bt_startshow->setIcon(QIcon(":/image/noshow.png"));
        wavetimer->stop();
    }
    else
    {
        showflag=true;
        ui->bt_startshow->setText("停止显示");
        Zoom_X_Scale=MAX_DATA_SIZE;
        ui->bt_startshow->setIcon(QIcon(":/image/show.png"));
        wavetimer->start();
    }
}



QVector<double> vectors,vector2;

void waveshow::readcsvfile() //读取csv
{
     //vectors.resize(10);
     vectors.append(0.01);
     vectors.append(0.32);
     vectors.append(0.33);
     QFile csvFile("D:/001.csv");
     QStringList csvList;
     QString csv;
     csvList.clear();
     if (csvFile.open(QIODevice::ReadWrite)) //对csv文件进行读写操作
     {
        QTextStream stream(&csvFile);
        while(!stream.atEnd())
        {
           csvList.push_back(stream.readLine()); //保存到List当中
        }
        //qDebug()<<csvList;
        qDebug()<<qwt_ch_val[0].ydata.size();

        for(uint32_t i=0;i<vectors.size();i++)
        {
          csv+=QString::number(vectors[i],10,2);
          csv+=',';
        }

        QString fileCSVHeader = tr("Sampling point time,") + tr("Relative altitude,") + tr("absolute altitude,")
            + tr("latitude,") + tr("longitude,") + tr("temperature,") + tr("humidity,") + tr("CO,")
            + tr("SO2,") + tr("NO2,") + tr("O3,") + tr("CO2,") + tr("PM1,") + tr("PM25,") + tr("PM10\n");

        csvFile.write(csv.toLocal8Bit().data());
        //csvFile.write(fileCSVHeader.toLocal8Bit().data());
        csvFile.close();
     }
     else
     {
         QMessageBox::about(NULL, "csv文件", "未打开该文件！");
     }


     /*
       int i = 0;
       Q_FOREACH(QString str, csvList)   //遍历List
      {
       i = i + 1;
       QStringList valsplit = str.split(","); //分隔字符串
       if(i > 2)
       {
        //得到深度、声速、温度
        QString depth = valsplit[0];
        QString sonicvelocity = valsplit[1];
        QString temperature = valsplit[2];
        double depthvalue = depth.toDouble();
        double sonicvalue = sonicvelocity.toDouble();
        double tempvalue = temperature.toDouble();
        //Q//MessageBox::warning(NULL, "dd", QString::number(tempvalue));
        QPointF point;
        point.setX(depthvalue);
        point.setY(sonicvalue);
        QPointF point2;
        point2.setX(depthvalue);
        point2.setY(tempvalue + 1510);
        //vectors.append(point);
        //vector2.append(point2);
       }
      }
      */

/*
     QDateTime current_date_time = QDateTime::currentDateTime();
     QString current_time = current_date_time.toString("yyyy-MM-dd-hh-mm-ss");

     QDir tempDir;
     //QString currentDir=tempDir.currentPath();//临时保存程序当前路径
     QString runPath = QCoreApplication::applicationDirPath();       //获取exe路径
     //qDebug()<<runPath;
     QString filePath=runPath+"/数据导出";
     if(!tempDir.exists(filePath))//如果filePath路径不存在，创建它
     {
        //qDebug()<<QObject::tr("不存在该路径")<<endl;
        tempDir.mkpath(filePath);
     }

     //QString fileName="数据导出.csv";
     QString fileName=current_time+".csv";
     QFile csvFile;// = new QFile;
     //将程序的执行路径设置到filePath下
     tempDir.setCurrent(filePath);
     //qDebug()<<tempDir.currentPath();
     if(csvFile.exists(fileName))//检查filePath路径下是否存在文件fileName,如果停止操作。
     {
         //qDebug()<<QObject::tr("文件存在");
         //return ;
     }
     else
     {
         csvFile.setFileName(fileName);//此时，路径下没有fileName文件，使用下面代码在当前路径下创建文件
         if(!csvFile.open(QIODevice::WriteOnly|QIODevice::Text))
         {
            //qDebug()<<QObject::tr("打开失败");
         }
         csvFile.close();
     }

     //QFile csvFile("D:/001.csv");
     QStringList csvList;
     QString csv[MAX_DATA_CH];
     csvList.clear();
     if(csvFile.open(QIODevice::ReadWrite)) //对csv文件进行读写操作
     {
        //QTextStream stream(&csvFile);
        //while(!stream.atEnd())
        //{
        //   csvList.push_back(stream.readLine()); //保存到List当中
        //}
        //qDebug()<<csvList;
        //qDebug()<<qwt_ch_val[0].ydata.size();

        for(uint16_t j=0;j<MAX_DATA_CH;j++)
        {
           csv[j]=ch_curve[j]->title().text()+',';
           for(uint32_t i=0;i<qwt_ch_val[j].ydata.size();i++)
           {
             csv[j]+=QString::number(qwt_ch_val[j].ydata[i],10,2);
             if(i!=qwt_ch_val[j].ydata.size()-1)  csv[j]+=',';
           }
           csv[j]+='\n';
           csvFile.write(csv[j].toLocal8Bit().data());
        }
        csvFile.close();
     }
     else
     {
         QMessageBox::about(NULL, "csv文件", "未打开该文件！");
     }

     /////////////////////////////////////////
    wavetimer->stop();
    for(uint16_t m=0;m<MAX_DATA_CH;m++)
    {
        qwt_ch_val[m].xdata.resize(0);
        qwt_ch_val[m].ydata.resize(0);
        //qwt_ch_val[m].xdata[0]=0;
        //qwt_ch_val[m].ydata[0]=0;
        qwt_ch_val[m].update_flag=false;
        qwt_ch_val[m].cnt=0;
        ch_curve[m]->setSamples(qwt_ch_val[m].xdata,qwt_ch_val[m].ydata);//更新每个通道数据
        ch_curve[m]->attach(ui->qwtPlot);//将通道数据写入qwt
    }
    ui->qwtPlot->replot();


    //四个参数的作用分别是：指定父窗口、设置对话框标题、指定默认打开的目录路径和设置文件类型的过滤器
    QString fileName = QFileDialog::getOpenFileName(this,tr("数据导入"),"F:",tr("逗号分隔值(*csv);;""文本文件(*txt)"));
    //qDebug()<<"filename : "<<fileName;
    QStringList csvList;
    QString csv[MAX_DATA_CH];
    csvList.clear();
    QFile csvFile;// = new QFile;
    csvFile.setFileName(fileName);//将程序的执行路径设置到filePath下

    if(csvFile.open(QIODevice::ReadWrite)) //对csv文件进行读写操作
    {
       QTextStream stream(&csvFile);
       while(!stream.atEnd())
       {
          csvList.push_back(stream.readLine()); //保存到List当中
       }
       //qDebug()<<csvList<<endl;
       //qDebug()<<csvList.size()<<endl;
       //qDebug()<<qwt_ch_val[0].ydata.size();
    }
    csvFile.close();

    if(csvFile.open(QIODevice::ReadWrite)) //对csv文件进行读写操作
    {
        //QTextStream *out=new QTextStream(&csvFile);//文本流
        //QStringList tempOption=out->readAll().split('\n');//每行以\n区分
        QStringList tempOption;
        QTextStream out(&csvFile);
        while(!out.atEnd())
        {
           tempOption.push_back(out.readLine()); //保存到List当中
        }


        for(int i=0;i<tempOption.count();i++)
        {
          QStringList tempbar = tempOption.at(i).split(",");//一行中的单元格以，区分
          qwt_ch_val[i].cnt=tempbar.size()-1;//数据长度
          //qDebug()<<qwt_ch_val[i].cnt;
          for (uint32_t k=1;k<tempbar.size();k++)//第一个数据为标号
          {
             qwt_ch_val[i].ydata.append(tempbar.at(k).toDouble());
             qwt_ch_val[i].xdata.append(qwt_ch_val[i].cnt);
             qwt_ch_val[i].cnt++;
          }
          //qDebug()<<qwt_ch_val[i].ydata.size()<<qwt_ch_val[i].ydata;
        }
    }
    csvFile.close();//操作完成后记得关闭文件

    ch_max_cnt=0;
    for(uint16_t i=0;i<MAX_DATA_CH;i++)
    {
        ch_max_cnt=MAX(ch_max_cnt,qwt_ch_val[i].cnt);
    }
    qDebug()<<ch_max_cnt;

    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,0.0,ch_max_cnt);//设置数据长度为横坐标端点值
    for(uint16_t m=0;m<MAX_DATA_CH;m++)
    {
        ch_curve[m]->setSamples(qwt_ch_val[m].xdata,qwt_ch_val[m].ydata);//更新每个通道数据
        ch_curve[m]->attach(ui->qwtPlot);//将通道数据写入qwt
    }
    ui->qwtPlot->replot();
     */
}

void waveshow::on_bt_export_clicked()
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_time = current_date_time.toString("yyyy-MM-dd-hh-mm-ss");

    QDir tempDir;
    QString runPath = QCoreApplication::applicationDirPath();       //获取exe路径
    QString filePath=runPath+"/数据导出";
    if(!tempDir.exists(filePath))//如果filePath路径不存在，创建它
    {
       tempDir.mkpath(filePath);
    }

    QString fileName=current_time+".csv";
    QFile csvFile;// = new QFile;
    tempDir.setCurrent(filePath);//将程序的执行路径设置到filePath下
    if(csvFile.exists(fileName))//检查filePath路径下是否存在文件fileName,如果停止操作。
    {
        //qDebug()<<QObject::tr("文件存在");
    }
    else
    {
        csvFile.setFileName(fileName);//此时，路径下没有fileName文件，使用下面代码在当前路径下创建文件
        if(!csvFile.open(QIODevice::WriteOnly|QIODevice::Text))
        {
           //qDebug()<<QObject::tr("打开失败");
        }
        csvFile.close();
    }
    QString csv[MAX_DATA_CH];
    if(csvFile.open(QIODevice::ReadWrite)) //对csv文件进行读写操作
    {
       for(uint16_t j=0;j<MAX_DATA_CH;j++)
       {
          csv[j]=ch_curve[j]->title().text()+',';
          for(uint32_t i=0;i<qwt_ch_val[j].ydata.size();i++)
          {
            csv[j]+=QString::number(qwt_ch_val[j].ydata[i],10,2);
            if(i!=qwt_ch_val[j].ydata.size()-1)  csv[j]+=',';
          }
          csv[j]+='\n';
          csvFile.write(csv[j].toLocal8Bit().data());
       }
       csvFile.close();



       //qDebug()<<qwt_ch_val[0].cnt;

       ui->qwtshow->appendPlainText("导出成功，文件路径为："+filePath+"/"+fileName);
    }
    else
    {
        //QMessageBox::about(NULL, "csv文件", "未打开该文件！");
        ui->qwtshow->appendPlainText("导出失败，请检查上次是否保存未完成");
    }
}

void waveshow::on_bt_import_clicked()
{
    showflag=false;
    wavetimer->stop();
    for(uint16_t m=0;m<MAX_DATA_CH;m++)
    {
        qwt_ch_val[m].xdata.resize(0);
        qwt_ch_val[m].ydata.resize(0);
        //qwt_ch_val[m].xdata[0]=0;
        //qwt_ch_val[m].ydata[0]=0;
        qwt_ch_val[m].update_flag=false;
        qwt_ch_val[m].cnt=0;
        ch_curve[m]->setSamples(qwt_ch_val[m].xdata,qwt_ch_val[m].ydata);//更新每个通道数据
        ch_curve[m]->attach(ui->qwtPlot);//将通道数据写入qwt
    }
    ui->qwtPlot->replot();
    QString runPath = QCoreApplication::applicationDirPath();       //获取exe路径


    //四个参数的作用分别是：指定父窗口、设置对话框标题、指定默认打开的目录路径和设置文件类型的过滤器
    QString fileName = QFileDialog::getOpenFileName(this,tr("数据导入"),runPath,tr("逗号分隔值(*csv);;""文本文件(*txt)"));
    QString csv[MAX_DATA_CH];
    QFile csvFile;// = new QFile;
    csvFile.setFileName(fileName);//将程序的执行路径设置到filePath下
    if(csvFile.open(QIODevice::ReadWrite)) //对csv文件进行读写操作
    {
        //QTextStream *out=new QTextStream(&csvFile);//文本流
        //QStringList tempOption=out->readAll().split('\n');//每行以\n区分
        QTextStream out(&csvFile);
        QStringList tempOption;
        while(!out.atEnd())
        {
            tempOption.push_back(out.readLine());//保存到List当中
        }
        //QStringList tempOption=out.readAll().split("\n");//每行以\n区分

        for(int i=0;i<tempOption.count();i++)
        {
          QStringList tempbar = tempOption.at(i).split(",");//一行中的单元格以，区分
          //qwt_ch_val[i].cnt=tempbar.size()-1;//数据长度
          for (uint32_t k=1;k<tempbar.size();k++)//第一个数据为标号
          {
             qwt_ch_val[i].ydata.append(tempbar.at(k).toDouble());
             qwt_ch_val[i].xdata.append(qwt_ch_val[i].cnt);
             qwt_ch_val[i].cnt++;//数据长度
          }
          //qDebug()<<"len"<<qwt_ch_val[i].ydata.size();
          //qDebug()<<"cnt"<<qwt_ch_val[i].cnt;
        }
    }
    csvFile.close();//操作完成后记得关闭文件
    ch_max_cnt=0;
    for(uint16_t i=0;i<MAX_DATA_CH;i++)
    {
        ch_max_cnt=MAX(ch_max_cnt,qwt_ch_val[i].cnt);
    }
    //qDebug()<<ch_max_cnt;
    //qDebug()<<qwt_ch_val[0].cnt;
    //ui->qwtPlot->setAxisAutoScale(QwtPlot::xBottom,true);
    ui->qwtPlot->setAxisAutoScale(QwtPlot::yLeft,true);
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,0.0,ch_max_cnt);//设置数据长度为横坐标端点值
    for(uint16_t m=0;m<MAX_DATA_CH;m++)
    {
        ch_curve[m]->setSamples(qwt_ch_val[m].xdata,qwt_ch_val[m].ydata);//更新每个通道数据
        ch_curve[m]->attach(ui->qwtPlot);//将通道数据写入qwt
    }
    ui->qwtPlot->replot();
}
