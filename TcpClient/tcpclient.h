#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QFile> // 操作文件，在这里主要为了启动时访问配置文件
#include <QTcpSocket> // 建立TCP会话连接，为了实现TCP客户端连接服务器及与数据库交互
#include <openwidget.h>

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

    QString getStrCurPath() const;
    void setStrCurPath(const QString &strCurPath);

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
    QString m_strRootPath;  // 用户根目录
    QString m_strCurPath;   // 当前目录
};
#endif // TCPCLIENT_H
