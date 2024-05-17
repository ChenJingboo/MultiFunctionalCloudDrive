#ifndef PRIVATECHATWID_H
#define PRIVATECHATWID_H

#include <QWidget>
#include <QMessageBox>

#include "protocol.h" //假如报错，就用前置声明

namespace Ui {
class privateChatWid;
}

class privateChatWid : public QWidget
{
    Q_OBJECT

public:
    explicit privateChatWid(QWidget *parent = nullptr);
    ~privateChatWid();

    QString strChatName() const;
    void setStrChatName(const QString &strChatName);

    QString strLoginName() const;
    void setStrLoginName(const QString &strLoginName);

    void updateShowMsgTE(PDU* pdu); // 更新showMsgTE聊天消息窗口的消息
    void setPriChatTitle(const char* caTitle); // 设置私聊聊天框的title label


private slots:
    void on_pushButton_send_clicked(); // 发送消息按钮的槽函数

private:
    Ui::privateChatWid *ui;

    QString m_strChatName;  // 聊天对象用户名
    QString m_strLoginName; // 请求用户名

};

#endif // PRIVATECHATWID_H
