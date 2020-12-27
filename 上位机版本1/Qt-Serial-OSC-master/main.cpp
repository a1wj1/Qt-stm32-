#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);//创建一个QApplication类型的新实例，并调用该类的构造函数
    Widget w;//创建Widget
    w.show();//输出窗口
    w.setWindowTitle("串口示波器");
    return a.exec();
}


