#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QTcpSocket>
#include <QDebug>

#include "protocol.h"
#include "dboperate.h"

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();
    QString getStrName();

public slots:
    void recvMsg(); // 槽函数，按照协议形式处理传输过来的数据
    void clientOffline();


signals:
    void offline(MyTcpSocket *socket); // 通过信号传送给mytcpserver用户下线通知，然后附带参数socket地址方便删除

private:
    QString m_strName; // 用户名，为了能区分该Socket属于哪个用户

};

#endif // MYTCPSOCKET_H
