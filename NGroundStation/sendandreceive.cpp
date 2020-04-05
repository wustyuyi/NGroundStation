#include "sendandreceive.h"
#include "ui_sendandreceive.h"
#include "Headfile.h"

#include "mainwindow.h"

uint32_t rec_cnt=0;
uint32_t send_cnt=0;
QByteArray buffer_Serial;
QByteArray str_buffer_Serial;
serial_config config;
QSerialPort* serialbasic=nullptr;//创建串口对象
QTimer *timer_serialsendandreceive=nullptr;
QTimer *timer_autosend=nullptr;
sendandreceive::sendandreceive(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sendandreceive)
{
    ui->setupUi(this);
    this->setWindowTitle("基本收发");                        //设置主界面名称
    setWindowIcon(QIcon(":/image/senddata.png"));               //设置主界面LOGO

    //串口接收界面刷新定时器
    timer_serialsendandreceive = new QTimer(this);
    connect(timer_serialsendandreceive, SIGNAL(timeout()), this, SLOT(timer_serialsendandreceiveEvent()));
    timer_serialsendandreceive->start(10);

    ui->send_period->setValidator(new QIntValidator(1, 100000, this));//限制数据输入范围

    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor(233,235,254));//QColor(78,168,243)
    this->setPalette(palette);

    init();
    connect(ui->cBox_SerialPortNum,     SIGNAL(activated(int)),this,SLOT(serialconfig_update(int)));
    connect(ui->cBox_SerialPortBaud,    SIGNAL(activated(int)),this,SLOT(serialconfig_update(int)));
    connect(ui->CBox_SerialPortcheck,   SIGNAL(activated(int)),this,SLOT(serialconfig_update(int)));
    connect(ui->cBox_SerialPortStopBit, SIGNAL(activated(int)),this,SLOT(serialconfig_update(int)));
    connect(ui->cBox_SerialPortDataNum, SIGNAL(activated(int)),this,SLOT(serialconfig_update(int)));
}

sendandreceive::~sendandreceive()
{
    delete ui;
}

void sendandreceive::on_bt_clearshow_clicked()
{
    ui->SerialPortDataShow->clear();//清屏
}

void sendandreceive::on_bt_clearsend_clicked()
{
   ui->SerialPortDataSend->clear();//清屏
}

void sendandreceive::timer_serialsendandreceiveEvent()
{
    static uint32_t last_cnt=0;
    QByteArray bufferHex = str_buffer_Serial.toHex();//转换成hex
    bufferHex=bufferHex.toUpper();//转换成大写
    QString str_buffer1;
    if(ui->cb_rechex->isChecked())//16进制显示
    {
      str_buffer1 = QString(bufferHex);//转换成字符
      for(int i = str_buffer1.count(); i > 0; i = i-2)
      {
        str_buffer1.insert(i," ");
      }
    }
    else//字符形式显示
    {
       str_buffer1 = QString(str_buffer_Serial);
    }
    if(ui->opencolse->text()=="关闭串口")
    {
       ui->SerialPortDataShow->insertPlainText(str_buffer1);
       ui->SerialPortDataShow->moveCursor(QTextCursor::End);
    }
     str_buffer1=ui->SerialPortDataShow->toPlainText();
     if(str_buffer1.size()>10000) ui->SerialPortDataShow->clear();

    QString str1 ="共接收:";
    QString str2=QString::number(rec_cnt,10);//转化成十进制字符型数据
    QString str3 ="字节，速度:";
    QString str4=QString::number((uint32_t)((rec_cnt-last_cnt)/0.01),10);//转化成十进制字符型数据
    QString str5 ="字节/秒，共发送:";
    QString str6 =QString::number(send_cnt,10);//转化成十进制字符型数据;
    QString str7 =str1+str2+str3+str4+str5+str6+"字节";
    ui->Label_RecSend_Cnt->setText(str7);

    str_buffer_Serial.clear();

    last_cnt=rec_cnt;
}



#include "mainwindow.h"
#include "ui_mainwindow.h"
void sendandreceive::on_bt_sendserialdata_clicked()
{
    //emit senddata(ui->SerialPortDataSend->toPlainText());

    if(ui->opencolse->text()=="打开串口")
    {
       QMessageBox::critical(this,"温馨提示","主窗口串口未打开。",QMessageBox::Yes,QMessageBox::Yes);
       return;
    }
    QString command = ui->SerialPortDataSend->toPlainText();//从发送窗口取出文本
    if(ui->cb_sendhex->isChecked())
    {
        QStringList commadList = command.split(' ');//寻找空格把字符串分开成字符
        QByteArray byteArray;
        byteArray.resize(commadList.count());//初始化新数组
        bool ok = false;
        for(int i = 0; i < commadList.count(); i++)
        {
            byteArray[i]=commadList.at(i).toInt(&ok,16);
        }
        send_cnt+=commadList.count();
        serialbasic->write(byteArray);//以16进制形式发送
    }
    else
    {
        serialbasic->write(command.toUtf8().data());//以字符形式发送
        send_cnt+=command.length();
    }

    int a=(int)QThread::currentThreadId();
    QString LogInfo=QString::number(a,10);
    //qDebug() << "sendandreceive " <<"threadID : "<<LogInfo+"ui";
}



void sendandreceive::on_bt_clearstatistics_clicked()
{
  rec_cnt=0;
  send_cnt=0;
}

void sendandreceive::on_cb_autosend_stateChanged(int arg1)
{
    if(ui->cb_autosend->isChecked())
    {
        timer_autosend = new QTimer(this);
        connect(timer_autosend, SIGNAL(timeout()), this, SLOT(timer_autosend_event()));
        QString period=ui->send_period->text();
        int period_cnt=period.toInt();
        timer_autosend->start(period_cnt);
        //qDebug()<<"自动发送开始"<<"start";
    }
    else
    {
        timer_autosend->stop();
        delete timer_autosend;
        //qDebug()<<"自动发送结束"<<"stop";
    }
}


void sendandreceive::timer_autosend_event()
{
    QString command = ui->SerialPortDataSend->toPlainText();//从发送窗口取出文本
    if(ui->opencolse->text()=="打开串口")
    {
       //QMessageBox::critical(this,"温馨提示","窗口串口未打开。",QMessageBox::Yes,QMessageBox::Yes);
       return;
    }

    if(ui->cb_sendhex->isChecked())
    {
        QStringList commadList = command.split(' ');//寻找空格把字符串分开成字符
        QByteArray byteArray;
        byteArray.resize(commadList.count());//初始化新数组
        bool ok = false;
        for(int i = 0; i < commadList.count(); i++)
        {
            byteArray[i]=commadList.at(i).toInt(&ok,16);
        }
        send_cnt+=commadList.count();
        serialbasic->write(byteArray);//以16进制形式发送
    }
    else
    {
        serialbasic->write(command.toUtf8().data());//以字符形式发送
        send_cnt+=command.length();
    }
}

void sendandreceive::on_send_period_editingFinished()//设置发送周期，回车后生效
{
    if(ui->cb_autosend->isChecked())
    {
        QString period=ui->send_period->text();
        int period_cnt=period.toInt();
        timer_autosend->start(period_cnt);
        //qDebug()<<"周期设置完毕"<<"period_set success";
    }
}



void sendandreceive::on_bt_send1_clicked()
{
    if(ui->opencolse->text()=="打开串口")
    {
       QMessageBox::critical(this,"温馨提示","主窗口串口未打开。",QMessageBox::Yes,QMessageBox::Yes);
       return;
    }
    QString command = ui->lineEdit1->text();//从发送窗口取出文本
    if(ui->cb_hex1->isChecked())
    {
        QStringList commadList = command.split(' ');//寻找空格把字符串分开成字符
        QByteArray byteArray;
        byteArray.resize(commadList.count());//初始化新数组
        bool ok = false;
        for(int i = 0; i < commadList.count(); i++)
        {
            byteArray[i]=commadList.at(i).toInt(&ok,16);
        }
        send_cnt+=commadList.count();
        serialbasic->write(byteArray);//以16进制形式发送
    }
    else
    {
        serialbasic->write(command.toUtf8().data());//以字符形式发送
        send_cnt+=command.length();
    }
}

void sendandreceive::on_bt_send2_clicked()
{
    if(ui->opencolse->text()=="打开串口")
    {
       QMessageBox::critical(this,"温馨提示","主窗口串口未打开。",QMessageBox::Yes,QMessageBox::Yes);
       return;
    }
    QString command = ui->lineEdit2->text();//从发送窗口取出文本
    if(ui->cb_hex2->isChecked())
    {
        QStringList commadList = command.split(' ');//寻找空格把字符串分开成字符
        QByteArray byteArray;
        byteArray.resize(commadList.count());//初始化新数组
        bool ok = false;
        for(int i = 0; i < commadList.count(); i++)
        {
            byteArray[i]=commadList.at(i).toInt(&ok,16);
        }
        send_cnt+=commadList.count();
        serialbasic->write(byteArray);//以16进制形式发送
    }
    else
    {
        serialbasic->write(command.toUtf8().data());//以字符形式发送
        send_cnt+=command.length();
    }
}

void sendandreceive::on_bt_send3_clicked()
{
    if(ui->opencolse->text()=="打开串口")
    {
       QMessageBox::critical(this,"温馨提示","主窗口串口未打开。",QMessageBox::Yes,QMessageBox::Yes);
       return;
    }
    QString command = ui->lineEdit3->text();//从发送窗口取出文本
    if(ui->cb_hex3->isChecked())
    {
        QStringList commadList = command.split(' ');//寻找空格把字符串分开成字符
        QByteArray byteArray;
        byteArray.resize(commadList.count());//初始化新数组
        bool ok = false;
        for(int i = 0; i < commadList.count(); i++)
        {
            byteArray[i]=commadList.at(i).toInt(&ok,16);
        }
        send_cnt+=commadList.count();
        serialbasic->write(byteArray);//以16进制形式发送
    }
    else
    {
        serialbasic->write(command.toUtf8().data());//以字符形式发送
        send_cnt+=command.length();
    }
}

void sendandreceive::on_bt_send4_clicked()
{
    if(ui->opencolse->text()=="打开串口")
    {
       QMessageBox::critical(this,"温馨提示","主窗口串口未打开。",QMessageBox::Yes,QMessageBox::Yes);
       return;
    }
    QString command = ui->lineEdit4->text();//从发送窗口取出文本
    if(ui->cb_hex4->isChecked())
    {
        QStringList commadList = command.split(' ');//寻找空格把字符串分开成字符
        QByteArray byteArray;
        byteArray.resize(commadList.count());//初始化新数组
        bool ok = false;
        for(int i = 0; i < commadList.count(); i++)
        {
            byteArray[i]=commadList.at(i).toInt(&ok,16);
        }
        send_cnt+=commadList.count();
        serialbasic->write(byteArray);//以16进制形式发送
    }
    else
    {
        serialbasic->write(command.toUtf8().data());//以字符形式发送
        send_cnt+=command.length();
    }
}

void sendandreceive::on_bt_send5_clicked()
{
    if(ui->opencolse->text()=="打开串口")
    {
       QMessageBox::critical(this,"温馨提示","主窗口串口未打开。",QMessageBox::Yes,QMessageBox::Yes);
       return;
    }
    QString command = ui->lineEdit5->text();//从发送窗口取出文本
    if(ui->cb_hex5->isChecked())
    {
        QStringList commadList = command.split(' ');//寻找空格把字符串分开成字符
        QByteArray byteArray;
        byteArray.resize(commadList.count());//初始化新数组
        bool ok = false;
        for(int i = 0; i < commadList.count(); i++)
        {
            byteArray[i]=commadList.at(i).toInt(&ok,16);
        }
        send_cnt+=commadList.count();
        serialbasic->write(byteArray);//以16进制形式发送
    }
    else
    {
        serialbasic->write(command.toUtf8().data());//以字符形式发送
        send_cnt+=command.length();
    }
}


void sendandreceive::init(void)
{
    QList<QSerialPortInfo> comList = QSerialPortInfo::availablePorts();//获取可用串口列表

    ui->cBox_SerialPortNum->clear();//清空item，避免搜索后重复添加
    ui->cBox_SerialPortBaud->clear();//清空item，避免搜索后重复添加
    ui->CBox_SerialPortcheck->clear();//清空item，避免搜索后重复添加
    ui->cBox_SerialPortStopBit->clear();//清空item，避免搜索后重复添加
    ui->cBox_SerialPortDataNum->clear();//清空item，避免搜索后重复添加

    if(comList.isEmpty())//若没有可用串口，则发送警告
    {
        QMessageBox::warning(this,"Waring!","There's no avalible COM to use, plese check your serialport!");
    }
    else//将可用串口显示到comboBox上以供选择
    {
        for(int i = 0; i < comList.size(); i ++)
        {
            ui->cBox_SerialPortNum->addItem(comList[i].portName());
            //ui->cBox_SerialPortNum->addItem(comList[i].description());//设备描述
        }
    }

    QStringList baudRateList = QStringList() << "1200" << "2400" << "4800"
                                             << "9600" << "14400" << "19200" << "38400" << "43000" << "57600"
                                             << "76800" << "115200" << "128000" << "230400" << "256000" <<"460800"
                                             << "921600" << "1382400";
    QStringList parityList = QStringList() << "无" << "奇校验" << "偶校验";
    QStringList stopBitsList = QStringList() << "1" << "1.5" << "2";
    QStringList dataBitsList = QStringList() << "8" << "7" << "6" << "5";

    ui->cBox_SerialPortBaud->addItems(baudRateList);
    ui->CBox_SerialPortcheck->addItems(parityList);
    ui->cBox_SerialPortStopBit->addItems(stopBitsList);
    ui->cBox_SerialPortDataNum->addItems(dataBitsList);

    ui->cBox_SerialPortBaud->setCurrentIndex(10);//设置115200为默认选项


    config.serialname=ui->cBox_SerialPortNum->currentText();//获取串口号;
    QString strBaudRate = ui->cBox_SerialPortBaud->currentText();//获取波特率
    config.baudrate=strBaudRate.toInt();//文本转整型;
    config.parity=ui->CBox_SerialPortcheck->currentIndex();//校验形式;
    config.dataBit=ui->cBox_SerialPortDataNum->currentIndex();//数据位数;
    config.stopBit=ui->cBox_SerialPortStopBit->currentIndex();//停止位数;
}

void sendandreceive::on_opencolse_clicked()
{
    if(ui->opencolse->text()=="打开串口")
    {


        serialbasic= new QSerialPort(this);//构建串口设备
        serialbasic->setPortName(config.serialname);//设置串口号
        if(serialbasic->isOpen())//如果串口已打开，立刻关闭
        {
            serialbasic->close();
        }

        bool bOK = serialbasic->open(QIODevice::ReadWrite);
        if(!bOK)
        {
            QMessageBox::critical(this,"提示","无法打开串口，请检查是否被占用。",QMessageBox::Yes,QMessageBox::Yes);
            return;
        }

        serialbasic->setBaudRate(config.baudrate);


        switch(config.parity)//设置数据校验形式
        {
            case 0:
                serialbasic->setParity(QSerialPort::NoParity);//无校验
                break;
            case 1:
                serialbasic->setParity(QSerialPort::OddParity);//奇校验
                break;
            case 2:
                serialbasic->setParity(QSerialPort::EvenParity);//偶校验
                break;
            default:
                serialbasic->setParity(QSerialPort::NoParity);
                break;
        }

        switch(config.dataBit)//设置数据位
        {
            case 0:
                serialbasic->setDataBits(QSerialPort::Data8);
                break;
            case 1:
                serialbasic->setDataBits(QSerialPort::Data7);
                break;
            case 2:
                serialbasic->setDataBits(QSerialPort::Data6);
                break;
            case 3:
                serialbasic->setDataBits(QSerialPort::Data5);
                break;
            default:
                serialbasic->setDataBits(QSerialPort::Data8);
                break;
        }

        switch(config.stopBit)//设置停止位
        {
            case 0:
                serialbasic->setStopBits(QSerialPort::OneStop);
                break;
            case 1:
                serialbasic->setStopBits(QSerialPort::OneAndHalfStop);
                break;
            case 2:
                serialbasic->setStopBits(QSerialPort::TwoStop);
                break;
            default:
                serialbasic->setStopBits(QSerialPort::OneStop);
                break;
        }
        serialbasic->setFlowControl(QSerialPort::NoFlowControl); //无流控
        serialbasic->setReadBufferSize(8192);                    //数据缓冲区长度
        connect(serialbasic,SIGNAL(readyRead()),this,SLOT(serialPort_getdata()),Qt::DirectConnection);//连接串口接收信号与槽函数

        ui->cBox_SerialPortNum->setEnabled(false);
        ui->cBox_SerialPortBaud->setEnabled(false);
        ui->CBox_SerialPortcheck->setEnabled(false);
        ui->cBox_SerialPortDataNum->setEnabled(false);
        ui->cBox_SerialPortStopBit->setEnabled(false);

        ui->opencolse->setText("关闭串口");
        ui->opencolse->setIcon(QIcon(":/image/opencom.png"));
    }
    else
    {     
        ui->opencolse->setText("打开串口");
        ui->opencolse->setIcon(QIcon(":/image/closecom.png"));
        serialbasic->close();
        delete serialbasic;

        ui->cBox_SerialPortNum->setEnabled(true);
        ui->cBox_SerialPortBaud->setEnabled(true);
        ui->CBox_SerialPortcheck->setEnabled(true);
        ui->cBox_SerialPortDataNum->setEnabled(true);
        ui->cBox_SerialPortStopBit->setEnabled(true);
    }
}

void sendandreceive::serialconfig_update(int index)
{
    config.serialname=ui->cBox_SerialPortNum->currentText();    //获取串口号;
    QString strBaudRate =ui->cBox_SerialPortBaud->currentText();//获取波特率
    config.baudrate=strBaudRate.toInt();                        //文本转整型;
    config.parity=ui->CBox_SerialPortcheck->currentIndex();     //校验形式;
    config.dataBit=ui->cBox_SerialPortDataNum->currentIndex();  //数据位数;
    config.stopBit=ui->cBox_SerialPortStopBit->currentIndex();  //停止位数;

    //qDebug() << config.serialname<<endl;
    //qDebug() << config.baudrate<<endl;
    //qDebug() << config.parity<<endl;
    //qDebug() << config.dataBit<<endl;
    //qDebug() << config.stopBit<<endl;
}



//串口接收数据槽函数
void sendandreceive::serialPort_getdata()
{
    qint64 byteLen_now = serialbasic->bytesAvailable();
    if(byteLen_now < 0)
    {
        return;
    }
    //qDebug()<<"当前缓存长度"<<byteLen_now<<endl;

    buffer_Serial+=serialbasic->readAll();

    QByteArray bufferHex = buffer_Serial.toHex();//转换成hex
    bufferHex=bufferHex.toUpper();//转换成大写
    rec_cnt=rec_cnt + buffer_Serial.count();//接收计数器自增
    str_buffer_Serial+=buffer_Serial;

    //QString str_buffer = QString(buffer_Serial);
    QString str_buffer = QString(bufferHex);//转换成字符
    for(int i = str_buffer.count(); i > 0; i = i-2)
    {
      str_buffer.insert(i," ");
    }

    buffer_Serial.clear();
}


void sendandreceive::on_cBox_SerialPortNum_highlighted(int index)
{
    QList<QSerialPortInfo> comList = QSerialPortInfo::availablePorts();//获取可用串口列表
    //qDebug() <<"搜索长度"<<comList.size();
    //qDebug() <<"当前长度"<<ui->cBox_SerialPortNum->count();
    if(comList.isEmpty())//若没有可用串口，则发送警告
    {
        QMessageBox::warning(this,"Waring!","There's no avalible COM to use, plese check your serialport!");
    }
    else//将可用串口显示到comboBox上以供选择
    {
        if(comList.size()!=ui->cBox_SerialPortNum->count())
        {
          ui->cBox_SerialPortNum->clear();//清空item，避免搜索后重复添加
          for(int i = 0; i < comList.size(); i ++)
          {
              ui->cBox_SerialPortNum->addItem(comList[i].portName());
          }
        }
    }
}


//当窗口出发关闭事件后，将信号发射出去
void sendandreceive::closeEvent(QCloseEvent *)
{
    emit exitwindows();
}
