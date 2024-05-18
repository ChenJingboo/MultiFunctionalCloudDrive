#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include <QList>

#include "mytcpsocket.h"

class MyTcpServer : public QTcpServer
{
    Q_OBJECT // 类既要继承QObject又要写上宏Q_OBJECT，才能支持信号槽

public:
    MyTcpServer();
    static MyTcpServer& getInstance();
    void incomingConnection(qintptr handle) override;
    bool forwardMsg(const QString caDesName, PDU *pdu); // 服务器转发给其他客户端消息
    ~MyTcpServer();
    QString getStrRootPath() const;
    void setStrRootPath(const QString &strRootPath);

public slots:
    void deleteSocket(MyTcpSocket* mySocket);

private:
    QList<MyTcpSocket*> m_tcpSocketList; // 存储服务器所有已经建立的Socket连接
    QString m_strRootPath; // 文件系统根目录
};

#endif // MYTCPSERVER_H
