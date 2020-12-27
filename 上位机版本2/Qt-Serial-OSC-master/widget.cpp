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
    start=0;
    state0=0;
    state1=0;
    state2=0;
    state3=0;
    state4=0;
    ui->tabWidget->setTabText(0,"Gui");
    ui->tabWidget->setTabText(1,"Data");
    ui->tabWidget_2->setTabText(0,"1");
    ui->tabWidget_2->setTabText(1,"2");
    ui->tabWidget_2->setTabText(2,"3");
    ui->tabWidget_2->setTabText(3,"4");
    ui->tabWidget_2->setTabText(4,"5");
    //分配空间
    XData.resize(3000);
    XData.value(0,1);
    //y轴存储数据的空间
    YData.resize(3000);
    Y1Data.resize(3000);


    //窗口打印设置
    Plot = ui->widget; //指定窗口用于显示波形
    Plot->setMouseTracking(true);// 使能图层鼠标跟随
    // 开启图层上的图层刻度可手动调动
    Plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    connect(Plot->xAxis, SIGNAL(rangeChanged(QCPRange)), Plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(Plot->yAxis, SIGNAL(rangeChanged(QCPRange)), Plot->yAxis2, SLOT(setRange(QCPRange)));

    connect(Plot,&QCustomPlot::mouseMove,[=](QMouseEvent *e){//显示坐标信息
            double _x = Plot->xAxis->pixelToCoord(e->x()),_y = Plot->yAxis->pixelToCoord(e->y());
            ui->label_3->setText(QString("(%1,%2)").arg(_x).arg(_y));

    });
    connect(Plot,&QCustomPlot::plottableClick,[=](//显示曲线名字
                    QCPAbstractPlottable * plottable,
                    int ,
                    QMouseEvent * ){

                ui->label_4->setText(plottable->name());
    });
    Plot->addGraph();        //添加一条线
    Plot->addGraph();        //再添加一条线
    Plot->addGraph();        //再添加一条线
    Plot->addGraph();        //再添加一条线
    Plot->addGraph();        //再添加一条线
    Plot->graph(0)->setPen(QPen(Qt::red));    //设置第一根线的颜色为红色
    Plot->graph(0)->setName("电流曲线");
    Plot->graph(0)->setVisible(false);		// 曲线不显示
    Plot->graph(1)->setPen(QPen(Qt::blue));   //设置第二根线的颜色为蓝色
    Plot->graph(1)->setName("电压曲线");
    Plot->graph(1)->setVisible(false);		// 曲线不显示
    Plot->graph(2)->setPen(QPen(Qt::green));   //设置第二根线的颜色为蓝色
    Plot->graph(2)->setName("力矩曲线");
    Plot->graph(2)->setVisible(false);		// 曲线不显示
    Plot->graph(3)->setPen(QPen(Qt::yellow));   //设置第二根线的颜色为蓝色
    Plot->graph(3)->setName("实际位置曲线");
    Plot->graph(3)->setVisible(false);		// 曲线不显示
    Plot->graph(4)->setPen(QPen(Qt::black));   //设置第二根线的颜色为蓝色
    Plot->graph(4)->setName("理论位置曲线");
    Plot->graph(4)->setVisible(false);		// 曲线不显示
    Plot->yAxis->setRange(0,1000);
    Plot->xAxis->setRange(0,100);
//    Plot->xAxis->setLabel("x轴");        //设置x轴的标签
//    Plot->yAxis->setLabel("y轴");        //设置y轴的标签
    Plot->rescaleAxes(true);
    Plot->yAxis->rescale(true);          //让y轴自动随着数变化
    //Plot->xAxis->rescale(true);          //让x轴自动随着数变化
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

/**
处理数据
目标：电机电流，电机电压，力矩大小,电机实际位置，电机理论位置
数据格式确定："Z#%f#X#%f#\r\n"
然后有一个显示波形选择的，就可以了
其中：
 * "I:  单位mA
 * "V:  单位 mV
 * "T:  单位 Nm
 * "A:  单位 弧度
 * "M:  单位 弧度
显示方案：
*/
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
/*图形显示函数*/
void Widget::GraphShow()
{
    QString Str;
    Str+=QString::number(state0);
    Str+=QString::number(state1);
    Str+=QString::number(state2);
    Str+=QString::number(state3);
    Str+=QString::number(state4);
    int tmp = Str.toInt();
    switch (tmp)
    {
        case 11000:{//输出电流和电压波形
        Plot->graph(0)->setData(XData,YData);
        Plot->graph(1)->setData(XData,Y1Data);
        }break;

        case 10100:{//输出电流和力矩波形
        Plot->graph(0)->setData(XData,YData);
        Plot->graph(2)->setData(XData,Y1Data);
        }break;

        case 10010:{//输出电流和实际位置
        Plot->graph(0)->setData(XData,YData);
        Plot->graph(3)->setData(XData,Y1Data);
        }break;
        case 10001:{//输出电流和理论位置
        Plot->graph(0)->setData(XData,YData);
        Plot->graph(4)->setData(XData,Y1Data);
        }break;

        case 1100:{//输出电压和力矩波形
        Plot->graph(1)->setData(XData,YData);
        Plot->graph(2)->setData(XData,Y1Data);
        }break;

        case 1010:{//输出电压和实际位置
        Plot->graph(1)->setData(XData,YData);
        Plot->graph(3)->setData(XData,Y1Data);
        }break;

        case 1001:{//输出电压和理论位置
        Plot->graph(1)->setData(XData,YData);
        Plot->graph(4)->setData(XData,Y1Data);

        }break;

        case 110:{//输出力矩和实际位置
        Plot->graph(2)->setData(XData,YData);
        Plot->graph(3)->setData(XData,Y1Data);

        }break;

        case 101:{//输出力矩和理论位置
        Plot->graph(2)->setData(XData,YData);
        Plot->graph(4)->setData(XData,Y1Data);

        }break;

        case 11:{//输出实际和理论位置
        Plot->graph(3)->setData(XData,YData);
        Plot->graph(4)->setData(XData,Y1Data);
        }break;
        default:break;
    }
    Plot->yAxis->setRange(0,1000);
    Plot->xAxis->setRange(0,100);
    Plot->yAxis->rescale(true);
    //Plot->xAxis->rescale(true);
    Plot->replot();

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
        GetData(QString(temp));
        GraphShow();
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
    ui->textEdit_5->clear();
    ui->textEdit_3->clear();
    ui->textEdit_4->clear();
    ui->textEdit->clear();
    ui->textEdit_9->clear();
    ui->textEdit_10->clear();
    ui->textEdit_2->clear();
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
        if(max==2)//发送显示哪条曲线的指令给下位机，下位机再根据指令打印相关数据
        {
            QByteArray Data3;
            QString sendStr3="B";
            sendStr3+=QString::number(state0);
            sendStr3+=QString::number(state1);
            sendStr3+=QString::number(state2);
            sendStr3+=QString::number(state3);
            sendStr3+=QString::number(state4);
            sendStr3+='#';
            Data3 = sendStr3.toUtf8();
            SerialPort.write(Data3);
           // qDebug()<<Data3<<endl;
        }
        else if(max==0)
        {
            QMessageBox::information(NULL, "Warnning", "请选择曲线");

        }
        else if(max==1)
        {
            QMessageBox::information(NULL, "Warnning", "请选择两条曲线");

        }
        ui->pushButtonCR->setText("暂停");
    }

}

//选好曲线后开始画图按钮
void Widget::on_pushButton_clicked()
{
   start=1;
   if(max==2)//发送显示哪条曲线的指令给下位机，下位机再根据指令打印相关数据
   {
       QByteArray Data3;
       QString sendStr3="B";
       sendStr3+=QString::number(state0);
       sendStr3+=QString::number(state1);
       sendStr3+=QString::number(state2);
       sendStr3+=QString::number(state3);
       sendStr3+=QString::number(state4);
       sendStr3+='#';
       Data3 = sendStr3.toUtf8();
       SerialPort.write(Data3);
      // qDebug()<<Data3<<endl;
   }
   else if(max==0)
   {
       QMessageBox::information(NULL, "Warnning", "请选择曲线");

   }
   else if(max==1)
   {
       QMessageBox::information(NULL, "Warnning", "请选择两条曲线");

   }
   else
   {
       QMessageBox::information(NULL, "Error", "曲线超过2条");

   }
}

/*checkBox 函数，用*/
//是否显示电流大小曲线
void Widget::on_checkBox_stateChanged(int arg1)
{
    if(ui->checkBox->isChecked())
        {
           Plot->graph(0)->setVisible(true);		// 曲线显示
           max++;
           state0=1;
        }

        else
        {
           Plot->graph(0)->setVisible(false);		// 曲线不显示
           max--;
           state0=0;
        }
    if(max==0)//没用勾选东西的时候
    {
        Clk=0;//没用勾选东西的时候，x从0开始
    }
}
//是否显示电压大小曲线
void Widget::on_checkBox_2_stateChanged(int arg1)
{
    if(ui->checkBox_2->isChecked())
        {
           Plot->graph(1)->setVisible(true);		// 曲线显示
           max++;
           state1=1;
        }
        else
        {
           Plot->graph(1)->setVisible(false);		// 曲线不显示
           max--;
           state1=0;
        }
    if(max==0)//没用勾选东西的时候
    {
        Clk=0;//没用勾选东西的时候，x从0开始
    }
}
//是否显示力矩大小曲线
void Widget::on_checkBox_3_stateChanged(int arg1)
{
    if(ui->checkBox_3->isChecked())
        {
           Plot->graph(2)->setVisible(true);		// 曲线显示
           max++;
           state2=1;
        }
        else
        {
           Plot->graph(2)->setVisible(false);		// 曲线不显示
           max--;
           state2=0;
        }
    if(max==0)//没用勾选东西的时候
    {
        Clk=0;//没用勾选东西的时候，x从0开始
    }
}
//是否显示实际位置曲线
void Widget::on_checkBox_4_stateChanged(int arg1)
{
    if(ui->checkBox_4->isChecked())
        {
           Plot->graph(3)->setVisible(true);		// 曲线显示
           max++;
           state3=1;
        }
        else
        {
           Plot->graph(3)->setVisible(false);		// 曲线不显示
           max--;
           state3=0;
        }
    if(max==0)//没用勾选东西的时候
    {
        Clk=0;//没用勾选东西的时候，x从0开始
    }
}
//是否显示理论位置曲线
void Widget::on_checkBox_5_stateChanged(int arg1)
{
    if(ui->checkBox_5->isChecked())
        {
           Plot->graph(4)->setVisible(true);		// 曲线显示
           max++;
           state4=1;
        }
        else
        {
           Plot->graph(4)->setVisible(false);		// 曲线不显示
           max--;
           state4=0;
        }
    if(max==0)//没用勾选东西的时候
    {
        Clk=0;//没用勾选东西的时候，x从0开始
    }
}
//是否显示误差曲线
void Widget::on_checkBox_6_stateChanged(int arg1)
{
    if(max==0)//没用勾选东西的时候
    {
        Clk=0;//没用勾选东西的时候，x从0开始
    }

}

/*发送数据给下位机:Pid参数*/
//速度环与位置环W
void Widget::on_pushButton_3_clicked()//P
{
        if( ui->textEdit_3->document()->isEmpty())
        {
                QMessageBox::information(NULL, "Warnning", "请输入数据");
        }
        else{
            QByteArray Data;
            QString sendStr="W";
            sendStr+= ui->textEdit_3->toPlainText();
            sendStr+='P';
            Data = sendStr.toUtf8();
            SerialPort.write(Data);
          //  qDebug()<<Data<<endl;
        }
}

void Widget::on_pushButton_4_clicked()//I
{
    if( ui->textEdit_4->document()->isEmpty())
    {
            QMessageBox::information(NULL, "Warnning", "请输入数据");
    }
    else{
        QByteArray Data1;
        QString sendStr1="W";
        sendStr1+= ui->textEdit_4->toPlainText();
        sendStr1+='I';
        Data1 = sendStr1.toUtf8();
        SerialPort.write(Data1);
       // qDebug()<<Data1<<endl;
    }

}

void Widget::on_pushButton_5_clicked()//D
{
    if( ui->textEdit_5->document()->isEmpty())
    {
            QMessageBox::information(NULL, "Warnning", "请输入数据");
    }
    else{
        QByteArray Data2;
        QString sendStr2="W";
        sendStr2+= ui->textEdit_5->toPlainText();
        sendStr2+='D';
        Data2 = sendStr2.toUtf8();
        SerialPort.write(Data2);
       // qDebug()<<Data2<<endl;
    }

}

//电流环V
void Widget::on_pushButton_2_clicked()//P
{
    if( ui->textEdit->document()->isEmpty())
    {
            QMessageBox::information(NULL, "Warnning", "请输入数据");
    }
    else{
        QByteArray Data;
        QString sendStr="V";
        sendStr+= ui->textEdit->toPlainText();
        sendStr+='P';
        Data = sendStr.toUtf8();
        SerialPort.write(Data);
      //  qDebug()<<Data<<endl;
    }
}

void Widget::on_pushButton_9_clicked()//I
{
    if( ui->textEdit_9->document()->isEmpty())
    {
            QMessageBox::information(NULL, "Warnning", "请输入数据");
    }
    else{
        QByteArray Data;
        QString sendStr="V";
        sendStr+= ui->textEdit_9->toPlainText();
        sendStr+='I';
        Data = sendStr.toUtf8();
        SerialPort.write(Data);
      //  qDebug()<<Data<<endl;
    }
}

void Widget::on_pushButton_10_clicked()//D
{
    if( ui->textEdit_10->document()->isEmpty())
    {
            QMessageBox::information(NULL, "Warnning", "请输入数据");
    }
    else{
        QByteArray Data;
        QString sendStr="V";
        sendStr+= ui->textEdit_10->toPlainText();
        sendStr+='D';
        Data = sendStr.toUtf8();
        SerialPort.write(Data);
      //  qDebug()<<Data<<endl;
    }

}


//每次重新画波形的时候需要清空原有波形数据和显示
void Widget::on_pushButton_17_clicked()
{
   Clk=0;
   for(int i=0;i<3000;i++)
   {
       XData[i]=0;
       YData[i]=0;
       Y1Data[i]=0;
   }

   ui->pushButtonCR->setText("暂停");
   //取消勾选
   ui->checkBox->setCheckState(Qt::Unchecked);
   ui->checkBox_2->setCheckState(Qt::Unchecked);
   ui->checkBox_3->setCheckState(Qt::Unchecked);
   ui->checkBox_4->setCheckState(Qt::Unchecked);
   ui->checkBox_5->setCheckState(Qt::Unchecked);
   ui->checkBox_6->setCheckState(Qt::Unchecked);
}
