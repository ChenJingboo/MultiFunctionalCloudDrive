#include "protocol.h"

PDU *mkPDU(uint uiMsgLen)
{
    uint uiPDULen = sizeof (PDU) + uiMsgLen;
    PDU* pdu = (PDU*)malloc(uiPDULen);

    if(nullptr == pdu)
    {
        exit(EXIT_FAILURE); // 错误退出程序
    }
    memset (pdu, 0, uiPDULen); // 数据初始化为0
    pdu -> uiPDULen = uiPDULen; // 数据参数初始化
    pdu -> uiMsgLen = uiMsgLen;

    return pdu;
}

// 处理注册请求并返回响应PDU
PDU* handleRegistRequest(PDU* pdu)
{
    char caName[32] = {'\0'};
    char caPwd[32] = {'\0'};
    // 拷贝读取的信息
    strncpy(caName, pdu -> caData, 32);
    strncpy(caPwd, pdu -> caData + 32, 32);
    qDebug() << pdu -> uiMsgType << " " << caName << " " << caPwd;
    bool ret = DBOperate::getInstance().handleRegist(caName, caPwd); // 处理请求，插入数据库

    // 响应客户端
    PDU *resPdu = mkPDU(0); // 响应消息
    resPdu -> uiMsgType = ENUM_MSG_TYPE_REGIST_RESPOND;
    if(ret)
    {
        strcpy(resPdu -> caData, REGIST_OK);
    }
    else
    {
        strcpy(resPdu -> caData, REGIST_FAILED);
    }
    // qDebug() << resPdu -> uiMsgType << " " << resPdu ->caData;

    return resPdu;
}

PDU* handleLoginRequest(PDU* pdu, QString& m_strName)
{
    char caName[32] = {'\0'};
    char caPwd[32] = {'\0'};
    // 拷贝读取的信息
    strncpy(caName, pdu -> caData, 32);
    strncpy(caPwd, pdu -> caData + 32, 32);
    qDebug() << pdu -> uiMsgType << " " << caName << " " << caPwd;
    bool ret = DBOperate::getInstance().handleLogin(caName, caPwd); // 处理请求，插入数据库

    // 响应客户端
    PDU *resPdu = mkPDU(0); // 响应消息
    resPdu -> uiMsgType = ENUM_MSG_TYPE_LOGIN_RESPOND;
    if(ret)
    {
        strcpy(resPdu -> caData, LOGIN_OK);
        // 在登陆成功时，记录Socket对应的用户名
        m_strName = caName;
    }
    else
    {
        strcpy(resPdu -> caData, LOGIN_FAILED);
    }
    // qDebug() << resPdu -> uiMsgType << " " << resPdu ->caData;

    return resPdu;
}

// 处理查询所有在线用户的请求
PDU* handleOnlineUsersRequest(){

    qDebug() << "Enter PDU* handleOnlineUsersRequest()";

    QStringList strList = DBOperate::getInstance().handleAllOnline(); // 查询请求，查询数据库所有在线用户
    uint uiMsgLen = strList.size() * 32; // 消息报文的长度

    // 响应客户端
    PDU *resPdu = mkPDU(uiMsgLen); // 响应消息
    resPdu -> uiMsgType = ENUM_MSG_TYPE_ALL_ONLINE_RESPOND;
    qDebug() << "在线用户数：" << strList.size();
    for(int i = 0; i < strList.size(); ++ i)
    {
        memcpy((char*)(resPdu -> caMsg) + 32 * i, strList[i].toStdString().c_str(), strList[i].size());
        qDebug() << "所有在线用户有：" << (char*)(resPdu -> caMsg) + 32 * i;
    }

    return resPdu;
}

PDU *handleSearchUserRequest(PDU *pdu)
{
    char caName[32] = {'\0'};
    strncpy(caName, pdu -> caData, 32);
    int ret = DBOperate::getInstance().handleSearchUser(caName); // 处理查找用户，0存在不在线，1存在并在线，2不存在

    // 响应客户端
    PDU *resPdu = mkPDU(0); // 响应消息
    resPdu -> uiMsgType = ENUM_MSG_TYPE_SEARCH_USER_RESPOND;
    if(ret == 1)
    {
        strcpy(resPdu -> caData, SEARCH_USER_OK);
    }
    else if(ret == 0)
    {
        strcpy(resPdu -> caData, SEARCH_USER_OFFLINE);
    }
    else
    {
        strcpy(resPdu -> caData, SEARCH_USER_EMPTY);
    }

    return resPdu;
}
