#include "mytcpsocket.h"
#include "mytcpserver.h"
#include "dboperate.h"

MyTcpSocket::MyTcpSocket()
{
    connect(this, SIGNAL(readyRead()),
            this, SLOT(recvMsg()));

    connect(this, SIGNAL(disconnected()),
            this, SLOT(clientOffline()));

}

void MyTcpSocket::recvMsg()
{
    qDebug() << this -> bytesAvailable(); // 输出接收到的数据大小
    uint uiPDULen = 0;
    this -> read((char*)&uiPDULen, sizeof(uint)); // 先读取uint大小的数据，首个uint正是总数据大小
    uint uiMsgLen = uiPDULen - sizeof(PDU); // 实际消息大小，sizeof(PDU)只会计算结构体大小，而不是分配的大小
    PDU *pdu = mkPDU(uiMsgLen);
    this -> read((char*)pdu + sizeof(uint), uiPDULen - sizeof(uint)); // 接收剩余部分数据（第一个uint已读取）
    // qDebug() << pdu -> uiMsgType << ' ' << (char*)pdu -> caMsg; // 输出

    // 根据不同消息类型，执行不同操作
    PDU* resPdu = NULL; // 响应报文
    switch(pdu -> uiMsgType)
    {
        case ENUM_MSG_TYPE_REGIST_REQUEST: // 注册请求
        {
            resPdu = handleRegistRequest(pdu); // 请求处理
            break;
        }
        case ENUM_MSG_TYPE_LOGIN_REQUEST: // 登录请求
        {
            qDebug() << "Accept case ENUM_MSG_TYPE_LOGIN_REQUEST" ;
            resPdu = handleLoginRequest(pdu, m_strName);
            break;
        }
        case ENUM_MSG_TYPE_ALL_ONLINE_REQUEST: // 显示在线好友请求
        {
            qDebug() << "Accept case ENUM_MSG_TYPE_ALL_ONLINE_REQUEST" ;
            resPdu = handleOnlineUsersRequest();
            break;
        }
        case ENUM_MSG_TYPE_SEARCH_USER_REQUEST: // 查找用户请求
        {
            qDebug() << "Accept case ENUM_MSG_TYPE_SEARCH_USER_REQUEST" ;
            resPdu = handleSearchUserRequest(pdu);
            break;
        }
        case ENUM_MSG_TYPE_FLSUH_FRIEND_REQUEST: // 刷新好友请求
        {
            qDebug() << "Accept case ENUM_MSG_TYPE_SEARCH_USER_REQUEST" ;
            resPdu = handleFlushFriendRequest(pdu);
            break;
        }
        default:
            break;
    }

    // 响应客户端
    if(NULL != resPdu)
    {
        qDebug() << resPdu -> uiMsgType << " " << resPdu ->caData;
        this -> write((char*)resPdu, resPdu -> uiPDULen);
        // 释放空间
        free(resPdu);
        resPdu = NULL;
    }
    // 释放空间
    free(pdu);
    pdu = NULL;
}

// 同意加好友
void handleAddFriendAgree(PDU* pdu)
{
    char addedName[32] = {'\0'};
    char sourceName[32] = {'\0'};
    // 拷贝读取的信息
    strncpy(addedName, pdu -> caData, 32);
    strncpy(sourceName, pdu -> caData + 32, 32);

    // 将新的好友关系信息写入数据库
    DBOperate::getInstance().handleAddFriendAgree(addedName, sourceName);

    // 服务器需要转发给发送好友请求方其被同意的消息
    MyTcpServer::getInstance().forwardMsg(sourceName, pdu);
}

// 拒绝加好友
void handleAddFriendReject(PDU* pdu)
{
    char sourceName[32] = {'\0'};
    // 拷贝读取的信息
    strncpy(sourceName, pdu -> caData + 32, 32);
    // 服务器需要转发给发送好友请求方其被拒绝的消息
    MyTcpServer::getInstance().forwardMsg(sourceName, pdu);
}

void MyTcpSocket::clientOffline()
{
    DBOperate::getInstance().handleOffline(m_strName.toStdString().c_str());
    emit offline(this); // 发送给mytcpserver该socket删除信号
}

QString MyTcpSocket::getStrName()
{
    return m_strName;
}
