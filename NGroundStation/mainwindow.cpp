#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Headfile.h"

sendandreceive *serialsendandreceive;
serialsetup *usbsetup;
waveshow *wave;
flightstate *state;
SetParameter *setpara;
remotectrl *remote;
calibration *cal;

QString bt_switch_text;
QSerialPort* serialPort=nullptr;
bool serialflag=false;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
    setFixedSize(this->width(),this->height());                     // 禁止拖动窗口大小


    this->setWindowTitle("无名创新地面站NGS");                        //设置主界面名称
    setWindowIcon(QIcon(":/image/Logo.png"));                       //设置主界面LOGO

    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor(233,235,254));
    this->setPalette(palette);

     connect(ui->Frame, SIGNAL(triggered()), this, SLOT(help()));
     connect(ui->web, SIGNAL(triggered()), this, SLOT(surfWeb()));
     connect(ui->blog, SIGNAL(triggered()), this, SLOT(blog()));
     connect(ui->store, SIGNAL(triggered()), this, SLOT(store()));
     connect(ui->update, SIGNAL(triggered()), this, SLOT(update()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //serialsendandreceive->close();
    //usbsetup->close();
    //wave->close();
    //state->close();
    //setpara->close();
    //delete serialsendandreceive;
    //delete usbsetup;
    //delete wave;
    //delete state;
    //delete setpara;
}

void MainWindow::redraw()
{
    //if(wave!=nullptr)  delete wave;

    this->show();
}


void MainWindow::on_bt_serialsetup_clicked()
{
//    //串口设置ui
    usbsetup =new serialsetup();
    usbsetup->setWindowFlags(Qt::Window);
    usbsetup->setAttribute(Qt::WA_DeleteOnClose);
    connect(usbsetup,SIGNAL(exitwindows()),this,SLOT(redraw()));//当点击子界面时，调用主界面的redraw()函数

    usbsetup->serialinit();
    usbsetup->show();
    this->hide();
}


void MainWindow::on_bt_switch_clicked()
{
    if(ui->bt_switch->text()=="打开串口")
    {
        if(serial_init())
        {
          ui->bt_switch->setText("关闭串口");
          ui->bt_switch->setIcon(QIcon(":/image/opencom.png"));
          serialflag=true;
        }
    }
    else
    {
        serialPort->close();
        delete serialPort;
        ui->bt_switch->setText("打开串口");
        ui->bt_switch->setIcon(QIcon(":/image/closecom.png"));
        serialflag=false;
    }
}









void MainWindow::on_bt_sendandreceive_clicked()
{  


    //基本收发ui
    serialsendandreceive=new sendandreceive();
    serialsendandreceive->setAttribute(Qt::WA_DeleteOnClose);
    serialsendandreceive->setWindowFlags(Qt::Window);
    connect(serialsendandreceive,SIGNAL(exitwindows()),this,SLOT(redraw()));//当点击子界面时，调用主界面的redraw()函数

    connect(serialsendandreceive,SIGNAL(senddata(QString)),this,SLOT(readdata(QString)));
    serialsendandreceive->setWindowFlag(Qt::WindowStaysOnTopHint);//窗口置顶
    serialsendandreceive->show();
    this->hide();
}





void MainWindow::readdata(QString data)
{
    //ui->txtb_message->setText(data);
}


QString MainWindow::GetCPUManID()
{
    QString cpu_id = "";
    QProcess p(0);
    p.start("wmic CPU get ProcessorID");
    p.waitForStarted();
    p.waitForFinished();
    cpu_id = QString::fromLocal8Bit(p.readAllStandardOutput());
    cpu_id = cpu_id.remove("ProcessorId").trimmed();
    //qDebug()<<cpu_id;
    return cpu_id;
}

void MainWindow::on_bt_datawave_clicked()
{
    //波形显示ui
    wave=new waveshow();
    wave->setWindowFlags(Qt::Window);
    wave->setAttribute(Qt::WA_DeleteOnClose);
    connect(wave,SIGNAL(exitwindows()),this,SLOT(redraw()));//当点击子界面时，调用主界面的redraw()函数
    //connect(this,SIGNAL(showwave()),wave,SLOT(showwave()));
    //wave->wave_init();
    wave->show();
    this->hide();
}

void MainWindow::on_bt_flightstate_clicked()
{
    //飞控状态ui
    state=new flightstate();
    state->setWindowFlags(Qt::Window);
    state->setAttribute(Qt::WA_DeleteOnClose);
    connect(state,SIGNAL(exitwindows()),this,SLOT(redraw()));//当点击子界面时，调用主界面的redraw()函数

    state->show();
    this->hide();
}


QString msg_data;
void MainWindow::getthreaddata(QByteArray data)
{ 
    static QByteArray data_temp;
    data_temp+=data;
    //msg_data=QString(data_temp);
/***********************************************
    QByteArray bufferHex = data.toHex();//转换成hex
    bufferHex=bufferHex.toUpper();//转换成大写
    data_temp+=bufferHex;
    msg_data=QString(data_temp);
    for(int i = msg_data.count(); i > 0; i = i-2)
    {
      msg_data.insert(i," ");
    }
***********************************************/
    if(data_temp.length()>1000)
    {
       data_temp.clear();
       //ui->txtb_message->clear();
    }
}




void MainWindow::on_bt_parameter_clicked()
{
    //飞控参数ui
    setpara=new SetParameter();
    setpara->setWindowFlags(Qt::Window);
    setpara->setAttribute(Qt::WA_DeleteOnClose);
    connect(setpara,SIGNAL(exitwindows()),this,SLOT(redraw()));//当点击子界面时，调用主界面的redraw()函数
    connect(this,SIGNAL(redrawparameter(uint8_t)),setpara,SLOT(setparameter_show(uint8_t)));
    connect(this,SIGNAL(sendcheck(uint8_t)),setpara,SLOT(getcheck(uint8_t)));
    setpara->show();
    this->hide();
}



/***********************************************************************************************/

bool MainWindow::serial_init()
{
    serialPort = new QSerialPort(this);//打开串口
    serialPort->setPortName(serialconfig.serialname);//设置串口号
    if(serialPort->isOpen())//如果串口已打开，立刻关闭
    {
        serialPort->close();
    }

    if(!serialPort->open(QIODevice::ReadWrite))
    {
        QMessageBox::critical(this,"提示","串口异常，请您先配置串口，再打开串口！！！",QMessageBox::Yes,QMessageBox::Yes);
        return false;
    }

    serialPort->setBaudRate(serialconfig.baudrate);
    switch(serialconfig.parity)//设置数据校验形式
    {
        case 0:
            serialPort->setParity(QSerialPort::NoParity);//无校验
            break;
        case 1:
            serialPort->setParity(QSerialPort::OddParity);//奇校验
            break;
        case 2:
            serialPort->setParity(QSerialPort::EvenParity);//偶校验
            break;
        default:
            serialPort->setParity(QSerialPort::NoParity);
            break;
    }

    switch(serialconfig.dataBit)//设置数据位
    {
        case 0:
            serialPort->setDataBits(QSerialPort::Data8);
            break;
        case 1:
            serialPort->setDataBits(QSerialPort::Data7);
            break;
        case 2:
            serialPort->setDataBits(QSerialPort::Data6);
            break;
        case 3:
            serialPort->setDataBits(QSerialPort::Data5);
            break;
        default:
            serialPort->setDataBits(QSerialPort::Data8);
            break;
    }

    switch(serialconfig.stopBit)//设置停止位
    {
        case 0:
            serialPort->setStopBits(QSerialPort::OneStop);
            break;
        case 1:
            serialPort->setStopBits(QSerialPort::OneAndHalfStop);
            break;
        case 2:
            serialPort->setStopBits(QSerialPort::TwoStop);
            break;
        default:
            serialPort->setStopBits(QSerialPort::OneStop);
            break;
    }
    serialPort->setFlowControl(QSerialPort::NoFlowControl); //无流控
    serialPort->setReadBufferSize(BufferSizeMax);                    //数据缓冲区长度65536*2
    //int a=(int)QThread::currentThreadId();
    //QString LogInfo=QString::number(a,10);
    //qDebug() << "thread初始化 " <<"threadID : "<<LogInfo;
    //serialPort->write("无名创新");
    //connect(serialPort,SIGNAL(readyRead()),this,SLOT(NCLinkDataGet()),Qt::DirectConnection);//有数据就读
    connect(serialPort,SIGNAL(readyRead()),this,SLOT(NCLinkDataGet()));//有数据就读
    return true;
}


union
{
    unsigned char floatByte[4];
    float floatValue;
}FloatUnion;


void MainWindow::Float2Byte(float *FloatValue, unsigned char *Byte, unsigned char Subscript)
{
    FloatUnion.floatValue = (float)2;

    if(FloatUnion.floatByte[0] == 0)
    {//小端模式
        FloatUnion.floatValue = *FloatValue;

        Byte[Subscript]     = FloatUnion.floatByte[0];
        Byte[Subscript + 1] = FloatUnion.floatByte[1];
        Byte[Subscript + 2] = FloatUnion.floatByte[2];
        Byte[Subscript + 3] = FloatUnion.floatByte[3];
    }
    else
    {//大端模式
        FloatUnion.floatValue = *FloatValue;

        Byte[Subscript]     = FloatUnion.floatByte[3];
        Byte[Subscript + 1] = FloatUnion.floatByte[2];
        Byte[Subscript + 2] = FloatUnion.floatByte[1];
        Byte[Subscript + 3] = FloatUnion.floatByte[0];
    }
}


void MainWindow::Byte2Float(unsigned char *Byte, unsigned char Subscript, float *FloatValue)
{
    FloatUnion.floatByte[0] = Byte[Subscript];
    FloatUnion.floatByte[1] = Byte[Subscript + 1];
    FloatUnion.floatByte[2] = Byte[Subscript + 2];
    FloatUnion.floatByte[3] = Byte[Subscript + 3];

    *FloatValue = FloatUnion.floatValue;
}


void MainWindow::NCLinkDataGet()
{
    qint64 byteLen = serialPort->bytesAvailable();

    if(byteLen < 0)
    {
        return;
    }
    receivedData=serialPort->readAll();
    for(int i=0;i<receivedData.size();i++)
    {
        NCLinkDataPrase(receivedData.at(i));
    }

}


void MainWindow::NCLinkDataPrase(uint8_t data)
{
    thr_cnt4++;
    static uint8_t RxBuffer[BufferSizeMax];//1024
    static uint8_t _data_len = 0,_data_cnt = 0;
    static uint8_t state = 0;
    if(state==0&&data==NCLink_Head0)//帧头1
    {
        state=1;
        RxBuffer[0]=data;
    }
    else if(state==1&&data==NCLink_Head1)//帧头2
    {
        state=2;
        RxBuffer[1]=data;
    }
    else if(state==2&&data<0XF1)//功能字
    {
        state=3;
        RxBuffer[2]=data;
    }
    else if(state==3&&data<100)//数据长度
    {
        state = 4;
        RxBuffer[3]=data;
        _data_len = data;
        _data_cnt = 0;
    }
    else if(state==4&&_data_len>0)//数据区
    {
        _data_len--;
        RxBuffer[4+_data_cnt++]=data;
        if(_data_len==0)
        state = 5;
    }
    else if(state==5)//校验和
    {
        state = 6;
        RxBuffer[4+_data_cnt++]=data;
    }
    else if(state==6&&data==NCLink_End0)//帧尾0
    {
        state = 7;
        RxBuffer[4+_data_cnt++]=data;
    }
    else if(state==7&&data==NCLink_End1)//帧尾1
    {
        state = 0;
        RxBuffer[4+_data_cnt]=data;
        dataSort(RxBuffer,_data_cnt+5);
    }
    else
    {
        state = 0;
        //qDebug()<<"default";
    }
}



void MainWindow::dataSort(uint8_t *data_buf,uint8_t num)
{
    uint8_t sum = 0;
    for(uint8_t i=0;i<(num-3);i++)     sum^=(*(data_buf+i));//计算校验和
    if(!(sum==*(data_buf+num-3)))
    {
        //qDebug()<<"default";
        return;
    }
    if(!(*(data_buf)==NCLink_Head0 && *(data_buf+1)==NCLink_Head1))//帧头校验
    {
        //qDebug()<<"default";
        return;
    }
    if(!(*(data_buf+num-2)==NCLink_End0 && *(data_buf+num-1)==NCLink_End1))//帧尾校验
    {
        //qDebug()<<"default";
        return;
    }
    //qDebug()<<"sum check okay"<<"FC:"<<data_buf[2]<<"LE:"<<data_buf[3];

    if(*(data_buf+2)==0X01)//基本姿态数据
    {

       roll =0.01f*((int16_t)(*(data_buf+4)<<8)|*(data_buf+5));
       pitch=0.01f*((int16_t)(*(data_buf+6)<<8)|*(data_buf+7));
       yaw  =0.01f*((int16_t)(*(data_buf+8)<<8)|*(data_buf+9));

       roll_gyro =0.01f*((int32_t)(*(data_buf+10)<<24)|(*(data_buf+11)<<16)|(*(data_buf+12)<<8)|*(data_buf+13));
       pitch_gyro =0.01f*((int32_t)(*(data_buf+14)<<24)|(*(data_buf+15)<<16)|(*(data_buf+16)<<8)|*(data_buf+17));
       yaw_gyro =0.01f*((int32_t)(*(data_buf+18)<<24)|(*(data_buf+19)<<16)|(*(data_buf+20)<<8)|*(data_buf+21));

       imu_temp =0.01f*((int16_t)(*(data_buf+22)<<8)|*(data_buf+23));
       vbat=0.01f*((int16_t)(*(data_buf+24)<<8)|*(data_buf+25));

       fly_mode=*(data_buf+26);
       armed_flag=*(data_buf+27);

       if(waveclear_cnt<flightstate_wave_num_max)
       {
           flightstate_ch_val[0][waveclear_cnt]=pitch;
           flightstate_ch_val[1][waveclear_cnt]=roll;
           flightstate_ch_val[2][waveclear_cnt]=yaw;
           waveclear_cnt++;
       }
       else
       {
           for(uint16_t i=0;i<3;i++)
           {
             for(uint16_t j=0;j<flightstate_wave_num_max-1;j++)  flightstate_ch_val[i][j] =  flightstate_ch_val[i][j+1];
           }
           flightstate_ch_val[0][flightstate_wave_num_max-1]=pitch;
           flightstate_ch_val[1][flightstate_wave_num_max-1]=roll;
           flightstate_ch_val[2][flightstate_wave_num_max-1]=yaw;
       }


       if(showflag==true)
       {
           //mutex.lock();
           qwt_ch_val[0].xdata.append(qwt_ch_val[0].cnt);
           qwt_ch_val[0].ydata.append(pitch);
           qwt_ch_val[0].cnt++;
           qwt_ch_val[0].update_flag=true;

           qwt_ch_val[1].xdata.append(qwt_ch_val[1].cnt);
           qwt_ch_val[1].ydata.append(roll);
           qwt_ch_val[1].cnt++;
           qwt_ch_val[1].update_flag=true;

           qwt_ch_val[2].xdata.append(qwt_ch_val[2].cnt);
           qwt_ch_val[2].ydata.append(yaw);
           qwt_ch_val[2].cnt++;
           qwt_ch_val[2].update_flag=true;

           qwt_ch_val[12].xdata.append(qwt_ch_val[12].cnt);
           qwt_ch_val[12].ydata.append(pitch_gyro);
           qwt_ch_val[12].cnt++;
           qwt_ch_val[12].update_flag=true;

           qwt_ch_val[13].xdata.append(qwt_ch_val[13].cnt);
           qwt_ch_val[13].ydata.append(roll_gyro);
           qwt_ch_val[13].cnt++;
           qwt_ch_val[13].update_flag=true;

           qwt_ch_val[14].xdata.append(qwt_ch_val[14].cnt);
           qwt_ch_val[14].ydata.append(yaw_gyro);
           qwt_ch_val[14].cnt++;
           qwt_ch_val[14].update_flag=true;
           //mutex.unlock();
           //emit showwave();
       }

    }
    else if(*(data_buf+2)==0X02)//IMU原始数据
    {
        ax=(int16_t)(*(data_buf+4)<<8)|*(data_buf+5);
        ay=(int16_t)(*(data_buf+6)<<8)|*(data_buf+7);
        az=(int16_t)(*(data_buf+8)<<8)|*(data_buf+9);
        gx=(int16_t)(*(data_buf+10)<<8)|*(data_buf+11);
        gy=(int16_t)(*(data_buf+12)<<8)|*(data_buf+13);
        gz=(int16_t)(*(data_buf+14)<<8)|*(data_buf+15);
        mx=(int16_t)(*(data_buf+16)<<8)|*(data_buf+17);
        my=(int16_t)(*(data_buf+18)<<8)|*(data_buf+19);
        mz=(int16_t)(*(data_buf+20)<<8)|*(data_buf+21);
    }
    else if(*(data_buf+2)==0X03)//遥控器各个通道数据
    {
        rc[0]=(int16_t)(*(data_buf+4)<<8)|*(data_buf+5);
        rc[1]=(int16_t)(*(data_buf+6)<<8)|*(data_buf+7);
        rc[2]=(int16_t)(*(data_buf+8)<<8)|*(data_buf+9);
        rc[3]=(int16_t)(*(data_buf+10)<<8)|*(data_buf+11);
        rc[4]=(int16_t)(*(data_buf+12)<<8)|*(data_buf+13);
        rc[5]=(int16_t)(*(data_buf+14)<<8)|*(data_buf+15);
        rc[6]=(int16_t)(*(data_buf+16)<<8)|*(data_buf+17);
        rc[7]=(int16_t)(*(data_buf+18)<<8)|*(data_buf+19);
    }
    else if(*(data_buf+2)==0X04)//GPS定位数据
    {
        gps_lng=0.0000001f*((int32_t)(*(data_buf+4)<<24)|(*(data_buf+5)<<16)|(*(data_buf+6)<<8)|*(data_buf+7));
        gps_lat=0.0000001f*((int32_t)(*(data_buf+8)<<24)|(*(data_buf+9)<<16)|(*(data_buf+10)<<8)|*(data_buf+11));
        gps_alt=0.001f*((int32_t)(*(data_buf+12)<<24)|(*(data_buf+13)<<16)|(*(data_buf+14)<<8)|*(data_buf+15));
        gps_pdop=(int16_t)(*(data_buf+16)<<8)|*(data_buf+17);
        gps_state=*(data_buf+18);
        gps_satellite_num=*(data_buf+19);
    }
    else if(*(data_buf+2)==0X05)//GPS速度数据
    {
        lat_pos_obs =0.01f*((int32_t)(*(data_buf+4)<<24)|(*(data_buf+5)<<16)|(*(data_buf+6)<<8)|*(data_buf+7));
        lng_pos_obs =0.01f*((int32_t)(*(data_buf+8)<<24)|(*(data_buf+9)<<16)|(*(data_buf+10)<<8)|*(data_buf+11));
        lat_vel_obs =0.01f*((int32_t)(*(data_buf+12)<<24)|(*(data_buf+13)<<16)|(*(data_buf+14)<<8)|*(data_buf+15));
        lng_vel_obs =0.01f*((int32_t)(*(data_buf+16)<<24)|(*(data_buf+17)<<16)|(*(data_buf+18)<<8)|*(data_buf+19));
        if(showflag==true)
        {
            //mutex.lock();
            qwt_ch_val[15].xdata.append(qwt_ch_val[15].cnt);
            qwt_ch_val[15].ydata.append(lat_pos_obs);
            qwt_ch_val[15].cnt++;
            qwt_ch_val[15].update_flag=true;

            qwt_ch_val[16].xdata.append(qwt_ch_val[16].cnt);
            qwt_ch_val[16].ydata.append(lng_pos_obs);
            qwt_ch_val[16].cnt++;
            qwt_ch_val[16].update_flag=true;

            qwt_ch_val[17].xdata.append(qwt_ch_val[17].cnt);
            qwt_ch_val[17].ydata.append(lat_vel_obs);
            qwt_ch_val[17].cnt++;
            qwt_ch_val[17].update_flag=true;

            qwt_ch_val[18].xdata.append(qwt_ch_val[18].cnt);
            qwt_ch_val[18].ydata.append(lng_vel_obs);
            qwt_ch_val[18].cnt++;
            qwt_ch_val[18].update_flag=true;
            //emit showwave();
        }
    }
    else if(*(data_buf+2)==0X06)//GPS速度数据
    {
        alt_obs_baro=0.01f*((int32_t)(*(data_buf+4)<<24)|(*(data_buf+5)<<16)|(*(data_buf+6)<<8)|*(data_buf+7));
        alt_obs_ult =0.01f*((int32_t)(*(data_buf+8)<<24)|(*(data_buf+9)<<16)|(*(data_buf+10)<<8)|*(data_buf+11));
        opt_vel_p   =0.01f*((int32_t)(*(data_buf+12)<<24)|(*(data_buf+13)<<16)|(*(data_buf+14)<<8)|*(data_buf+15));
        opt_vel_r   =0.01f*((int32_t)(*(data_buf+16)<<24)|(*(data_buf+17)<<16)|(*(data_buf+18)<<8)|*(data_buf+19));
        if(showflag==true)
        {
            //mutex.lock();
            qwt_ch_val[19].xdata.append(qwt_ch_val[19].cnt);
            qwt_ch_val[19].ydata.append(alt_obs_baro);
            qwt_ch_val[19].cnt++;
            qwt_ch_val[19].update_flag=true;

            qwt_ch_val[20].xdata.append(qwt_ch_val[20].cnt);
            qwt_ch_val[20].ydata.append(alt_obs_ult);
            qwt_ch_val[20].cnt++;
            qwt_ch_val[20].update_flag=true;
            //emit showwave();
        }

    }
    else if(*(data_buf+2)==0X07)
    {
        alt_pos_fus=0.01f*((int32_t)(*(data_buf+4)<<24)|(*(data_buf+5)<<16)|(*(data_buf+6)<<8)|*(data_buf+7));
        alt_vel_fus=0.01f*((int32_t)(*(data_buf+8)<<24)|(*(data_buf+9)<<16)|(*(data_buf+10)<<8)|*(data_buf+11));
        alt_accel_fus=0.01f*((int32_t)(*(data_buf+12)<<24)|(*(data_buf+13)<<16)|(*(data_buf+14)<<8)|*(data_buf+15));

        if(showflag==true)
        {
            qwt_ch_val[5].xdata.append(qwt_ch_val[5].cnt);
            qwt_ch_val[5].ydata.append(alt_pos_fus);
            qwt_ch_val[5].cnt++;
            qwt_ch_val[5].update_flag=true;

            qwt_ch_val[8].xdata.append(qwt_ch_val[8].cnt);
            qwt_ch_val[8].ydata.append(alt_vel_fus);
            qwt_ch_val[8].cnt++;
            qwt_ch_val[8].update_flag=true;

            qwt_ch_val[11].xdata.append(qwt_ch_val[11].cnt);
            qwt_ch_val[11].ydata.append(alt_accel_fus);
            qwt_ch_val[11].cnt++;
            qwt_ch_val[11].update_flag=true;
            //emit showwave();
        }

    }
    else if(*(data_buf+2)==0X08)
    {
        lat_pos_fusion   =0.01*((int32_t)(*(data_buf+4)<<24)|(*(data_buf+5)<<16)|(*(data_buf+6)<<8)|*(data_buf+7));
        lng_pos_fusion   =0.01*((int32_t)(*(data_buf+8)<<24)|(*(data_buf+9)<<16)|(*(data_buf+10)<<8)|*(data_buf+11));
        lat_vel_fusion   =0.01*((int32_t)(*(data_buf+12)<<24)|(*(data_buf+13)<<16)|(*(data_buf+14)<<8)|*(data_buf+15));
        lng_vel_fusion   =0.01*((int32_t)(*(data_buf+16)<<24)|(*(data_buf+17)<<16)|(*(data_buf+18)<<8)|*(data_buf+19));
        lat_accel_fusion =0.01*((int32_t)(*(data_buf+20)<<24)|(*(data_buf+21)<<16)|(*(data_buf+22)<<8)|*(data_buf+23));
        lng_accel_fusion =0.01*((int32_t)(*(data_buf+24)<<24)|(*(data_buf+25)<<16)|(*(data_buf+26)<<8)|*(data_buf+27));

        if(showflag==true)
        {
            qwt_ch_val[3].xdata.append(qwt_ch_val[3].cnt);
            qwt_ch_val[3].ydata.append(lat_pos_fusion);
            qwt_ch_val[3].cnt++;
            qwt_ch_val[3].update_flag=true;

            qwt_ch_val[4].xdata.append(qwt_ch_val[4].cnt);
            qwt_ch_val[4].ydata.append(lng_pos_fusion);
            qwt_ch_val[4].cnt++;
            qwt_ch_val[4].update_flag=true;

            qwt_ch_val[6].xdata.append(qwt_ch_val[6].cnt);
            qwt_ch_val[6].ydata.append(lat_vel_fusion);
            qwt_ch_val[6].cnt++;
            qwt_ch_val[6].update_flag=true;

            qwt_ch_val[7].xdata.append(qwt_ch_val[7].cnt);
            qwt_ch_val[7].ydata.append(lng_vel_fusion);
            qwt_ch_val[7].cnt++;
            qwt_ch_val[7].update_flag=true;

            qwt_ch_val[9].xdata.append(qwt_ch_val[9].cnt);
            qwt_ch_val[9].ydata.append(lat_accel_fusion);
            qwt_ch_val[9].cnt++;
            qwt_ch_val[9].update_flag=true;

            qwt_ch_val[10].xdata.append(qwt_ch_val[10].cnt);
            qwt_ch_val[10].ydata.append(lng_accel_fusion);
            qwt_ch_val[10].cnt++;
            qwt_ch_val[10].update_flag=true;
            //emit showwave();
        }
    }
    else if(*(data_buf+2)==0X09)
    {
        Byte2Float(data_buf,4,&userdata[0]);
        Byte2Float(data_buf,8,&userdata[1]);
        Byte2Float(data_buf,12,&userdata[2]);
        Byte2Float(data_buf,16,&userdata[3]);
        Byte2Float(data_buf,20,&userdata[4]);
        Byte2Float(data_buf,24,&userdata[5]);
        if(showflag==true)
        {
            for(uint16_t i=0;i<6;i++)
            {
                uint16_t j=i+21;
                qwt_ch_val[j].xdata.append(qwt_ch_val[j].cnt);
                qwt_ch_val[j].ydata.append(userdata[i]);
                qwt_ch_val[j].cnt++;
                qwt_ch_val[j].update_flag=true;
            }
            //emit showwave();
        }
    }
    else if(*(data_buf+2)==0X0A)                             //PID1-3
    {
        kp[0]=((int16_t)(*(data_buf+4)<<8)|*(data_buf+5));
        ki[0]=((int16_t)(*(data_buf+6)<<8)|*(data_buf+7));
        kd[0]=((int16_t)(*(data_buf+8)<<8)|*(data_buf+9));
        kp[1]=((int16_t)(*(data_buf+10)<<8)|*(data_buf+11));
        ki[1]=((int16_t)(*(data_buf+12)<<8)|*(data_buf+13));
        kd[1]=((int16_t)(*(data_buf+14)<<8)|*(data_buf+15));
        kp[2]=((int16_t)(*(data_buf+16)<<8)|*(data_buf+17));
        ki[2]=((int16_t)(*(data_buf+18)<<8)|*(data_buf+19));
        kd[2]=((int16_t)(*(data_buf+20)<<8)|*(data_buf+21));
        uint8_t ch=0x01;
        emit redrawparameter(ch);
    }
    else if(*(data_buf+2)==0X0B)                             //PID4-6
    {
        kp[3]=((int16_t)(*(data_buf+4)<<8)|*(data_buf+5));
        ki[3]=((int16_t)(*(data_buf+6)<<8)|*(data_buf+7));
        kd[3]=((int16_t)(*(data_buf+8)<<8)|*(data_buf+9));
        kp[4]=((int16_t)(*(data_buf+10)<<8)|*(data_buf+11));
        ki[4]=((int16_t)(*(data_buf+12)<<8)|*(data_buf+13));
        kd[4]=((int16_t)(*(data_buf+14)<<8)|*(data_buf+15));
        kp[5]=((int16_t)(*(data_buf+16)<<8)|*(data_buf+17));
        ki[5]=((int16_t)(*(data_buf+18)<<8)|*(data_buf+19));
        kd[5]=((int16_t)(*(data_buf+20)<<8)|*(data_buf+21));
        uint8_t ch=0x02;
        emit redrawparameter(ch);
    }
    else if(*(data_buf+2)==0X0C)                             //PID7-9
    {
        kp[6]=((int16_t)(*(data_buf+4)<<8)|*(data_buf+5));
        ki[6]=((int16_t)(*(data_buf+6)<<8)|*(data_buf+7));
        kd[6]=((int16_t)(*(data_buf+8)<<8)|*(data_buf+9));
        kp[7]=((int16_t)(*(data_buf+10)<<8)|*(data_buf+11));
        ki[7]=((int16_t)(*(data_buf+12)<<8)|*(data_buf+13));
        kd[7]=((int16_t)(*(data_buf+14)<<8)|*(data_buf+15));
        kp[8]=((int16_t)(*(data_buf+16)<<8)|*(data_buf+17));
        ki[8]=((int16_t)(*(data_buf+18)<<8)|*(data_buf+19));
        kd[8]=((int16_t)(*(data_buf+20)<<8)|*(data_buf+21));
        uint8_t ch=0x03;
        emit redrawparameter(ch);
    }
    else if(*(data_buf+2)==0X0D)                             //PID7-9
    {
        kp[9]=((int16_t)(*(data_buf+4)<<8)|*(data_buf+5));
        ki[9]=((int16_t)(*(data_buf+6)<<8)|*(data_buf+7));
        kd[9]=((int16_t)(*(data_buf+8)<<8)|*(data_buf+9));
        kp[10]=((int16_t)(*(data_buf+10)<<8)|*(data_buf+11));
        ki[10]=((int16_t)(*(data_buf+12)<<8)|*(data_buf+13));
        kd[10]=((int16_t)(*(data_buf+14)<<8)|*(data_buf+15));
        kp[11]=((int16_t)(*(data_buf+16)<<8)|*(data_buf+17));
        ki[11]=((int16_t)(*(data_buf+18)<<8)|*(data_buf+19));
        kd[11]=((int16_t)(*(data_buf+20)<<8)|*(data_buf+21));
        uint8_t ch=0x04;
        emit redrawparameter(ch);
    }
    else if(*(data_buf+2)==0X0E)                             //PID7-9
    {
        kp[12]=((int16_t)(*(data_buf+4)<<8)|*(data_buf+5));
        ki[12]=((int16_t)(*(data_buf+6)<<8)|*(data_buf+7));
        kd[12]=((int16_t)(*(data_buf+8)<<8)|*(data_buf+9));
        kp[13]=((int16_t)(*(data_buf+10)<<8)|*(data_buf+11));
        ki[13]=((int16_t)(*(data_buf+12)<<8)|*(data_buf+13));
        kd[13]=((int16_t)(*(data_buf+14)<<8)|*(data_buf+15));
        kp[14]=((int16_t)(*(data_buf+16)<<8)|*(data_buf+17));
        ki[14]=((int16_t)(*(data_buf+18)<<8)|*(data_buf+19));
        kd[14]=((int16_t)(*(data_buf+20)<<8)|*(data_buf+21));
        uint8_t ch=0x05;
        emit redrawparameter(ch);
    }
    else if(*(data_buf+2)==0X0F)                             //PID7-9
    {
        kp[15]=((int16_t)(*(data_buf+4)<<8)|*(data_buf+5));
        ki[15]=((int16_t)(*(data_buf+6)<<8)|*(data_buf+7));
        kd[15]=((int16_t)(*(data_buf+8)<<8)|*(data_buf+9));
        kp[16]=((int16_t)(*(data_buf+10)<<8)|*(data_buf+11));
        ki[16]=((int16_t)(*(data_buf+12)<<8)|*(data_buf+13));
        kd[16]=((int16_t)(*(data_buf+14)<<8)|*(data_buf+15));
        kp[17]=((int16_t)(*(data_buf+16)<<8)|*(data_buf+17));
        ki[17]=((int16_t)(*(data_buf+18)<<8)|*(data_buf+19));
        kd[17]=((int16_t)(*(data_buf+20)<<8)|*(data_buf+21));
        uint8_t ch=0x06;
        emit redrawparameter(ch);
    }
    else if(*(data_buf+2)==0X10)
    {
        targeheight=((int16_t)(*(data_buf+4)<<8)|*(data_buf+5));
        safeheight=((int16_t)(*(data_buf+6)<<8)|*(data_buf+7));
        safevbat=((int16_t)(*(data_buf+8)<<8)|*(data_buf+9));
        maxheight=((int16_t)(*(data_buf+10)<<8)|*(data_buf+11));
        maxradius=((int16_t)(*(data_buf+12)<<8)|*(data_buf+13));
        maxupvel=((int16_t)(*(data_buf+14)<<8)|*(data_buf+15));
        maxdownvel=((int16_t)(*(data_buf+16)<<8)|*(data_buf+17));
        maxhorvel=((int16_t)(*(data_buf+18)<<8)|*(data_buf+19));
        uint8_t ch=0x07;
        emit redrawparameter(ch);
    }
    else if(*(data_buf+2)==0X11)
    {
        gyro_auto_cal_flag=*(data_buf+4);
        Byte2Float(data_buf, 5,&gyro_x_raw);
        Byte2Float(data_buf, 9,&gyro_y_raw);
        Byte2Float(data_buf,13,&gyro_z_raw);
        Byte2Float(data_buf,17,&acce_x_raw);
        Byte2Float(data_buf,21,&acce_y_raw);
        Byte2Float(data_buf,25,&acce_z_raw);
        Byte2Float(data_buf,29,&mag_x_raw);
        Byte2Float(data_buf,33,&mag_y_raw);
        Byte2Float(data_buf,37,&mag_z_raw);
        //uint8_t ch=0x07;
        //emit redrawparameter(ch);
    }
    else if(*(data_buf+2)==0X12)
    {
        Byte2Float(data_buf,4, &gyro_x_offset);
        Byte2Float(data_buf,8, &gyro_y_offset);
        Byte2Float(data_buf,12,&gyro_z_offset);
        Byte2Float(data_buf,16,&acce_x_offset);
        Byte2Float(data_buf,20,&acce_y_offset);
        Byte2Float(data_buf,24,&acce_z_offset);
        Byte2Float(data_buf,28,&acce_x_scale);
        Byte2Float(data_buf,32,&acce_y_scale);
        Byte2Float(data_buf,36,&acce_z_scale);
        Byte2Float(data_buf,40,&mag_x_offset);
        Byte2Float(data_buf,44,&mag_y_offset);
        Byte2Float(data_buf,48,&mag_z_offset);
        Byte2Float(data_buf,52,&pitch_offset);
        Byte2Float(data_buf,56,&roll_offset);
        //uint8_t ch=0x07;
        //emit redrawparameter(ch);
    }

    else if(*(data_buf+2)==0XF0)
    {
       emit sendcheck(*(data_buf+4));
    }
}


void MainWindow::help()
{
   QString runPath = QCoreApplication::applicationDirPath();       //获取exe路径
   QString hglpName = "nc";
   //QString hglpPath = QString("%1/%2").arg(runPath).arg(hglpName);
   QString hglpPath = QString("%1").arg(runPath);
   //qDebug()<<hglpPath;
   QFile bfilePath(hglpPath);
   if(!bfilePath.exists())
   {
      return;
   }
   QString filePath = hglpPath+"/NCLink.xlsx";
   QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}


void MainWindow::surfWeb()
{
   QDesktopServices::openUrl(QUrl(QString("www.nameless.tech")));
}

void MainWindow::blog()
{
   QDesktopServices::openUrl(QUrl(QString("https://space.bilibili.com/67803559/video")));
   QDesktopServices::openUrl(QUrl(QString("https://zhuanlan.zhihu.com/namelesscotrun")));
   QDesktopServices::openUrl(QUrl(QString("https://blog.csdn.net/u011992534")));
   QDesktopServices::openUrl(QUrl(QString("https://wx.zsxq.com/dweb2/index/group/142181515582")));
}

void MainWindow::store()
{
   QDesktopServices::openUrl(QUrl(QString("https://shop348646912.taobao.com/?spm=2013.1.0.0.5cff440bqOr4yG")));
}

void MainWindow::update()
{
   QDesktopServices::openUrl(QUrl(QString("http://www.nameless.tech/down/NGroundStation.zip")));
}

void MainWindow::on_bt_remote_clicked()
{
    if(serialflag==false)
    {
      QMessageBox::critical(this,"提示","串口异常，请您先配置串口，再打开串口！！！",QMessageBox::Yes,QMessageBox::Yes);
    }
    else
    {
        //遥控ui
        remote=new remotectrl();
        remote->setWindowFlags(Qt::Window);
        remote->setAttribute(Qt::WA_DeleteOnClose);
        connect(remote,SIGNAL(exitwindows()),this,SLOT(redraw()));//当点击子界面时，调用主界面的redraw()函数
        connect(this,SIGNAL(sendcheck(uint8_t)),remote,SLOT(getcheck(uint8_t)));
        remote->show();
        this->hide();
    }
}

void MainWindow::on_bt_calibration_clicked()
{
    //串口设置ui
    cal =new calibration();
    cal->setWindowFlags(Qt::Window);
    cal->setAttribute(Qt::WA_DeleteOnClose);
    connect(cal,SIGNAL(exitwindows()),this,SLOT(redraw()));//当点击子界面时，调用主界面的redraw()函数
    connect(this,SIGNAL(sendcheck(uint8_t)),cal,SLOT(getcheck(uint8_t)));

    cal->show();
    this->hide();
}
