#include "friend.h"
#include "tcpclient.h"  // 如果需要的话，包含 TcpClient 类的头文件

Friend::Friend(QWidget *parent) : QWidget(parent)
{
    m_pFriendListwidget = new QListWidget;
    m_pInputMsgLE = new QLineEdit;
    m_pShowMsgTE = new QTextEdit;
    m_pDelFriendPB = new QPushButton("删除好友");
    m_pFlushFriendPB = new QPushButton("刷新好友");
    m_pShowOnlineUsrPB = new QPushButton("显示在线用户");
    m_pSearchUsrPB = new QPushButton("查找用户");
    m_pMsgSendPB = new QPushButton("发送");
    m_pPrivateChatPB = new QPushButton("私聊");
    m_pOnline = new Online;

    QVBoxLayout *pLeftRightVBL = new QVBoxLayout; // 左侧右部分好友操作按钮布局
    pLeftRightVBL -> addWidget(m_pPrivateChatPB);
    pLeftRightVBL -> addWidget(m_pDelFriendPB);
    pLeftRightVBL -> addWidget(m_pFlushFriendPB);
    pLeftRightVBL -> addWidget(m_pShowOnlineUsrPB);
    pLeftRightVBL -> addWidget(m_pSearchUsrPB);
    QHBoxLayout *pRightDownHBL = new QHBoxLayout; // 右侧下方发送消息布局
    pRightDownHBL -> addWidget(m_pInputMsgLE);
    pRightDownHBL -> addWidget(m_pMsgSendPB);
    QVBoxLayout *pRightVBL = new QVBoxLayout; // 右侧聊天布局
    pRightVBL -> addWidget(m_pShowMsgTE);
    pRightVBL -> addLayout(pRightDownHBL);
    QHBoxLayout *pAllHBL = new QHBoxLayout; // 整体水平布局
    pAllHBL -> addWidget(m_pFriendListwidget);      // 左侧左部分好友列表
    pAllHBL -> addLayout(pLeftRightVBL);    // 左侧右部分好友操作
    pAllHBL -> addLayout(pRightVBL);        // 右侧聊天布局

    setLayout(pAllHBL); // 将整体布局pAllHBL设置为页面布局

    // 构造函数中绑定刷新好友列表按钮与对应事件
    connect(m_pShowOnlineUsrPB, SIGNAL(clicked(bool)), this, SLOT(showOnline()));

    // 绑定查找用户按钮与对应事件
    connect(m_pSearchUsrPB, SIGNAL(clicked(bool)),this, SLOT(searchUser()));

    // 绑定刷新好友列表按钮与对应事件
    connect(m_pFlushFriendPB, SIGNAL(clicked(bool)), this, SLOT(flushFriendList()));

    // 绑定删除好友与对应事件
    connect(m_pDelFriendPB, SIGNAL(clicked(bool)), this,  SLOT(deleteFriend()));

    // 绑定私聊好友与对应事件
    connect(m_pPrivateChatPB, SIGNAL(clicked(bool)), this, SLOT(privateChat()));

}

void Friend::showOnlineUser(PDU *pdu)
{
    qDebug() << "Enter Friend::showOnlineUser(PDU *pdu)";
    if(pdu == nullptr){
        return;
    }
    m_pOnline->showUser(pdu);
}

void Friend::updateFriendList(PDU *pdu){

    qDebug() << "Enter void Friend::updateFriendList(PDU *pdu)";

    if(NULL == pdu)
    {
        return ;
    }

    uint uiSize = (pdu -> uiMsgLen) / 36; // 注意是36，32 name + 4 online
    char caName[32] = {'\0'};
    char caOnline[4] = {'\0'};

    m_pFriendListwidget -> clear(); // 清除好友列表原有数据

    for(uint i = 0; i < uiSize; ++ i)
    {
        memcpy(caName, (char*)(pdu -> caMsg) + i * 36, 32);
        memcpy(caOnline, (char*)(pdu -> caMsg) + 32 + i * 36, 4);
        //无法接收到更新状态
        qDebug() << "客户端好友" << caName << " 在线状态" << caOnline;
        m_pFriendListwidget -> addItem(QString("%1\t%2").arg(caName)
                               .arg(strcmp(caOnline, "1") == 0?"在线":"离线"));
    }
}

QString Friend::getStrSearchName() const{
    return m_strSearchName;
}

void Friend::showOnline()
{
    qDebug() << "Enter Friend::showOnline()";

    if(m_pOnline->isHidden())
    {
        m_pOnline->show();
        // 发送请求查询数据库获取在线用户
        PDU* pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_ALL_ONLINE_REQUEST;
        TcpClient::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }else{
        m_pOnline->hide();
    }
}

void Friend::searchUser(){
    QString name = QInputDialog::getText(this, "搜索", "用户名："); // 通过输入子页面来获取用户输入返回一个文本类型

    if(!name.isEmpty())
    {
        qDebug() << "查找：" << name;
        PDU *pdu = mkPDU(0);
        pdu -> uiMsgType = ENUM_MSG_TYPE_SEARCH_USER_REQUEST;
        memcpy((char*)pdu -> caData, name.toStdString().c_str(), 32);
        TcpClient::getInstance().getTcpSocket().write((char*)pdu, pdu -> uiPDULen);
        free(pdu);
        pdu = NULL;
    }
}

void Friend::flushFriendList()
{
    qDebug() << "Enter Friend::flushFriendList()";

    QString strName = TcpClient::getInstance().getStrName(); // 获取自己用户名
    qDebug() << "自己的用户名是" << strName;

    PDU* pdu = mkPDU(0);
    pdu -> uiMsgType = ENUM_MSG_TYPE_FLSUH_FRIEND_REQUEST;
    strncpy(pdu -> caData, strName.toStdString().c_str(), strName.size());
    TcpClient::getInstance().getTcpSocket().write((char*)pdu, pdu -> uiPDULen);
    free(pdu);
    pdu = NULL;
}

void Friend::deleteFriend()
{
    if(NULL == m_pFriendListwidget -> currentItem()) // 如果没有选中好友
    {
        return ;
    }
    QString friName = m_pFriendListwidget -> currentItem() -> text(); // 获得选中的好友用户名
    friName = friName.split("\t")[0];
    QString loginName = TcpClient::getInstance().getStrName(); // 登录用户用户名

    qDebug() << friName;

    PDU* pdu = mkPDU(0);
    pdu -> uiMsgType = ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST;
    strncpy(pdu -> caData, friName.toStdString().c_str(), 32);
    strncpy(pdu -> caData + 32, loginName.toStdString().c_str(), 32);
    TcpClient::getInstance().getTcpSocket().write((char*)pdu, pdu -> uiPDULen);
    free(pdu);
    pdu = NULL;
}

// 获得对应用户名的私聊窗口
privateChatWid* Friend::searchPriChatWid(const char* chatName){

    qDebug() << "privateChatWid* Friend::searchPriChatWid(const char* chatName)";

    for (privateChatWid *ptr:m_priChatWidList)
    {
        if(ptr->strChatName() == chatName)
        {
            return ptr;
        }
    }
    return NULL;
}

void Friend::insertPriChatWidList(privateChatWid *priChat)
{
    m_priChatWidList.append(priChat);
}

void Friend::privateChat()
{
    qDebug() << "Enter Friend::privateChat()";

    if(NULL == m_pFriendListwidget -> currentItem()) // 如果没有选中好友
    {
        return ;
    }
    QString friName = m_pFriendListwidget -> currentItem() -> text(); // 获得选中的好友用户名
    friName = friName.split("\t")[0];
    QString loginName = TcpClient::getInstance().getStrName(); // 登录用户用户名
    qDebug() << "选中的好友姓名是：" << friName;

    //这个私聊窗口是好友的私聊窗口吗还是
    privateChatWid *priChat = searchPriChatWid(friName.toStdString().c_str());

    if(priChat == NULL) // 没找到该窗口，说明之前没有创建私聊窗口
    {
        qDebug() << "未找到窗口";
        priChat = new privateChatWid;
        priChat -> setStrChatName(friName);
        priChat -> setStrLoginName(loginName);
        priChat -> setPriChatTitle(friName.toStdString().c_str());
        m_priChatWidList.append(priChat); // 添加入该客户端私聊List
    }
    if(priChat->isHidden()) // 如果窗口被隐藏，则让其显示
    {
        qDebug() << "窗口被隐藏了";
        priChat->show();
    }
    if(priChat -> isMinimized()) // 如果窗口被最小化了
    {
        qDebug() << "窗口被最小化了";
        priChat->showNormal();
    }
}

