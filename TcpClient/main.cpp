#include <QApplication>

#include "tcpclient.h"
#include "online.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //采用单例模式的公共获取例子接口
    TcpClient::getInstance().show();

    return a.exec();
}
