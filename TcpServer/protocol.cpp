#include "protocol.h"
#include "mytcpserver.h"

#include <QDir>
#include <QDate>
#include <QFileInfo>

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
    qDebug() << "Enter PDU* handleRegistRequest(PDU* pdu)";

    char caName[32] = {'\0'};
    char caPwd[32] = {'\0'};
    // 拷贝读取的信息
    strncpy(caName, pdu -> caData, 32);
    strncpy(caPwd, pdu -> caData + 32, 32);
    bool ret = DBOperate::getInstance().handleRegist(caName, caPwd); // 处理请求，插入数据库

    // 响应客户端
    PDU *resPdu = mkPDU(0); // 响应消息
    resPdu -> uiMsgType = ENUM_MSG_TYPE_REGIST_RESPOND;
    if(ret)
    {
        strcpy(resPdu -> caData, REGIST_OK);
        // 注册成功，为新用户按用户名创建文件夹
        QDir dir;
        ret = dir.mkdir(QString("%1/%2").arg(MyTcpServer::getInstance().getStrRootPath()).arg(caName));
        qDebug() << "创建新用户文件夹" << ret;
    }
    if(!ret)
    {
        strcpy(resPdu -> caData, REGIST_FAILED);
    }

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
        PDU *resPdu = NULL; // 响应消息
        if(ret)
        {
            QString strUserRootPath = QString("%1/%2")
                    .arg(MyTcpServer::getInstance().getStrRootPath()).arg(caName); // 用户文件系统根目录
            qDebug() << "登录用户的路径：" << strUserRootPath;
            resPdu = mkPDU(strUserRootPath.size() + 1);
            memcpy(resPdu -> caData, LOGIN_OK, 32);
            memcpy(resPdu -> caData + 32, caName, 32); // 将登录后的用户名传回，便于tcpclient确认已经登陆的用户名
            // 在登陆成功时，记录Socket对应的用户名
            m_strName = caName;
            // qDebug() << "m_strName: " << m_strName;
            // 返回用户的根目录
            strncpy((char*)resPdu -> caMsg, strUserRootPath.toStdString().c_str(), strUserRootPath.size() + 1);
        }
        else
        {
            resPdu = mkPDU(0);
            strcpy(resPdu -> caData, LOGIN_FAILED);
        }
        resPdu -> uiMsgType = ENUM_MSG_TYPE_LOGIN_RESPOND;
        qDebug() << "登录处理：" << resPdu -> uiMsgType << " " << resPdu ->caData << " " << resPdu ->caData + 32;

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

PDU* handleAddFriendRequest(PDU* pdu){
    char addedName[32] = {'\0'};
        char sourceName[32] = {'\0'};
        // 拷贝读取的信息
        strncpy(addedName, pdu -> caData, 32);
        strncpy(sourceName, pdu -> caData + 32, 32);
        qDebug() << "handleAddFriendRequest  " << addedName << " " << sourceName;
        int iSearchUserStatus = DBOperate::getInstance().handleAddFriend(addedName, sourceName);
        // 0对方存在不在线，1对方存在在线，2不存在，3已是好友，4请求错误
        PDU* resPdu = NULL;

        switch (iSearchUserStatus) {
        case 0: // 0对方存在不在线
        {
            resPdu = mkPDU(0);
            resPdu -> uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
            strcpy(resPdu -> caData, ADD_FRIEND_OFFLINE);
            break;
        }
        case 1: // 1对方存在在线
        {
            // 需要转发给对方请求添加好友消息
            MyTcpServer::getInstance().forwardMsg(addedName, pdu);

            resPdu = mkPDU(0);
            resPdu -> uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
            strcpy(resPdu -> caData, ADD_FRIEND_OK); // 表示加好友请求已发送
            break;
        }
        case 2: // 2用户不存在
        {
            resPdu = mkPDU(0);
            resPdu -> uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
            strcpy(resPdu -> caData, ADD_FRIEND_EMPTY);
            break;
        }
        case 3: // 3已是好友
        {
            resPdu = mkPDU(0);
            resPdu -> uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
            strcpy(resPdu -> caData, ADD_FRIEND_EXIST);
            break;
        }
        case 4: // 4请求错误
        {
            resPdu = mkPDU(0);
            resPdu -> uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
            strcpy(resPdu -> caData, UNKNOWN_ERROR);
            break;
        }
        default:
            break;
        }
        return resPdu;
}

PDU* handleFlushFriendRequest(PDU* pdu){

    qDebug() << "Enter PDU* handleFlushFriendRequest(PDU* pdu)";

    char caName[32] = {'\0'};

    strncpy(caName, pdu -> caData, 32);

    QStringList strList = DBOperate::getInstance().handleFlushFriendRequest(caName);
    qDebug() << "caName = " << caName;

    uint uiMsgLen = strList.size() / 2 * 36; // 36 char[32] 好友名字+ 4 int 在线状态

    PDU* resPdu = mkPDU(uiMsgLen);
    resPdu -> uiMsgType = ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND;
    for(int i = 0; i * 2 < strList.size(); ++ i)
    {
        strncpy((char*)(resPdu -> caMsg) + 36 * i, strList.at(i * 2).toStdString().c_str(), 32);
        strncpy((char*)(resPdu -> caMsg) + 36 * i + 32, strList.at(i * 2 + 1).toStdString().c_str(), 4);
    }

    return resPdu;
}

PDU* handleDeleteFriendRequest(PDU* pdu){

    qDebug() << "Enter PDU* handleDeleteFriendRequest(PDU* pdu)";

    char deletedName[32] = {'\0'};
    char sourceName[32] = {'\0'};
    // 拷贝读取的信息
    strncpy(deletedName, pdu -> caData, 32);
    strncpy(sourceName, pdu -> caData + 32, 32);
    bool ret = DBOperate::getInstance().handleDeleteFriend(deletedName, sourceName);

    // 给请求删除方消息提示，以返回值形式
    PDU *resPdu = mkPDU(0);
    resPdu -> uiMsgType = ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND;
    if(ret)
    {
        strncpy(resPdu -> caData, DEL_FRIEND_OK, 32);
    }
    else
    {
        strncpy(resPdu -> caData, DEL_FRIEND_FAILED, 32);
    }

    // 给被删除方消息提示，如果在线的话
    MyTcpServer::getInstance().forwardMsg(deletedName, pdu);

    return resPdu;
}

// 私聊发送消息请求
PDU* handlePrivateChatRequest(PDU* pdu)
{
    qDebug() << "Enter PDU* handlePrivateChatRequest(PDU* pdu)";

    char chatedName[32] = {'\0'};
    char sourceName[32] = {'\0'};
    // 拷贝读取的信息
    strncpy(chatedName, pdu -> caData, 32);
    strncpy(sourceName, pdu -> caData + 32, 32);
    qDebug() << "handlePrivateChatRequest  " << chatedName << " " << sourceName;

    PDU* resPdu = NULL;

    // 转发给对方消息  0对方存在不在线，1对方存在在线
    bool ret = MyTcpServer::getInstance().forwardMsg(chatedName, pdu);

    // 发送失败则给发送者消息
    if(!ret)// 0对方不在线
    {
        qDebug() << "对方不在线";
        resPdu = mkPDU(0);
        resPdu -> uiMsgType = ENUM_MSG_TYPE_PRIVATE_CHAT_RESPOND;
        strcpy(resPdu -> caData, PRIVATE_CHAT_OFFLINE);
    }

    return resPdu;
}

// 创建文件夹请求处理
PDU* handleCreateDirRequest(PDU* pdu)
{

    qDebug() << "Enter PDU* handleCreateDirRequest(PDU* pdu)";

    char caDirName[32];
    char caCurPath[pdu -> uiMsgLen];
    strncpy(caDirName, pdu -> caData, 32);
    strncpy(caCurPath, (char*)pdu -> caMsg, pdu -> uiMsgLen);

    QString strDir = QString("%1/%2").arg(caCurPath).arg(caDirName);
    QDir dir;
    PDU *resPdu = mkPDU(0);
    resPdu -> uiMsgType = ENUM_MSG_TYPE_CREATE_DIR_RESPOND;

    qDebug() << "创建文件夹：" << strDir;
    if(dir.exists(caCurPath)) // 路径存在
    {
        if(dir.exists(strDir)) // 文件夹已经存在
        {
            strncpy(resPdu -> caData, CREATE_DIR_EXIST, 32);
        }
        else
        {
            dir.mkdir(strDir); // 创建文件夹
            strncpy(resPdu -> caData, CREATE_DIR_OK, 32);
        }
    }
    else // 路径不存在
    {
        strncpy(resPdu -> caData, PATH_NOT_EXIST, 32);
    }

    return resPdu;
}

// 刷新文件夹请求处理
PDU* handleFlushDirRequest(PDU* pdu)
{
    qDebug() << "Enter PDU* handleFlushDirRequest(PDU* pdu)";

    char caCurDir[pdu -> uiMsgLen];
    memcpy(caCurDir, (char*)pdu -> caMsg, pdu -> uiMsgLen);
    qDebug() << "刷新文件夹：" << caCurDir;
    QDir dir;
    PDU* resPdu = NULL;

    if(!dir.exists(caCurDir)) // 请求文件夹不存在
    {
        resPdu = mkPDU(0);
        strncpy(resPdu -> caData, PATH_NOT_EXIST, 32);
    }
    else // 存在
    {
        dir.setPath(caCurDir); // 设置为当前目录
        QFileInfoList fileInfoList = dir.entryInfoList(); // 获取当前目录下所有文件
        int iFileNum = fileInfoList.size();

        resPdu = mkPDU(sizeof(FileInfo) * iFileNum);
        FileInfo *pFileInfo = NULL; // 创建一个文件信息结构体指针，方便之后遍历PDU空间来赋值

        for(int i = 0; i < iFileNum; ++ i)
        {
            pFileInfo = (FileInfo*)(resPdu -> caMsg) + i; // 通过指针指向，直接修改PDU空间值，每次偏移FileInfo大小
            memcpy(pFileInfo -> caName, fileInfoList[i].fileName().toStdString().c_str(), fileInfoList[i].fileName().size());
            pFileInfo -> bIsDir = fileInfoList[i].isDir();
            pFileInfo -> uiSize = fileInfoList[i].size();
            QDateTime dtLastTime = fileInfoList[i].lastModified(); // 获取文件最后修改时间
            QString strLastTime = dtLastTime.toString("yyyy/MM/dd hh:mm");
            memcpy(pFileInfo -> caTime, strLastTime.toStdString().c_str(), strLastTime.size());
            qDebug() << "文件信息：" << pFileInfo -> caName << " " << pFileInfo -> bIsDir << " " << pFileInfo -> uiSize << " " << pFileInfo -> caTime;
        }
    }
    resPdu -> uiMsgType = ENUM_MSG_TYPE_FLUSH_DIR_RESPOND;

    return resPdu;
}
