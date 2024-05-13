#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdlib.h>
#include <unistd.h> // Unix库函数，包含了read等系统服务函数
#include <string.h>

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


//typedef <current_name> <new_name>
typedef unsigned int uint;

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

PDU *mkPDU(uint uiMsgLen); // 创建PDU，uiMsglen是可变的，总大小可有其计算得到

//枚举方式，定义消息类型
enum ENUM_MSG_TYPE
{
    ENUM_MSG_TYPE_MINN = 0,

    ENUM_MSG_TYPE_REGIST_REQUEST, // 注册请求
    ENUM_MSG_TYPE_REGIST_RESPOND, // 注册回复

    ENUM_MSG_TYPE_LOGIN_REQUEST,  // 登录请求
    ENUM_MSG_TYPE_LOGIN_RESPOND,  // 登录回复

    ENUM_MSG_TYPE_ALL_ONLINE_REQUEST, // 在线用户请求
    ENUM_MSG_TYPE_ALL_ONLINE_RESPOND, // 在线用户回复

    ENUM_MSG_TYPE_SEARCH_USER_REQUEST, // 搜索用户请求
    ENUM_MSG_TYPE_SEARCH_USER_RESPOND, // 搜索用户回复

    ENUM_MSG_TYPE_MAX = 0x00fffff, //uint最大值Oxffffffff
};

#endif // PROTOCOL_H