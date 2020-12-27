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
    void GetData(QString);//接收数据
    QString formatInput(QString hexStr);
    char ConvertHexChar(char c);
    QByteArray QString2Hex(QString hexStr);
    QString decInt2HexStr(int dec);
private slots://信号触发事件，在这个区内声明的槽意味着任何对象都可将信号与之相连接。
    void on_pushButtonS_clicked();

    void on_pushButtonCW_clicked();

    void on_pushButtonCR_clicked();

    void on_pushButton_3_clicked();


    void on_pushButton_2_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_checkBox_clicked();

private:
    Ui::Widget *ui;
    QTimer *Timer;
    QSerialPort SerialPort;//用于设置串口
    QSerialPortInfo SerialPortInfo;//用于得到串口信息
    int Clk ;//x轴的时间数
    int max;
    int start;
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










