#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QString>
#include <QTcpSocket> // 建立TCP会话连接，为了实现TCP客户端连接服务器及与数据库交互
#include <QMessageBox>
#include <QPushButton>
#include <QDebug>
#include <QFile>
#include <QHostAddress>

#include "protocol.h"
class OpenWidget;   //前置声明

QT_BEGIN_NAMESPACE
namespace Ui { class TcpClient; }
QT_END_NAMESPACE

class TcpClient : public QWidget
{
    Q_OBJECT

public:
    TcpClient(QWidget *parent = nullptr);
    void loadConfig(); // 加载配置文件信息
    void sendText();
    static TcpClient& getInstance();
    QTcpSocket& getTcpSocket();
    QString getStrName() const;
    ~TcpClient();

public slots:
    void showConnect(); // 检测服务器是否连接成功
    void recvMsg();

private slots:
    void on_pushButton_login_clicked();

    void on_pushButton_register_clicked();

    void on_pushButton_logout_clicked();

private:
    Ui::TcpClient *ui;
    QTcpSocket m_tcpSocket; // TCPSocket用来和服务器连接与交互
    QString m_strIP;        // 存储配置文件读取到的IP地址
    quint16 m_usPort;       // 无符号16位整型 存储配置文件的端口号
    QString m_strName;      // 该客户端用户名
};
#endif // TCPCLIENT_H
