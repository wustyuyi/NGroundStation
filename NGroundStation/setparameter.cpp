#include "Headfile.h"
#include "setparameter.h"
#include "ui_setparameter.h"


QTimer *setparameter=nullptr;
SetParameter::SetParameter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetParameter)
{
    ui->setupUi(this);

    this->setWindowTitle("参数调试");
    setWindowIcon(QIcon(":/image/debug.png"));

    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor(233,235,254));//QColor(78,168,243)
    this->setPalette(palette);

    ui->kp1->setValidator(new QIntValidator(0, 100000, this));//限制数据输入范围
    ui->kp2->setValidator(new QIntValidator(0, 100000, this));
    ui->kp3->setValidator(new QIntValidator(0, 100000, this));
    ui->kp4->setValidator(new QIntValidator(0, 100000, this));
    ui->kp5->setValidator(new QIntValidator(0, 100000, this));
    ui->kp6->setValidator(new QIntValidator(0, 100000, this));
    ui->kp7->setValidator(new QIntValidator(0, 100000, this));
    ui->kp8->setValidator(new QIntValidator(0, 100000, this));
    ui->kp9->setValidator(new QIntValidator(0, 100000, this));
    ui->kp10->setValidator(new QIntValidator(0, 100000, this));
    ui->kp11->setValidator(new QIntValidator(0, 100000, this));
    ui->kp12->setValidator(new QIntValidator(0, 100000, this));
    ui->kp13->setValidator(new QIntValidator(0, 100000, this));
    ui->kp14->setValidator(new QIntValidator(0, 100000, this));
    ui->kp15->setValidator(new QIntValidator(0, 100000, this));
    ui->kp16->setValidator(new QIntValidator(0, 100000, this));
    ui->kp17->setValidator(new QIntValidator(0, 100000, this));
    ui->kp18->setValidator(new QIntValidator(0, 100000, this));

    ui->ki1->setValidator(new QIntValidator(0, 100000, this));//限制数据输入范围
    ui->ki2->setValidator(new QIntValidator(0, 100000, this));
    ui->ki3->setValidator(new QIntValidator(0, 100000, this));
    ui->ki4->setValidator(new QIntValidator(0, 100000, this));
    ui->ki5->setValidator(new QIntValidator(0, 100000, this));
    ui->ki6->setValidator(new QIntValidator(0, 100000, this));
    ui->ki7->setValidator(new QIntValidator(0, 100000, this));
    ui->ki8->setValidator(new QIntValidator(0, 100000, this));
    ui->ki9->setValidator(new QIntValidator(0, 100000, this));
    ui->ki10->setValidator(new QIntValidator(0, 100000, this));
    ui->ki11->setValidator(new QIntValidator(0, 100000, this));
    ui->ki12->setValidator(new QIntValidator(0, 100000, this));
    ui->ki13->setValidator(new QIntValidator(0, 100000, this));
    ui->ki14->setValidator(new QIntValidator(0, 100000, this));
    ui->ki15->setValidator(new QIntValidator(0, 100000, this));
    ui->ki16->setValidator(new QIntValidator(0, 100000, this));
    ui->ki17->setValidator(new QIntValidator(0, 100000, this));
    ui->ki18->setValidator(new QIntValidator(0, 100000, this));

    ui->kd1->setValidator(new QIntValidator(0, 100000, this));//限制数据输入范围
    ui->kd2->setValidator(new QIntValidator(0, 100000, this));
    ui->kd3->setValidator(new QIntValidator(0, 100000, this));
    ui->kd4->setValidator(new QIntValidator(0, 100000, this));
    ui->kd5->setValidator(new QIntValidator(0, 100000, this));
    ui->kd6->setValidator(new QIntValidator(0, 100000, this));
    ui->kd7->setValidator(new QIntValidator(0, 100000, this));
    ui->kd8->setValidator(new QIntValidator(0, 100000, this));
    ui->kd9->setValidator(new QIntValidator(0, 100000, this));
    ui->kd10->setValidator(new QIntValidator(0, 100000, this));
    ui->kd11->setValidator(new QIntValidator(0, 100000, this));
    ui->kd12->setValidator(new QIntValidator(0, 100000, this));
    ui->kd13->setValidator(new QIntValidator(0, 100000, this));
    ui->kd14->setValidator(new QIntValidator(0, 100000, this));
    ui->kd15->setValidator(new QIntValidator(0, 100000, this));
    ui->kd16->setValidator(new QIntValidator(0, 100000, this));
    ui->kd17->setValidator(new QIntValidator(0, 100000, this));
    ui->kd18->setValidator(new QIntValidator(0, 100000, this));

    ui->targeheight->setValidator(new QIntValidator(0, 100000, this));
    ui->safeheight->setValidator(new QIntValidator(0, 100000, this));
    ui->safevbat->setValidator(new QIntValidator(0, 100000, this));
    ui->maxheight->setValidator(new QIntValidator(0, 100000, this));
    ui->maxradius->setValidator(new QIntValidator(0, 100000, this));

    setparameter = new QTimer(this);
    connect(setparameter, SIGNAL(timeout()), this, SLOT(timer_setparameter_show()));
    setparameter->start(50);//20HZ
}

SetParameter::~SetParameter()
{
    delete ui;
}


//当窗口出发关闭事件后，将信号发射出去
void SetParameter::closeEvent(QCloseEvent *)
{
    emit exitwindows();
}

void SetParameter::setparameter_show(uint8_t ch)
{
    ui->kp1->setText(QString::number(kp[0]));
    ui->kp2->setText(QString::number(kp[1]));
    ui->kp3->setText(QString::number(kp[2]));
    ui->kp4->setText(QString::number(kp[3]));
    ui->kp5->setText(QString::number(kp[4]));
    ui->kp6->setText(QString::number(kp[5]));
    ui->kp7->setText(QString::number(kp[6]));
    ui->kp8->setText(QString::number(kp[7]));
    ui->kp9->setText(QString::number(kp[8]));
    ui->kp10->setText(QString::number(kp[9]));
    ui->kp11->setText(QString::number(kp[10]));
    ui->kp12->setText(QString::number(kp[11]));
    ui->kp13->setText(QString::number(kp[12]));
    ui->kp14->setText(QString::number(kp[13]));
    ui->kp15->setText(QString::number(kp[14]));
    ui->kp16->setText(QString::number(kp[15]));
    ui->kp17->setText(QString::number(kp[16]));
    ui->kp18->setText(QString::number(kp[17]));

    ui->ki1->setText(QString::number(ki[0]));
    ui->ki2->setText(QString::number(ki[1]));
    ui->ki3->setText(QString::number(ki[2]));
    ui->ki4->setText(QString::number(ki[3]));
    ui->ki5->setText(QString::number(ki[4]));
    ui->ki6->setText(QString::number(ki[5]));
    ui->ki7->setText(QString::number(ki[6]));
    ui->ki8->setText(QString::number(ki[7]));
    ui->ki9->setText(QString::number(ki[8]));
    ui->ki10->setText(QString::number(ki[9]));
    ui->ki11->setText(QString::number(ki[10]));
    ui->ki12->setText(QString::number(ki[11]));
    ui->ki13->setText(QString::number(ki[12]));
    ui->ki14->setText(QString::number(ki[13]));
    ui->ki15->setText(QString::number(ki[14]));
    ui->ki16->setText(QString::number(ki[15]));
    ui->ki17->setText(QString::number(ki[16]));
    ui->ki18->setText(QString::number(ki[17]));

    ui->kd1->setText(QString::number(kd[0]));
    ui->kd2->setText(QString::number(kd[1]));
    ui->kd3->setText(QString::number(kd[2]));
    ui->kd4->setText(QString::number(kd[3]));
    ui->kd5->setText(QString::number(kd[4]));
    ui->kd6->setText(QString::number(kd[5]));
    ui->kd7->setText(QString::number(kd[6]));
    ui->kd8->setText(QString::number(kd[7]));
    ui->kd9->setText(QString::number(kd[8]));
    ui->kd10->setText(QString::number(kd[9]));
    ui->kd11->setText(QString::number(kd[10]));
    ui->kd12->setText(QString::number(kd[11]));
    ui->kd13->setText(QString::number(kd[12]));
    ui->kd14->setText(QString::number(kd[13]));
    ui->kd15->setText(QString::number(kd[14]));
    ui->kd16->setText(QString::number(kd[15]));
    ui->kd17->setText(QString::number(kd[16]));
    ui->kd18->setText(QString::number(kd[17]));

    ui->targeheight->setText(QString::number(targeheight));
    ui->safeheight->setText(QString::number(safeheight));
    ui->safevbat->setText(QString::number(safevbat));
    ui->maxheight->setText(QString::number(maxheight));
    ui->maxradius->setText(QString::number(maxradius));
    ui->maxupvel->setText(QString::number(maxupvel));
    ui->maxdownvel->setText(QString::number(maxdownvel));
    ui->maxhorvel->setText(QString::number(maxhorvel));

    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_time = current_date_time.toString("hh:mm:ss.zzz");
    if(ch==1)
    {
       ui->msg->appendPlainText(current_time+"毫秒>>PID1-PID3读取成功");
    }
    else if(ch==2)
    {
       ui->msg->appendPlainText(current_time+"毫秒>>PID4-PID6读取成功");
    }
    else if(ch==3)
    {
       ui->msg->appendPlainText(current_time+"毫秒>>PID7-PID9读取成功");
    }
    else if(ch==4)
    {
       ui->msg->appendPlainText(current_time+"毫秒>>PID10-PID12读取成功");
    }
    else if(ch==5)
    {
       ui->msg->appendPlainText(current_time+"毫秒>>PID13-PID15读取成功");
    }
    else if(ch==6)
    {
       ui->msg->appendPlainText(current_time+"毫秒>>PID16-PID18读取成功");
    }
    else if(ch==7)
    {
       ui->msg->appendPlainText(current_time+"毫秒>>读取其它参数成功");
    }
    ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}
void SetParameter::timer_setparameter_show()
{

}


void SetParameter::getcheck(uint8_t ch)
{
    uint16_t kp[3]={0},ki[3]={0},kd[3]={0};
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_time = current_date_time.toString("hh:mm:ss.zzz");
    if(ch==0x0A)
    {
       ui->msg->appendPlainText(current_time+"毫秒>>飞控返回校验:PID1-PID3写入成功");

       kp[0]=ui->kp4->text().toUInt();
       ki[0]=ui->ki4->text().toUInt();
       kd[0]=ui->kd4->text().toUInt();

       kp[1]=ui->kp5->text().toUInt();
       ki[1]=ui->ki5->text().toUInt();
       kd[1]=ui->kd5->text().toUInt();

       kp[2]=ui->kp6->text().toUInt();
       ki[2]=ui->ki6->text().toUInt();
       kd[2]=ui->kd6->text().toUInt();
       Send_PID(2,kp[0],ki[0],kd[0],
                     kp[1],ki[1],kd[1],
                     kp[2],ki[2],kd[2]);

    }
    else if(ch==0x0B)
    {
       ui->msg->appendPlainText(current_time+"毫秒>>飞控返回校验:PID4-PID6写入成功");
       kp[0]=ui->kp7->text().toUInt();
       ki[0]=ui->ki7->text().toUInt();
       kd[0]=ui->kd7->text().toUInt();

       kp[1]=ui->kp8->text().toUInt();
       ki[1]=ui->ki8->text().toUInt();
       kd[1]=ui->kd8->text().toUInt();

       kp[2]=ui->kp9->text().toUInt();
       ki[2]=ui->ki9->text().toUInt();
       kd[2]=ui->kd9->text().toUInt();
       Send_PID(3,kp[0],ki[0],kd[0],
                     kp[1],ki[1],kd[1],
                     kp[2],ki[2],kd[2]);
    }
    else if(ch==0x0C)
    {
       ui->msg->appendPlainText(current_time+"毫秒>>飞控返回校验:PID7-PID9写入成功");
       kp[0]=ui->kp10->text().toUInt();
       ki[0]=ui->ki10->text().toUInt();
       kd[0]=ui->kd10->text().toUInt();

       kp[1]=ui->kp11->text().toUInt();
       ki[1]=ui->ki11->text().toUInt();
       kd[1]=ui->kd11->text().toUInt();

       kp[2]=ui->kp12->text().toUInt();
       ki[2]=ui->ki12->text().toUInt();
       kd[2]=ui->kd12->text().toUInt();
       Send_PID(4,kp[0],ki[0],kd[0],
                     kp[1],ki[1],kd[1],
                     kp[2],ki[2],kd[2]);
    }
    else if(ch==0x0D)
    {
       ui->msg->appendPlainText(current_time+"毫秒>>飞控返回校验:PID10-PID12写入成功");
       kp[0]=ui->kp13->text().toUInt();
       ki[0]=ui->ki13->text().toUInt();
       kd[0]=ui->kd13->text().toUInt();

       kp[1]=ui->kp14->text().toUInt();
       ki[1]=ui->ki14->text().toUInt();
       kd[1]=ui->kd14->text().toUInt();

       kp[2]=ui->kp15->text().toUInt();
       ki[2]=ui->ki15->text().toUInt();
       kd[2]=ui->kd15->text().toUInt();
       Send_PID(5,kp[0],ki[0],kd[0],
                  kp[1],ki[1],kd[1],
                  kp[2],ki[2],kd[2]);
    }
    else if(ch==0x0E)
    {
       ui->msg->appendPlainText(current_time+"毫秒>>飞控返回校验:PID13-PID15写入成功");
       kp[0]=ui->kp16->text().toUInt();
       ki[0]=ui->ki16->text().toUInt();
       kd[0]=ui->kd16->text().toUInt();

       kp[1]=ui->kp17->text().toUInt();
       ki[1]=ui->ki17->text().toUInt();
       kd[1]=ui->kd17->text().toUInt();

       kp[2]=ui->kp18->text().toUInt();
       ki[2]=ui->ki18->text().toUInt();
       kd[2]=ui->kd18->text().toUInt();
       Send_PID(6,kp[0],ki[0],kd[0],
                  kp[1],ki[1],kd[1],
                  kp[2],ki[2],kd[2]);
    }
    else if(ch==0x0F)
    {
       ui->msg->appendPlainText(current_time+"毫秒>>飞控返回校验:PID16-PID18写入成功");
    }
    else if(ch==0x10)
    {
       ui->msg->appendPlainText(current_time+"毫秒>>飞控返回校验:其它参数写入成功");
    }
    ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

extern QSerialPort* serialPort;


#define NCLink_Head0 0xFF
#define NCLink_Head1 0xFC
#define NCLink_End0  0xA1
#define NCLink_End1  0xA2
void SetParameter::Send_Cmd_Check(uint8_t cmd,uint8_t data)//地面站指令发送
{
  uint8_t data_to_send[50];
  uint8_t sum = 0,i=0,len=0;
  len=1;
  data_to_send[0]=NCLink_Head1;
  data_to_send[1]=NCLink_Head0;
  data_to_send[2]=cmd;
  data_to_send[3]=len;
  data_to_send[4]=data;
  for(i=0;i<5;i++)
    sum ^= data_to_send[i];
  data_to_send[5]=sum;
  data_to_send[6]=NCLink_End0;
  data_to_send[7]=NCLink_End1;
  if(serialflag) serialPort->write((char *)(data_to_send),8);
  else
  {
      QDateTime current_date_time = QDateTime::currentDateTime();
      QString current_time = current_date_time.toString("hh:mm:ss.zzz");
      ui->msg->appendPlainText(current_time+"毫秒>>读写串口失败，请检查串口配置！！！");
      ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
  }
}

void SetParameter::Send_PID(uint8_t group,uint16_t kp1,uint16_t ki1,uint16_t kd1,
                                          uint16_t kp2,uint16_t ki2,uint16_t kd2,
                                          uint16_t kp3,uint16_t ki3,uint16_t kd3)//地面站指令发送
{
  uint8_t data_to_send[50];
  uint8_t sum = 0,i=0,cnt=0;
  uint16_t _temp;
  data_to_send[cnt++]=NCLink_Head1;
  data_to_send[cnt++]=NCLink_Head0;
  data_to_send[cnt++]=0x02+group-1;
  data_to_send[cnt++]=0;

  _temp = kp1;
  data_to_send[cnt++]=BYTE1(_temp);
  data_to_send[cnt++]=BYTE0(_temp);
  _temp = ki1;
  data_to_send[cnt++]=BYTE1(_temp);
  data_to_send[cnt++]=BYTE0(_temp);
  _temp = kd1;
  data_to_send[cnt++]=BYTE1(_temp);
  data_to_send[cnt++]=BYTE0(_temp);

  _temp = kp2;
  data_to_send[cnt++]=BYTE1(_temp);
  data_to_send[cnt++]=BYTE0(_temp);
  _temp = ki2;
  data_to_send[cnt++]=BYTE1(_temp);
  data_to_send[cnt++]=BYTE0(_temp);
  _temp = kd2;
  data_to_send[cnt++]=BYTE1(_temp);
  data_to_send[cnt++]=BYTE0(_temp);

  _temp = kp3;
  data_to_send[cnt++]=BYTE1(_temp);
  data_to_send[cnt++]=BYTE0(_temp);
  _temp = ki3;
  data_to_send[cnt++]=BYTE1(_temp);
  data_to_send[cnt++]=BYTE0(_temp);
  _temp = kd3;
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
      ui->msg->appendPlainText(current_time+"毫秒>>读写串口失败，请检查串口配置！！！");
      ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
  }
}


void SetParameter::Send_Parameter(uint8_t group,
                                  uint16_t targeheight,
                                  uint16_t safeheight,
                                  uint16_t safevbat,
                                  uint16_t maxheight,
                                  uint16_t maxradius,
                                  uint16_t maxupvel,
                                  uint16_t maxdownvel,
                                  uint16_t maxhorvel)//地面站指令发送
{
  uint8_t data_to_send[50];
  uint8_t sum = 0,i=0,cnt=0;
  uint16_t _temp;
  data_to_send[cnt++]=NCLink_Head1;
  data_to_send[cnt++]=NCLink_Head0;
  data_to_send[cnt++]=0x08+group-1;
  data_to_send[cnt++]=0;

  _temp = targeheight;
  data_to_send[cnt++]=BYTE1(_temp);
  data_to_send[cnt++]=BYTE0(_temp);

  _temp = safeheight;
  data_to_send[cnt++]=BYTE1(_temp);
  data_to_send[cnt++]=BYTE0(_temp);

  _temp = safevbat;
  data_to_send[cnt++]=BYTE1(_temp);
  data_to_send[cnt++]=BYTE0(_temp);

  _temp = maxheight;
  data_to_send[cnt++]=BYTE1(_temp);
  data_to_send[cnt++]=BYTE0(_temp);

  _temp = maxradius;
  data_to_send[cnt++]=BYTE1(_temp);
  data_to_send[cnt++]=BYTE0(_temp);

  _temp = maxupvel;
  data_to_send[cnt++]=BYTE1(_temp);
  data_to_send[cnt++]=BYTE0(_temp);

  _temp = maxdownvel;
  data_to_send[cnt++]=BYTE1(_temp);
  data_to_send[cnt++]=BYTE0(_temp);

  _temp = maxhorvel;
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
      ui->msg->appendPlainText(current_time+"毫秒>>读写串口失败，请检查串口配置！！！");
      ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
  }
}


void SetParameter::on_readall_clicked()
{
  Send_Cmd_Check(0x01,0x01);
}

void SetParameter::on_defaultall_clicked()
{
  Send_Cmd_Check(0x01,0x02);
}

void SetParameter::on_writeall_clicked()
{
    //QString period=ui->send_period->text();
    //int period_cnt=period.toInt();
    uint16_t kp[3]={0},ki[3]={0},kd[3]={0};
    kp[0]=ui->kp1->text().toUInt();
    ki[0]=ui->ki1->text().toUInt();
    kd[0]=ui->kd1->text().toUInt();

    kp[1]=ui->kp2->text().toUInt();
    ki[1]=ui->ki2->text().toUInt();
    kd[1]=ui->kd2->text().toUInt();

    kp[2]=ui->kp3->text().toUInt();
    ki[2]=ui->ki3->text().toUInt();
    kd[2]=ui->kd3->text().toUInt();
    Send_PID(1,kp[0],ki[0],kd[0],
                  kp[1],ki[1],kd[1],
                  kp[2],ki[2],kd[2]);
}

void SetParameter::on_clear_clicked()
{
  ui->msg->clear();
  ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

void SetParameter::on_write_clicked()
{
  uint16_t targeheight=0,safeheight=0,safevbat=0,maxheight=0,maxradius=0,maxupvel=0,maxdownvel=0,maxhorvel=0;
  targeheight=ui->targeheight->text().toUInt();
  safeheight=ui->safeheight->text().toUInt();
  safevbat=ui->safevbat->text().toUInt();
  maxheight=ui->maxheight->text().toUInt();
  maxradius=ui->maxradius->text().toUInt();
  maxupvel=ui->maxupvel->text().toUInt();
  maxdownvel=ui->maxdownvel->text().toUInt();
  maxhorvel=ui->maxhorvel->text().toUInt();

  Send_Parameter(1,targeheight,safeheight,safevbat,maxheight,maxradius,maxupvel,maxdownvel,maxhorvel);

}

void SetParameter::on_read_clicked()
{
  Send_Cmd_Check(0x01,0x03);
}

void SetParameter::on_factory_clicked()
{
  Send_Cmd_Check(0x01,0x04);
}
