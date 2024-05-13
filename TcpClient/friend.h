#ifndef FRIEND_H
#define FRIEND_H

#include <QWidget>
#include <QTextEdit>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QInputDialog>

#include "protocol.h"
#include "online.h"

class Friend : public QWidget
{
    Q_OBJECT
public:
    explicit Friend(QWidget *parent = nullptr);
    void showOnlineUser(PDU* pdu); // 显示在线好友
    QString getStrSearchName() const;

signals:

public slots:
    void showOnline();
    void searchUser(); // 处理查找用户按钮点击信号的槽函数

private:
    QTextEdit* m_pShowMsgTE;
    QListWidget* m_pFriendListwidget;
    QLineEdit* m_pInputMsgLE;

    QPushButton* m_pDelFriendPB;
    QPushButton* m_pFlushFriendPB;
    QPushButton* m_pShowOnlineUsrPB;
    QPushButton* m_pSearchUsrPB;
    QPushButton* m_pMsgSendPB;
    QPushButton* m_pPrivateChatPB;

    Online* m_pOnline; // 所有在线用户页面

    QString m_strSearchName;          // 查找的用户的名字

};

#endif // FRIEND_H
