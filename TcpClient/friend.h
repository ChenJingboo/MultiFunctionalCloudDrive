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
#include "privatechatwid.h"

class Friend : public QWidget
{
    Q_OBJECT
public:
    explicit Friend(QWidget *parent = nullptr);
    void showOnlineUser(PDU* pdu); // 显示在线好友
    void updateFriendList(PDU *pdu); // 刷新好友列表
    QString getStrSearchName() const;

signals:

public slots:
    void showOnline();
    void searchUser();      // 处理查找用户按钮点击信号的槽函数
    void flushFriendList(); // 刷新好友按钮的槽函数
    void deleteFriend();    // 删除好友
    void privateChat();     // 私聊好友
    privateChatWid* searchPriChatWid(const char* chatName); // 获得对应用户名的私聊窗口
    void insertPriChatWidList(privateChatWid* priChat); // 将私聊窗口插入到私聊窗口List

private:
    QTextEdit* m_pShowMsgTE;
    QListWidget* m_pFriendListwidget;   //好友列表
    QLineEdit* m_pInputMsgLE;

    QPushButton* m_pDelFriendPB;    //删除好友
    QPushButton* m_pFlushFriendPB;  //刷新好友列表
    QPushButton* m_pShowOnlineUsrPB;//显示在线好友
    QPushButton* m_pSearchUsrPB;    //搜索好友
    QPushButton* m_pMsgSendPB;      //信息发送按钮
    QPushButton* m_pPrivateChatPB;  //私聊

    Online* m_pOnline; // 所有在线用户页面

    QString m_strSearchName;          // 查找的用户的名字
    QList<privateChatWid*> m_priChatWidList; // 所有私聊的窗口

};

#endif // FRIEND_H
