#include "online.h"
#include "ui_online.h"
#include "tcpclient.h"

Online::Online(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Online)
{
    ui->setupUi(this);
}

void Online::showUser(PDU *pdu)
{
    qDebug() << "Enter Online::showUser(PDU *pdu)";
    if(pdu == nullptr){
        return;
    }
//    uint uiSize = pdu->uiMsgLen/32;
//    char caTmp[32];
//    for(uint i = 0; i < uiSize; i++){
//        memcpy(caTmp, (char*)(pdu->caMsg) + i * 32, 3);
//        ui->listWidget->addItem(caTmp);
//    }
    // 处理pdu的Msg部分，将所有在线用户显示出来
    uint uiSize = pdu -> uiMsgLen / 32; // 消息Msg部分包含的用户数
    char caTmp[32];

    ui -> listWidget -> clear();// 清除之前在线用户列表的旧数据
    for(uint i = 0; i < uiSize; ++ i)
    {
        memcpy(caTmp, (char*)(pdu -> caMsg) + 32 * i, 32);
         qDebug() << "在线用户：" << caTmp;

        // 补充：不显示自己信息，防止之后添加自己为好友等操作错误
        if(strcmp(caTmp, TcpClient::getInstance().getStrName().toStdString().c_str()) == 0)
        {
            continue;
        }
        ui -> listWidget -> addItem(caTmp);
    }
}

Online::~Online()
{
    delete ui;
}
