#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QDebug>

#include <stdlib.h>
#include <unistd.h> // Unix库函数，包含了read等系统服务函数
#include <string.h>
#include "mytcpsocket.h"
#include "dboperate.h"

// 定义响应消息
// 注册
#define REGIST_OK "regist ok"
#define REGIST_FAILED "regist failed"

// 登录
#define LOGIN_OK "login ok"
#define LOGIN_FAILED "login failed"

// 好友操作
#define SEARCH_USER_OK "search user ok"                  // 查找
#define SEARCH_USER_OFFLINE "user offline"
#define SEARCH_USER_EMPTY "no such people"

#define ADD_FRIEND_OK "add friend ok"                    // 添加好友 1对方存在在线，0对方存在不在线，2不存在，3已是好友，4请求错误
#define ADD_FRIEND_OFFLINE "added friend offline"
#define ADD_FRIEND_EMPTY "no such people"
#define ADD_FRIEND_EXIST "added friend already exist"

#define DEL_FRIEND_OK "delete friend ok"                 // 删除好友
#define DEL_FRIEND_FAILED "delete friend failed"

#define UNKNOWN_ERROR "unknown error"                    // 通用未知错误

#define PRIVATE_CHAT_OFFLINE "the friend offline"        // 私聊好友

// 文件操作
#define PATH_NOT_EXIST "path does not exist"             // 文件路径不存在
#define CREATE_DIR_OK "create dir ok"                    // 新建文件夹
#define CREATE_DIR_EXIST "created dir already exist"

//typedef <current_name> <new_name>
typedef unsigned int uint;

enum ENUM_MSG_TYPE
{
    ENUM_MSG_TYPE_MIN = 0,

    ENUM_MSG_TYPE_REGIST_REQUEST, //注册请求
    ENUM_MSG_TYPE_REGIST_RESPOND, //注册回复

    ENUM_MSG_TYPE_LOGIN_REQUEST,  // 登录请求
    ENUM_MSG_TYPE_LOGIN_RESPOND,  // 登录回复

    ENUM_MSG_TYPE_ALL_ONLINE_REQUEST,       // 在线用户请求
    ENUM_MSG_TYPE_ALL_ONLINE_RESPOND, // 在线用户回复

    ENUM_MSG_TYPE_SEARCH_USER_REQUEST, // 搜索用户请求
    ENUM_MSG_TYPE_SEARCH_USER_RESPOND, // 搜索用户回复

    ENUM_MSG_TYPE_ADD_FRIEND_REQUEST, // 添加好友请求
    ENUM_MSG_TYPE_ADD_FRIEND_RESPOND, // 添加好友回复

    ENUM_MSG_TYPE_ADD_FRIEND_AGREE,     // 被添加好友消息回复同意
    ENUM_MSG_TYPE_ADD_FRIEND_REJECT,    // 被添加好友消息回复拒绝

    ENUM_MSG_TYPE_FLSUH_FRIEND_REQUEST, // 刷新好友请求
    ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND, // 刷新好友响应

    ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST, // 删除好友请求
    ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND, // 删除好友响应

    ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST, // 私聊请求
    ENUM_MSG_TYPE_PRIVATE_CHAT_RESPOND, // 私聊回复

    ENUM_MSG_TYPE_GROUP_CHAT_REQUEST, // 群聊请求
    ENUM_MSG_TYPE_GROUP_CHAT_RESPOND, // 群聊回复

    // 文件操作
    ENUM_MSG_TYPE_CREATE_DIR_REQUEST, // 新建文件夹请求
    ENUM_MSG_TYPE_CREATE_DIR_RESPOND, // 新建文件夹回复

    ENUM_MSG_TYPE_FLUSH_DIR_REQUEST, // 刷新文件夹请求
    ENUM_MSG_TYPE_FLUSH_DIR_RESPOND, // 刷新文件夹回复

    ENUM_MSG_TYPE_MAX = 0x00ffffff,// uint最大值 Oxffffffff
};

// 设计协议数据单元格式
// 知识点：弹性/柔性结构体
struct PDU
{
    uint uiPDULen;       // 总的协议数据单元大小
    uint uiMsgType;      // 消息类型
    char caData[64];     // 其他数据
    uint uiMsgLen;       // 实际消息长度
    int caMsg[];         // 实际消息，主要通过caMsg访问消息数据
};

// 文件信息结构体
struct FileInfo
{
    char caName[32];     // 文件名字
    bool bIsDir;         // 文件类型，是否为文件夹
    long long uiSize;         // 文件大小
    char caTime[128];    // 修改时间
};

PDU *mkPDU(uint uiMsgLen); // 创建PDU，uiMsglen是可变的，总大小可有其计算得到
PDU* handleRegistRequest(PDU* pdu); // 处理注册请求并返回响应PDU
PDU* handleLoginRequest(PDU* pdu, QString& m_strName); // 处理登录请求并返回响应PDU
PDU* handleOnlineUsersRequest();            // 处理查询所有在线用户的请求
PDU* handleSearchUserRequest(PDU* pdu);     // 处理查找用户的请求
PDU* handleAddFriendRequest(PDU* pdu);      // 处理添加好友请求
PDU* handleFlushFriendRequest(PDU* pdu);    // 刷新好友列表请求
PDU* handleDeleteFriendRequest(PDU* pdu);   // 删除好友请求
PDU* handlePrivateChatRequest(PDU* pdu);    // 私聊发送消息请求
PDU* handleCreateDirRequest(PDU* pdu);      // 创建文件夹请求处理
PDU* handleFlushDirRequest(PDU* pdu);       // 刷新文件夹请求处理

#endif // PROTOCOL_H
