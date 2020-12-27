#include "widget.h"
#include "ui_widget.h"
#include "QDebug"
Widget::Widget(QWidget *parent) ://构造函数
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    Timer = new QTimer(this);
    /*初始化参数*/
    max=0;
    Clk=0;
    ui->tabWidget->setTabText(0,"Data");
    ui->tabWidget_2->setTabText(0,"控制");
    ui->tabWidget_2->setTabText(1,"电机");
    ui->tabWidget_2->setTabText(2,"模式");
    /*执行函数*/
      InitPort();//判断当前是否有效的串口，并得到相关得数据
      ReadPort();
      Clk = 0;
}
//ui是系统根据bai界面ui文件自动生成du的ui类创建的对象，
//作为窗口serial类的成员，可能zhi因为在UI命名控件继承的时候没有加OBJECT宏，所以需要手动delete
Widget::~Widget()//析构函数，在析构的时候baidelete掉ui
{
    delete ui;
}
/*
函数说明
str.section();
分割后生成数组，取某位数
*/
void Widget::GetData(QString str)//接收的数据
{
   //如果数据为空则返回
   if(str.isEmpty())
        return;

   //提取串口数据
   if(str.section("#",0,0) == "Z")//如果头目是I
   {
       double num  = str.section("#",1,1).toDouble();//取完数后转为成double类型
       double num1 = str.section("#",3,3).toDouble();
       //ui->textEdit_2->setText(QString("I is %1mA\r\nV is %2mV\r\nT is %3Nm\r\na is %4°\r\nm is %5°\r\n").arg(num).arg(num1).arg(num2).arg(num3).arg(num4));
       if(num <= 0 || num1 <= 0)//如果收到的数据是小于0的，则没用跳过
           return ;
      // YData.pop_front();
       YData.push_back(num);//数据1
       if(YData.count() >= MAX_YCOUNT){
            YData.pop_front();
        }

       //Y1Data.pop_front();
       Y1Data.push_back(num1);//数据2
       if(Y1Data.count() >= MAX_YCOUNT){
            Y1Data.pop_front();
        }
       //x轴变化
       //XData.pop_front();//删除第一个元素，清空数据
       XData.push_back(Clk++);//加入数组尾部，其实就是让x轴坐标每次增加
       if(XData.count() >= MAX_XCOUNT){
            XData.pop_front();
        }
    }
}
/*获取串口数据，我这里是采用每隔40ms来获取一次数据*/
void Widget::ReadPort()
{
    QTimer::singleShot(40,this,&Widget::ReadPort);
    if(SerialPort.isOpen() == false)
        return;
    //得到串口数据
    QByteArray temp = SerialPort.readAll();
    if(temp.isEmpty())
        return;
    else{
        //将数据进行文本化显示
        ui->textEdit_2->append(temp);
        //处理当前数据，得出有效数据
        if(max==0)
        {
           GetData(QString(temp));
        }
       else if(max==1)
        {
           GetData(QString(temp.toHex()));
        }
    }
}


/*判断当前是否有效的串口，并得到相关得数据*/
void Widget::InitPort()
{
    //遍历所有有效的串口，得到每个有效串口得数据
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name :"<<info.portName();
        qDebug() << "Description: "<<info.description();
        qDebug() << "Manufacturer: "<<info.manufacturer();
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {//判断是否能够打开该串口，如果能，则将相应的串口名加到串口的下拉栏里
            ui->comboBoxCom->addItem(info.portName());
            serial.close();//这里只是起判定效果，不用以打开，还得关闭当前串口，方便查询其他串口
        }
    }
}
/*这个是开启按键的槽函数，如果有有效的串口，然后进行打开操作，也可以充当关闭串口的作用*/
void Widget::on_pushButtonS_clicked()
{
    //判断当前程序处于什么状态中，如果没有串口使用，则进入“开启串口”
    if(ui->pushButtonS->text() == "开启串口")
    {    //得到当前串口栏得数据
        SerialPort.setPortName(ui->comboBoxCom->currentText());
        if(SerialPort.open(QIODevice::ReadWrite))
        {
            SerialPort.setBaudRate(ui->comboBoxBaut->currentText().toInt());//设置波特率
            SerialPort.setDataBits(QSerialPort::Data8);//设置数据位为8位
            SerialPort.setParity(QSerialPort::NoParity);//没有奇偶校验位
            SerialPort.setFlowControl(QSerialPort::NoFlowControl);//没有流控制
            SerialPort.setStopBits(QSerialPort::OneStop);//设置一个停止位

            ui->pushButtonS->setText("关闭串口");    //更改按键状态
        }
        else{
            qDebug() << "串口没有打开";
            return;
        }
    }
    else if(ui->pushButtonS->text() == "关闭串口"){
        SerialPort.close();
        ui->pushButtonS->setText("开启串口");
    }
    else{
        qDebug() <<"无效操作";
    }
}


/**
信号槽处理函数
*/
void Widget::on_pushButtonCW_clicked()
{
    ui->textEdit->clear();
    ui->textEdit_3->clear();
    ui->textEdit_9->clear();
    ui->textEdit_2->clear();
    ui->textEdit_6->clear();
    ui->textEdit_11->clear();
    ui->textEdit_12->clear();
    ui->textEdit_7->clear();
    ui->textEdit_8->clear();
    Clk = 0;
}

//停止发送
void Widget::on_pushButtonCR_clicked()
{
    if(start==1)
    {
        if(ui->pushButtonCR->text() == "暂停")
        {
            QByteArray Data4;
            QString sendStr4="S";
            sendStr4+="1";
            sendStr4+='#';
            Data4 = sendStr4.toUtf8();
            SerialPort.write(Data4);
            ui->pushButtonCR->setText("继续");    //更改按键状态
            start=0;
        }
    }
    else if(ui->pushButtonCR->text() == "继续"){
            QByteArray Data3;
            QString sendStr3="B";
            sendStr3+="1";
            sendStr3+='#';
            Data3 = sendStr3.toUtf8();
            SerialPort.write(Data3);
           // qDebug()<<Data3<<endl;
        ui->pushButtonCR->setText("暂停");
    }

}
/*发送数据给下位机:参数*/
//控制参数
void Widget::on_pushButton_6_clicked()//位置
{
    if(ui->textEdit_6->document()->isEmpty()){
            QMessageBox::information(NULL, "Warnning", "请输入数据");
    }
    else{
        QByteArray Data;
        QString sendStr="A";
        sendStr+= ui->textEdit_6->toPlainText();
        sendStr+='W';
        Data = sendStr.toUtf8();
        SerialPort.write(Data);
    }
}

void Widget::on_pushButton_7_clicked()//扭矩
{
    if(ui->textEdit_7->document()->isEmpty()){
            QMessageBox::information(NULL, "Warnning", "请输入数据");
    }
    else{
        QByteArray Data;
        QString sendStr="B";
        sendStr+= ui->textEdit_7->toPlainText();
        sendStr+='W';
        Data = sendStr.toUtf8();
        SerialPort.write(Data);
    }
}

void Widget::on_pushButton_8_clicked()//转速
{
    if(ui->textEdit_8->document()->isEmpty()){
            QMessageBox::information(NULL, "Warnning", "请输入数据");
    }
    else{
        QByteArray Data;
        QString sendStr="C";
        sendStr+= ui->textEdit_8->toPlainText();
        sendStr+='W';
        Data = sendStr.toUtf8();
        SerialPort.write(Data);
    }
}
void Widget::on_pushButton_clicked()//控P
{
    if(ui->textEdit_8->document()->isEmpty()){
            QMessageBox::information(NULL, "Warnning", "请输入数据");
    }
    else{
        QByteArray Data;
        QString sendStr="D";
        sendStr+= ui->textEdit_11->toPlainText();
        sendStr+='W';
        Data = sendStr.toUtf8();
        SerialPort.write(Data);
    }
}
void Widget::on_pushButton_14_clicked()//控I
{
    if(ui->textEdit_8->document()->isEmpty()){
            QMessageBox::information(NULL, "Warnning", "请输入数据");
    }
    else{
        QByteArray Data;
        QString sendStr="E";
        sendStr+= ui->textEdit_12->toPlainText();
        sendStr+='W';
        Data = sendStr.toUtf8();
        SerialPort.write(Data);
    }
}
//电机参数
void Widget::on_pushButton_2_clicked()//电P
{
    if( ui->textEdit->document()->isEmpty())
    {
            QMessageBox::information(NULL, "Warnning", "请输入数据");
    }
    else{
        QByteArray Data;
        QString sendStr="F";
        sendStr+= ui->textEdit->toPlainText();
        sendStr+='W';
        Data = sendStr.toUtf8();
        SerialPort.write(Data);
    }
}
void Widget::on_pushButton_9_clicked()//电I
{
    if( ui->textEdit_9->document()->isEmpty())
    {
            QMessageBox::information(NULL, "Warnning", "请输入数据");
    }
    else{
        QByteArray Data;
        QString sendStr="G";
        sendStr+= ui->textEdit_9->toPlainText();
        sendStr+='W';
        Data = sendStr.toUtf8();
        SerialPort.write(Data);
    }
}
void Widget::on_pushButton_3_clicked()//电机的ID
{
        if( ui->textEdit_3->document()->isEmpty())
        {
                QMessageBox::information(NULL, "Warnning", "请输入数据");
        }
        else{
            QByteArray Data;
            QString sendStr="H";
            sendStr+= ui->textEdit_3->toPlainText();
            sendStr+='W';
            Data = sendStr.toUtf8();
            SerialPort.write(Data);
          //  qDebug()<<Data<<endl;
        }
}
//电机模式切换
void Widget::on_pushButton_12_clicked()//机械模式
{
    QByteArray Data;
    QString sendStr="Z";
    sendStr+="0";
    sendStr+='#';
    Data = sendStr.toUtf8();
    SerialPort.write(Data);
  //  qDebug()<<Data<<endl;
}
void Widget::on_pushButton_13_clicked()//休眠模式
{
    QByteArray Data;
    QString sendStr="Z";
    sendStr+="1";
    sendStr+='#';
    Data = sendStr.toUtf8();
    SerialPort.write(Data);
  //  qDebug()<<Data<<endl;
}
void Widget::on_pushButton_4_clicked()//设置模式
{
    QByteArray Data;
    QString sendStr="Z";
    sendStr+="2";
    sendStr+='#';
    Data = sendStr.toUtf8();
    SerialPort.write(Data);
  //  qDebug()<<Data<<endl;
}
void Widget::on_pushButton_5_clicked()//编码值发射模式
{
    QByteArray Data;
    QString sendStr="Z";
    sendStr+="3";
    sendStr+='#';
    Data = sendStr.toUtf8();
    SerialPort.write(Data);
  //  qDebug()<<Data<<endl;
}

void Widget::on_pushButton_10_clicked()//矫正模式
{
    QByteArray Data;
    QString sendStr="Z";
    sendStr+="4";
    sendStr+='#';
    Data = sendStr.toUtf8();
    SerialPort.write(Data);
  //  qDebug()<<Data<<endl;
}



void Widget::on_pushButton_11_clicked()//电机置零位
{
    QByteArray Data;
    QString sendStr="Z";
    sendStr+="5";
    sendStr+='#';
    Data = sendStr.toUtf8();
    SerialPort.write(Data);
  //  qDebug()<<Data<<endl;
}

//纯发送数据。字符串接收。
void Widget::on_pushButton_15_clicked()
{
    QByteArray Data;
    QString sendStr= ui->textEdit_4->toPlainText();
    Data = sendStr.toUtf8();
    SerialPort.write(Data);
}
//将十进制int转换为十六进制字符串
QString Widget::decInt2HexStr(int dec)
{
    //保证数据在两帧范围内
    if(dec > 65535)
    {
        QMessageBox::information(NULL, "警告", "输入超出规定范围（input < 65535）");
        return "0";
    }
    QString hexStr;
    int temp = dec / 16;
    int left = dec % 16;
    if(temp > 0)
        hexStr += decInt2HexStr(temp);
    if(left < 10)
        hexStr += (left + '0');
    else
        hexStr += ('A' + left - 10);
    return hexStr;
}
//字符串转Hex(QByteArray)类型
QByteArray Widget::QString2Hex(QString hexStr)
{
    QByteArray senddata;
    int hexdata, lowhexdata;
    int hexdatalen = 0;
    int len = hexStr.length();
    senddata.resize(len/2);
    char lstr, hstr;
    for(int i = 0; i < len; )
    {
        //将第一个不为' '的字符赋给hstr;
        hstr = hexStr[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        //当i >= len时，跳出循环
        if(i >= len)
            break;
        //当i < len时，将下一个字符赋值给lstr;
        lstr = hexStr[i].toLatin1();
        //将hstr和lstr转换为0-15的对应数值
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        //
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata * 16 + lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
    return senddata;
}

//将单个字符串转换为hex
//0-F -> 0-15
char Widget::ConvertHexChar(char c)
{
    if((c >= '0') && (c <= '9'))
        return c - 0x30;
    else if((c >= 'A') && (c <= 'F'))
        return c - 'A' + 10;//'A' = 65;
    else if((c >= 'a') && (c <= 'f'))
        return c - 'a' + 10;
    else
        return -1;
}
//将输入格式化，补满四位：0XFFFF
QString Widget::formatInput(QString hexStr)
{
    int strlen = hexStr.length();
    switch(strlen)
    {
        case 0:
            hexStr = "0000";
            break;
        case 1:
            hexStr = "000" + hexStr;
            break;
        case 2:
            hexStr = "00" + hexStr;
            break;
        case 3:
            hexStr = "0" + hexStr;
            break;
        case 4:
            hexStr = hexStr;
            break;
        default:
            break;
    }
    return hexStr;
}
//Hex发送
void Widget::on_pushButton_16_clicked()
{
    QByteArray Data;
    QString hexStr= ui->textEdit_5->toPlainText();
    Data = QString2Hex(formatInput(hexStr));
    SerialPort.write(Data);
}

void Widget::on_checkBox_clicked()
{
 if(ui->checkBox->isChecked())
 {
     max=1;
 }
 else
 {
     max=0;
 }

}
