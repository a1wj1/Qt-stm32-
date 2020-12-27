#ifndef WIDGET_H
#define WIDGET_H
#define MAX_XCOUNT       3000	// x轴可显示的最大范围
#define MAX_YCOUNT       3000	// y轴可显示的最大范围
#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include "qcustomplot.h"
#include <QVector>
namespace Ui {
class Widget;
}

class Widget : public QWidget//继承关系QWidget
{
    Q_OBJECT


public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    void InitPort();//初始化串口
    void ReadPort();//读取串口
    void GraphShow();//画图
    void GetData(QString);//接收数据
private slots://信号触发事件，在这个区内声明的槽意味着任何对象都可将信号与之相连接。
    void on_pushButtonS_clicked();

    void on_pushButtonCW_clicked();

    void on_pushButtonCR_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_checkBox_3_stateChanged(int arg1);

    void on_checkBox_4_stateChanged(int arg1);

    void on_checkBox_5_stateChanged(int arg1);

    void on_checkBox_6_stateChanged(int arg1);

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_17_clicked();
//私有成员属性
private:
    Ui::Widget *ui;
    QTimer *Timer;
    QSerialPort SerialPort;//用于设置串口
    QSerialPortInfo SerialPortInfo;//用于得到串口信息
    int Clk ;//x轴的时间数
    int max;//勾选计数
    int start;//开始的标志位
    int state0,state1,state2,state3,state4;//勾选计数
    QCustomPlot *Plot;//定义一个窗口绘图的句柄
    QVector <double>    XData;//x轴数据
    QVector <double>    X1Data;//x轴数据
    QVector <double>    X2Data;//x轴数据
    QVector <double>    X3Data;//x轴数据
    QVector <double>    X4Data;//x轴数据
    QVector <double>    YData;//线1的y轴数据
    QVector <double>    Y1Data;//线2的y轴数据
    QVector <double>    Y2Data;
    QVector <double>    Y3Data;
    QVector <double>    Y4Data;


};

#endif // WIDGET_H










