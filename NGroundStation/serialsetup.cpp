#include "serialsetup.h"
#include "ui_serialsetup.h"

#include "Headfile.h"


serial_config serialconfig;
extern serialsetup *usbsetup;


serialsetup::serialsetup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::serialsetup)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);     // 禁止最大化按钮
    setFixedSize(this->width(),this->height());                     // 禁止拖动窗口大小
    this->setWindowTitle("串口配置");                               //设置主界面名称
    setWindowIcon(QIcon(":/image/usb.png"));                      //设置主界面LOGO
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor(233,235,254));//QColor(78,168,243)
    this->setPalette(palette);

    connect(ui->cBox_SerialPortNum,     SIGNAL(activated(int)),this,SLOT(serialconfig_update(int)));
    connect(ui->cBox_SerialPortBaud,    SIGNAL(activated(int)),this,SLOT(serialconfig_update(int)));
    connect(ui->CBox_SerialPortcheck,   SIGNAL(activated(int)),this,SLOT(serialconfig_update(int)));
    connect(ui->cBox_SerialPortStopBit, SIGNAL(activated(int)),this,SLOT(serialconfig_update(int)));
    connect(ui->cBox_SerialPortDataNum, SIGNAL(activated(int)),this,SLOT(serialconfig_update(int)));

    connect(ui->SerialPortBaud,SIGNAL(editingFinished()),this,SLOT(serial_update()));

            /*
            void textChanged(const QString &);
            void textEdited(const QString &);
            void cursorPositionChanged(int, int);
            void returnPressed();
            void editingFinished();
            void selectionChanged();
            void inputRejected();
            */
    ui->SerialPortBaud->setValidator(new QIntValidator(0, 4500000, this));//限制数据输入范围
}


serialsetup::~serialsetup()
{ 
    delete ui;
}


void serialsetup::serialinit(void)
{
    QList<QSerialPortInfo> comList = QSerialPortInfo::availablePorts();//获取可用串口列表

    ui->cBox_SerialPortNum->clear();//清空item，避免搜索后重复添加
    ui->cBox_SerialPortBaud->clear();//清空item，避免搜索后重复添加
    ui->CBox_SerialPortcheck->clear();//清空item，避免搜索后重复添加
    ui->cBox_SerialPortStopBit->clear();//清空item，避免搜索后重复添加
    ui->cBox_SerialPortDataNum->clear();//清空item，避免搜索后重复添加

    if(comList.isEmpty())//若没有可用串口，则发送警告
    {
        QMessageBox::warning(this,"串口异常警告","电脑没有可用串口设备, 请检查硬件连接！！！");
    }
    else//将可用串口显示到comboBox上以供选择
    {
        for(int i = 0; i < comList.size(); i ++)
        {
            ui->cBox_SerialPortNum->addItem(comList[i].portName());
            //ui->cBox_SerialPortNum->addItem(comList[i].description());//设备描述
        }
    }
    /*
    QStringList comList = QStringList() << "COM0" << "COM1" << "COM2"
                                             << "COM3" << "COM4" << "COM5" << "COM6" << "COM7" << "COM8"
                                             << "COM9" << "COM10" << "COM11" << "COM12" << "COM13" <<"COM14"
                                             << "COM15" << "COM16";
    ui->cBox_SerialPortNum->addItems(comList);
   */
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

    ui->cBox_SerialPortBaud->setCurrentIndex(15);//设置115200为默认选项  15

    serial_update();
}


void serialsetup::serialsetup_enable(bool flag)
{
    serialconfig.serialname=usbsetup->ui->cBox_SerialPortNum->currentText();//获取串口号;
    //QString strBaudRate = usbsetup->ui->cBox_SerialPortBaud->currentText();//获取波特率
    QString strBaudRate = usbsetup->ui->SerialPortBaud->text();//获取波特率
    serialconfig.baudrate=strBaudRate.toInt();//文本转整型;
    serialconfig.parity=usbsetup->ui->CBox_SerialPortcheck->currentIndex();//校验形式;
    serialconfig.dataBit=usbsetup->ui->cBox_SerialPortDataNum->currentIndex();//数据位数;
    serialconfig.stopBit=usbsetup->ui->cBox_SerialPortStopBit->currentIndex();//停止位数;

    ui->cBox_SerialPortNum->setEnabled(flag);
    ui->cBox_SerialPortBaud->setEnabled(flag);
    ui->cBox_SerialPortDataNum->setEnabled(flag);
    ui->CBox_SerialPortcheck->setEnabled(flag);
    ui->cBox_SerialPortStopBit->setEnabled(flag);
}



void serialsetup::serialconfig_update(int index)
{
    serialconfig.serialname=usbsetup->ui->cBox_SerialPortNum->currentText();//获取串口号;
    //QString strBaudRate = usbsetup->ui->cBox_SerialPortBaud->currentText();//获取波特率
    QString strBaudRate = usbsetup->ui->SerialPortBaud->text();//获取波特率
    serialconfig.baudrate=strBaudRate.toInt();//文本转整型;
    serialconfig.parity=usbsetup->ui->CBox_SerialPortcheck->currentIndex();//校验形式;
    serialconfig.dataBit=usbsetup->ui->cBox_SerialPortDataNum->currentIndex();//数据位数;
    serialconfig.stopBit=usbsetup->ui->cBox_SerialPortStopBit->currentIndex();//停止位数;
}

void serialsetup::serial_update()
{
    serialconfig.serialname=usbsetup->ui->cBox_SerialPortNum->currentText();//获取串口号;
    //QString strBaudRate = usbsetup->ui->cBox_SerialPortBaud->currentText();//获取波特率
    QString strBaudRate = usbsetup->ui->SerialPortBaud->text();//获取波特率
    serialconfig.baudrate=strBaudRate.toInt();//文本转整型;
    serialconfig.parity=usbsetup->ui->CBox_SerialPortcheck->currentIndex();//校验形式;
    serialconfig.dataBit=usbsetup->ui->cBox_SerialPortDataNum->currentIndex();//数据位数;
    serialconfig.stopBit=usbsetup->ui->cBox_SerialPortStopBit->currentIndex();//停止位数;
}


void serialsetup::on_cBox_SerialPortNum_highlighted(int index)
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
void serialsetup::closeEvent(QCloseEvent *)
{
    emit exitwindows();
}


void serialsetup::on_cBox_SerialPortBaud_currentIndexChanged(int index)
{
   ui->SerialPortBaud->setText(ui->cBox_SerialPortBaud->currentText());
}
