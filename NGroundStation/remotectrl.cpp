#include "Headfile.h"
#include "remotectrl.h"
#include "ui_remotectrl.h"

QTimer *timer_remote=nullptr;
int16_t channel[8]={0};
uint16_t channel_package[8]={0};
uint8_t flag_unlock=0,flag_takeoff=0;
double accuracy=0;
double accuracy_send_cnt=0,accuracy_rec_cnt=0;


remotectrl::remotectrl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::remotectrl)
{
    ui->setupUi(this);


    accuracy=0;
    accuracy_send_cnt=0;
    accuracy_rec_cnt=0;



    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);     // 禁止最大化按钮
    setFixedSize(this->width(),this->height());                     // 禁止拖动窗口大小
    this->setWindowTitle("虚拟遥控器");                               //设置主界面名称
    setWindowIcon(QIcon(":/image/remote.png"));                      //设置主界面LOGO
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor(233,235,254));//QColor(78,168,243)
    this->setPalette(palette);

    ui->lineEdit_front->setValidator(new QIntValidator(0, 100000, this));//限制数据输入范围
    ui->lineEdit_behind->setValidator(new QIntValidator(0, 100000, this));
    ui->lineEdit_left->setValidator(new QIntValidator(0, 100000, this));
    ui->lineEdit_right->setValidator(new QIntValidator(0, 100000, this));
    ui->lineEdit_up->setValidator(new QIntValidator(0, 100000, this));
    ui->lineEdit_down->setValidator(new QIntValidator(0, 100000, this));

    ui->order_front->setValidator(new QIntValidator(0, 100000, this));//限制数据输入范围
    ui->order_behind->setValidator(new QIntValidator(0, 100000, this));
    ui->order_left->setValidator(new QIntValidator(0, 100000, this));
    ui->order_right->setValidator(new QIntValidator(0, 100000, this));
    ui->order_up->setValidator(new QIntValidator(0, 100000, this));
    ui->order_down->setValidator(new QIntValidator(0, 100000, this));

    timer_remote = new QTimer(this);
    connect(timer_remote, SIGNAL(timeout()), this, SLOT(timer_event()));
    int period_cnt=20;
    timer_remote->start(period_cnt);

    connect(ui->customRocker_ch12, SIGNAL(moveAngle(double,double)), this, SLOT(sort_data12(double,double)),Qt::AutoConnection);//DirectConnection
    connect(ui->customRocker_ch34, SIGNAL(moveAngle(double,double)), this, SLOT(sort_data34(double,double)),Qt::AutoConnection);//DirectConnection

    connect(ui->bt_unlock, SIGNAL(checkedChanged(bool)), this, SLOT(sort_data_bt_unlock(bool)),Qt::AutoConnection);//DirectConnection
    connect(ui->bt_takeoff, SIGNAL(checkedChanged(bool)), this, SLOT(sort_data_bt_takeoff(bool)),Qt::AutoConnection);//DirectConnection
}

remotectrl::~remotectrl()
{
    delete ui;
}


//当窗口出发关闭事件后，将信号发射出去
void remotectrl::closeEvent(QCloseEvent *)
{
    emit exitwindows();
}


void remotectrl::on_checkBox_stateChanged(int arg1)
{
        if(ui->checkBox->isChecked())
        {
            timer_remote = new QTimer(this);
            connect(timer_remote, SIGNAL(timeout()), this, SLOT(timer_event()));
            int period_cnt=20;
            timer_remote->start(period_cnt);
        }
        else
        {
            timer_remote->stop();
            delete timer_remote;
        }
}

void remotectrl::sort_data12(double angle,double r)
{

    channel[0]=(int16_t)(r*(double)(cos((angle/180.0)*3.141593f)));
    channel[1]=(int16_t)(r*(double)(sin((angle/180.0)*3.141593f)));

    channel_package[0]=1500+500*channel[0]/132;
    channel_package[1]=1500+500*channel[1]/132;

    ui->ch0_pro->setValue(channel_package[0]);
    ui->ch1_pro->setValue(channel_package[1]);

    ui->ch0->setText(QString::number(channel_package[0]));
    ui->ch1->setText(QString::number(channel_package[1]));
}

void remotectrl::sort_data34(double angle,double r)
{

    channel[2]=(int16_t)(r*(double)(sin((angle/180.0)*3.141593f)));
    channel[3]=(int16_t)(r*(double)(cos((angle/180.0)*3.141593f)));

    channel_package[2]=1500+500*channel[2]/132;
    channel_package[3]=1500+500*channel[3]/132;

    ui->ch2_pro->setValue(channel_package[2]);
    ui->ch3_pro->setValue(channel_package[3]);

    ui->ch2->setText(QString::number(channel_package[2]));
    ui->ch3->setText(QString::number(channel_package[3]));
}



void remotectrl::sort_data_bt_unlock(bool flag)
{
    if(ui->bt_unlock->getChecked()==true) flag_unlock=1;//解锁
    else flag_unlock=0;//上锁
}

void remotectrl::sort_data_bt_takeoff(bool flag)
{
    if(ui->bt_takeoff->getChecked()==true) flag_takeoff=1;//一键起飞
    else flag_takeoff=0;//一键降落
}



void remotectrl::timer_event()
{
    if(ui->customRocker_ch12->pressed==false)
    {
        channel_package[0]=1500;
        channel_package[1]=1500;
        ui->ch0_pro->setValue(channel_package[0]);
        ui->ch1_pro->setValue(channel_package[1]);
        ui->ch0->setText(QString::number(channel_package[0]));
        ui->ch1->setText(QString::number(channel_package[1]));
    }

    if(ui->customRocker_ch34->pressed==false)
    {
        channel_package[2]=1500;
        channel_package[3]=1500;
        ui->ch2_pro->setValue(channel_package[2]);
        ui->ch3_pro->setValue(channel_package[3]);
        ui->ch2->setText(QString::number(channel_package[2]));
        ui->ch3->setText(QString::number(channel_package[3]));
    }

    channel[4]=ui->xslider1->value();
    channel[5]=ui->xslider2->value();
    channel[6]=ui->xslider3->value();
    channel[7]=ui->xslider4->value();

    ui->ch4_pro->setValue(channel[4]);
    ui->ch5_pro->setValue(channel[5]);
    ui->ch6_pro->setValue(channel[6]);
    ui->ch7_pro->setValue(channel[7]);

    ui->ch4->setText(QString::number(channel[4]));
    ui->ch5->setText(QString::number(channel[5]));
    ui->ch6->setText(QString::number(channel[6]));
    ui->ch7->setText(QString::number(channel[7]));

    channel_package[4]=channel[4];
    channel_package[5]=channel[5];
    channel_package[6]=channel[6];
    channel_package[7]=channel[7];

    Send_RC(channel_package[0],channel_package[1],channel_package[2],channel_package[3],
            channel_package[4],channel_package[5],channel_package[6],channel_package[7],
            flag_unlock,flag_takeoff);

    flag_unlock=2;//中间状态
    flag_takeoff=2;//中间状态
    accuracy_send_cnt++;
}


extern QSerialPort* serialPort;
void remotectrl::Send_RC(uint16_t ch0,uint16_t ch1,uint16_t ch2,uint16_t ch3,uint16_t ch4,uint16_t ch5,uint16_t ch6,uint16_t ch7,uint8_t unlock_flag,uint8_t takeoff_flag)//地面站指令发送RC数据
{
  uint8_t data_to_send[50];
  uint8_t sum = 0,i=0,cnt=0;
  uint16_t _temp;
  data_to_send[cnt++]=NCLink_Head1;
  data_to_send[cnt++]=NCLink_Head0;
  data_to_send[cnt++]=0x09;
  data_to_send[cnt++]=0;

  _temp = ch0;
  data_to_send[cnt++]=BYTE1(_temp);
  data_to_send[cnt++]=BYTE0(_temp);
  _temp = ch1;
  data_to_send[cnt++]=BYTE1(_temp);
  data_to_send[cnt++]=BYTE0(_temp);
  _temp = ch2;
  data_to_send[cnt++]=BYTE1(_temp);
  data_to_send[cnt++]=BYTE0(_temp);
  _temp = ch3;
  data_to_send[cnt++]=BYTE1(_temp);
  data_to_send[cnt++]=BYTE0(_temp);
  _temp = ch4;
  data_to_send[cnt++]=BYTE1(_temp);
  data_to_send[cnt++]=BYTE0(_temp);
  _temp = ch5;
  data_to_send[cnt++]=BYTE1(_temp);
  data_to_send[cnt++]=BYTE0(_temp);
  _temp = ch6;
  data_to_send[cnt++]=BYTE1(_temp);
  data_to_send[cnt++]=BYTE0(_temp);
  _temp = ch7;
  data_to_send[cnt++]=BYTE1(_temp);
  data_to_send[cnt++]=BYTE0(_temp);

  data_to_send[cnt++]=BYTE0(unlock_flag);
  data_to_send[cnt++]=BYTE0(takeoff_flag);

  data_to_send[3] = cnt-4;
  for(i=0;i<cnt;i++) sum ^= data_to_send[i];
  data_to_send[cnt++]=sum;
  data_to_send[cnt++]=NCLink_End0;
  data_to_send[cnt++]=NCLink_End1;
  if(serialflag) serialPort->write((char *)(data_to_send),cnt);
  else
  {
      QDateTime current_date_time = QDateTime::currentDateTime();
      QString current_time = current_date_time.toString("hh:mm:ss.zzz");
  }
}



void remotectrl::getcheck(uint8_t ch)
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_time = current_date_time.toString("hh:mm:ss.zzz");
    if(ch==0x11)
    {
       ui->msg->appendPlainText(current_time+"毫秒>飞控接收遥控器数据成功");
       accuracy_rec_cnt++;
       accuracy=(double)(100*accuracy_rec_cnt/accuracy_send_cnt);
       ui->progressArc->setValue(accuracy);
       if(accuracy_send_cnt>=1000)
       {
           accuracy_send_cnt=0;
           accuracy_rec_cnt=0;
       }
    }
    else if(ch==0x12)
    {
       ui->msg->appendPlainText(current_time+"毫秒>飞控接收位移数据成功");
    }
    ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);

}

void remotectrl::on_clear_clicked()
{
    ui->msg->clear();
    ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}



void remotectrl::Send_DIS(uint8_t move_flag,uint8_t move_order,uint16_t move_distance)//地面站指令发送移动距离数据
{
  uint8_t data_to_send[50];
  uint8_t sum = 0,i=0,cnt=0;
  uint16_t _temp;
  data_to_send[cnt++]=NCLink_Head1;
  data_to_send[cnt++]=NCLink_Head0;
  data_to_send[cnt++]=0x0A;
  data_to_send[cnt++]=0;


  data_to_send[cnt++]=BYTE0(move_flag);
  data_to_send[cnt++]=BYTE0(move_order);

  _temp = move_distance;
  data_to_send[cnt++]=BYTE1(_temp);
  data_to_send[cnt++]=BYTE0(_temp);

  data_to_send[3] = cnt-4;
  for(i=0;i<cnt;i++) sum ^= data_to_send[i];
  data_to_send[cnt++]=sum;
  data_to_send[cnt++]=NCLink_End0;
  data_to_send[cnt++]=NCLink_End1;
  if(serialflag) serialPort->write((char *)(data_to_send),cnt);
  else
  {
      QDateTime current_date_time = QDateTime::currentDateTime();
      QString current_time = current_date_time.toString("hh:mm:ss.zzz");
  }
}



void remotectrl::on_bt_send1_clicked()
{
  Send_DIS(0x00,ui->order_front->text().toUInt(),ui->lineEdit_front->text().toUInt());

  QDateTime current_date_time = QDateTime::currentDateTime();
  QString current_time = current_date_time.toString("hh:mm:ss.zzz");
  ui->msg->appendPlainText(current_time+"毫秒>地面站向前位移数据已发送");
  ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

void remotectrl::on_bt_send2_clicked()
{
  Send_DIS(0x01,ui->order_behind->text().toUInt(),ui->lineEdit_behind->text().toUInt());
  QDateTime current_date_time = QDateTime::currentDateTime();
  QString current_time = current_date_time.toString("hh:mm:ss.zzz");
  ui->msg->appendPlainText(current_time+"毫秒>地面站向后位移数据已发送");
  ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

void remotectrl::on_bt_send3_clicked()
{
  Send_DIS(0x02,ui->order_left->text().toUInt(),ui->lineEdit_left->text().toUInt());
  QDateTime current_date_time = QDateTime::currentDateTime();
  QString current_time = current_date_time.toString("hh:mm:ss.zzz");
  ui->msg->appendPlainText(current_time+"毫秒>地面站向左位移数据已发送");
  ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

void remotectrl::on_bt_send4_clicked()
{
  Send_DIS(0x03,ui->order_right->text().toUInt(),ui->lineEdit_right->text().toUInt());
  QDateTime current_date_time = QDateTime::currentDateTime();
  QString current_time = current_date_time.toString("hh:mm:ss.zzz");
  ui->msg->appendPlainText(current_time+"毫秒>地面站向右位移数据已发送");
  ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

void remotectrl::on_bt_send5_clicked()
{
  Send_DIS(0x04,ui->order_up->text().toUInt(),ui->lineEdit_up->text().toUInt());
  QDateTime current_date_time = QDateTime::currentDateTime();
  QString current_time = current_date_time.toString("hh:mm:ss.zzz");
  ui->msg->appendPlainText(current_time+"毫秒>地面站向上位移数据已发送");
  ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

void remotectrl::on_bt_send6_clicked()
{
  Send_DIS(0x05,ui->order_down->text().toUInt(),ui->lineEdit_down->text().toUInt());
  QDateTime current_date_time = QDateTime::currentDateTime();
  QString current_time = current_date_time.toString("hh:mm:ss.zzz");
  ui->msg->appendPlainText(current_time+"毫秒>地面站向下位移数据已发送");
  ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);

  accuracy=0;
  accuracy_send_cnt=0;
  accuracy_rec_cnt=0;
}

void remotectrl::on_manualsend_clicked()
{
    //timer_remote->stop();
    //delete timer_remote;
    if(ui->checkBox->isChecked())
    {
        ui->checkBox->setChecked(false);
    }
    timer_event();
}
