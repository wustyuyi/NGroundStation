#include "calibration.h"
#include "ui_calibration.h"
#include "Headfile.h"


calibration::calibration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::calibration)
{
    ui->setupUi(this);
    this->setWindowTitle("飞控校准");
    setWindowIcon(QIcon(":/image/calibration.png"));                //设置主界面LOGO
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
    setFixedSize(this->width(),this->height());                     // 禁止拖动窗口大小

    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor(233,235,254));//QColor(78,168,243)
    this->setPalette(palette);


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timer_show()));
    timer->start(50);//20HZ

}

calibration::~calibration()
{
    delete ui;
}



//当窗口出发关闭事件后，将信号发射出去
void calibration::closeEvent(QCloseEvent *)
{
    emit exitwindows();
}


void calibration::getcheck(uint8_t ch)
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_time = current_date_time.toString("hh:mm:ss.zzz");
    if(ch==0x13)
    {
       ui->msg->appendPlainText(current_time+"毫秒>飞控传感器数据校准成功");
    }
    else if(ch==0x14)
    {
       ui->msg->appendPlainText(current_time+"毫秒>飞控传感器校准参数获取成功");
    }
    ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

void calibration::on_clear_clicked()
{
    ui->msg->clear();
    ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}



void calibration::timer_show()
{
  ui->gx->setText(QString::number(gyro_x_raw,10,4));
  ui->gy->setText(QString::number(gyro_y_raw,10,4));
  ui->gz->setText(QString::number(gyro_z_raw,10,4));
  ui->ax->setText(QString::number(acce_x_raw,10,4));
  ui->ay->setText(QString::number(acce_y_raw,10,4));
  ui->az->setText(QString::number(acce_z_raw,10,4));
  ui->mx->setText(QString::number(mag_x_raw,10,4));
  ui->my->setText(QString::number(mag_y_raw,10,4));
  ui->mz->setText(QString::number(mag_z_raw,10,4));

  ui->temp->setText(QString::number(imu_temp,10,2));


  ui->gx_offset->setText(QString::number(gyro_x_offset,10,4));
  ui->gy_offset->setText(QString::number(gyro_y_offset,10,4));
  ui->gz_offset->setText(QString::number(gyro_z_offset,10,4));
  ui->ax_offset->setText(QString::number(acce_x_offset,10,4));
  ui->ay_offset->setText(QString::number(acce_y_offset,10,4));
  ui->az_offset->setText(QString::number(acce_z_offset,10,4));
  ui->mx_offset->setText(QString::number(mag_x_offset,10,4));
  ui->my_offset->setText(QString::number(mag_y_offset,10,4));
  ui->mz_offset->setText(QString::number(mag_z_offset,10,4));
  ui->ax_k->setText(QString::number(acce_x_scale,10,4));
  ui->ay_k->setText(QString::number(acce_y_scale,10,4));
  ui->az_k->setText(QString::number(acce_z_scale,10,4));

  ui->pitch_offset->setText(QString::number(pitch_offset,10,4));
  ui->roll_offset->setText(QString::number(roll_offset,10,4));


  if(gyro_auto_cal_flag==0x00) ui->cal_finished->setText("否");
  else ui->cal_finished->setText("是");
}


extern QSerialPort* serialPort;
bool calibration::Send_CAL(uint8_t cal_flag,uint8_t cal_step,uint8_t cal_cmd)//地面站指令发送RC数据
{
  uint8_t data_to_send[50];
  uint8_t sum = 0,i=0,cnt=0;
  uint16_t _temp;
  data_to_send[cnt++]=NCLink_Head1;
  data_to_send[cnt++]=NCLink_Head0;
  data_to_send[cnt++]=0x0B;
  data_to_send[cnt++]=0;

  data_to_send[cnt++]=BYTE0(cal_flag);
  data_to_send[cnt++]=BYTE0(cal_step);
  data_to_send[cnt++]=BYTE0(cal_cmd);


  data_to_send[3] = cnt-4;
  for(i=0;i<cnt;i++) sum ^= data_to_send[i];
  data_to_send[cnt++]=sum;
  data_to_send[cnt++]=NCLink_End0;
  data_to_send[cnt++]=NCLink_End1;
  if(serialflag)
  {
     serialPort->write((char *)(data_to_send),cnt);
     return true;
  }
  else
  {
      QDateTime current_date_time = QDateTime::currentDateTime();
      QString current_time = current_date_time.toString("hh:mm:ss.zzz");
      ui->msg->appendPlainText(current_time+"毫秒>串口发送失败，请检查串口配置");
      ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
      return false;
  }
}


void calibration::on_gyro_cal_start_clicked()
{
  if(Send_CAL(0x01,0x00,0x00)==true)
  {
      QDateTime current_date_time = QDateTime::currentDateTime();
      QString current_time = current_date_time.toString("hh:mm:ss.zzz");
      ui->msg->appendPlainText(current_time+"毫秒>陀螺仪校准指令已发送");
      ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
  }
}

void calibration::on_acc_cal_start_clicked()
{
  if(Send_CAL(0x02,0x00,0x00)==true)
  {
      QDateTime current_date_time = QDateTime::currentDateTime();
      QString current_time = current_date_time.toString("hh:mm:ss.zzz");
      ui->msg->appendPlainText(current_time+"毫秒>加速度计校准指令已发送");
      ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
  }
}

void calibration::on_mag_cal_start_clicked()
{
  if(Send_CAL(0x03,0x01,0x00)==true)
  {
      QDateTime current_date_time = QDateTime::currentDateTime();
      QString current_time = current_date_time.toString("hh:mm:ss.zzz");
      ui->msg->appendPlainText(current_time+"毫秒>磁力计校准指令已发送");
      ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
  }
}

void calibration::on_hor_cal_start_clicked()
{
  if(Send_CAL(0x04,0x00,0x00)==true)
  {
      QDateTime current_date_time = QDateTime::currentDateTime();
      QString current_time = current_date_time.toString("hh:mm:ss.zzz");
      ui->msg->appendPlainText(current_time+"毫秒>机架水平校准指令已发送");
      ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
  }
}



void calibration::on_bt1_acc_cal_clicked()
{
  if(Send_CAL(0x00,0x01,0x00)==true)
  {
      QDateTime current_date_time = QDateTime::currentDateTime();
      QString current_time = current_date_time.toString("hh:mm:ss.zzz");
      ui->msg->appendPlainText(current_time+"毫秒>第1面校准，请将飞机水平放置");
      ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
  }
}

void calibration::on_bt2_acc_cal_clicked()
{
  if(Send_CAL(0x00,0x02,0x00)==true)
  {
      QDateTime current_date_time = QDateTime::currentDateTime();
      QString current_time = current_date_time.toString("hh:mm:ss.zzz");
      ui->msg->appendPlainText(current_time+"毫秒>第2面校准，请将飞机右侧朝上放置");
      ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
  }
}

void calibration::on_bt3_acc_cal_clicked()
{
  if(Send_CAL(0x00,0x03,0x00)==true)
  {
      QDateTime current_date_time = QDateTime::currentDateTime();
      QString current_time = current_date_time.toString("hh:mm:ss.zzz");
      ui->msg->appendPlainText(current_time+"毫秒>第3面校准，请将飞机左侧朝上放置");
      ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
  }
}

void calibration::on_bt4_acc_cal_clicked()
{
  if(Send_CAL(0x00,0x04,0x00)==true)
  {
      QDateTime current_date_time = QDateTime::currentDateTime();
      QString current_time = current_date_time.toString("hh:mm:ss.zzz");
      ui->msg->appendPlainText(current_time+"毫秒>第4面校准，请将飞机机头朝下放置");
      ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
  }
}

void calibration::on_bt5_acc_cal_clicked()
{
  if(Send_CAL(0x00,0x05,0x00)==true)
  {
      QDateTime current_date_time = QDateTime::currentDateTime();
      QString current_time = current_date_time.toString("hh:mm:ss.zzz");
      ui->msg->appendPlainText(current_time+"毫秒>第5面校准，请将飞机机头朝上放置");
      ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
  }
}

void calibration::on_bt6_acc_cal_clicked()
{
  if(Send_CAL(0x00,0x06,0x00)==true)
  {
      QDateTime current_date_time = QDateTime::currentDateTime();
      QString current_time = current_date_time.toString("hh:mm:ss.zzz");
      ui->msg->appendPlainText(current_time+"毫秒>第6面校准，请将飞机翻转并水平放置");
      ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
  }
}

void calibration::on_get_cal_para_clicked()
{
    if(Send_CAL(0x00,0x00,0x01)==true)
    {
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_time = current_date_time.toString("hh:mm:ss.zzz");
        ui->msg->appendPlainText(current_time+"毫秒>获取校准参数发送成功");
        ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    }
}


void calibration::on_quit_cal_now_clicked()
{
    if(Send_CAL(0x00,0x00,0x00)==true)
    {
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_time = current_date_time.toString("hh:mm:ss.zzz");
        ui->msg->appendPlainText(current_time+"毫秒>提前终止当前校准指令已发送");
        ui->msg->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    }
}
