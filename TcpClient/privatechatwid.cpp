#include "privatechatwid.h"
#include "ui_privatechatwid.h"

#include "tcpclient.h"
#include "protocol.h"

privateChatWid::privateChatWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::privateChatWid)
{
    ui->setupUi(this);
}

privateChatWid::~privateChatWid()
{
    delete ui;
}

QString privateChatWid::strChatName() const
{
    return m_strChatName;
}

void privateChatWid::setStrChatName(const QString &strChatName)
{
    m_strChatName = strChatName;
}

void privateChatWid::setPriChatTitle(const char *caTitle)
{
    ui -> label -> setText(caTitle);
}

QString privateChatWid::strLoginName() const
{
    return m_strLoginName;
}

void privateChatWid::setStrLoginName(const QString &strLoginName)
{
    m_strLoginName = strLoginName;
}

void privateChatWid::on_pushButton_send_clicked()
{
    qDebug() << "privateChatWid::on_pushButton_send_clicked()";

    QString strSendMsg = ui -> lineEdit_input -> text();
    if(strSendMsg.isEmpty())
    {
        QMessageBox::warning(this, "私聊", "发送消息不能为空！");
        return ;
    }

    // 显示在自己showMsgTE窗口上
    ui -> lineEdit_input -> clear(); // 清空输入框内容
    ui -> textEdit_show -> append(QString("%1 : %2").arg(m_strLoginName).arg(strSendMsg));

    // 发送消息给服务器来转发给对方
    PDU *pdu = mkPDU(strSendMsg.size());
    pdu -> uiMsgType = ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST;
    strncpy(pdu -> caData, m_strChatName.toStdString().c_str(), 32); // 目标用户名
    strncpy(pdu -> caData + 32, m_strLoginName.toStdString().c_str(), 32); // 请求方用户名
    strncpy((char*)pdu -> caMsg, strSendMsg.toStdString().c_str(), strSendMsg.size()); // 发送内容

    TcpClient::getInstance().getTcpSocket().write((char*)pdu, pdu -> uiPDULen);
    free(pdu);
    pdu = NULL;
}

void privateChatWid::updateShowMsgTE(PDU *pdu)
{


    if(NULL == pdu)
    {
        return ;
    }
    char caSendName[32] = {'\0'};
    strncpy(caSendName, pdu -> caData + 32, 32);
    QString strMsg = QString("%1 : %2").arg(caSendName).arg((char*)pdu -> caMsg);
    ui -> textEdit_show -> append(strMsg);
}
