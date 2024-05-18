#include "book.h"
#include "tcpclient.h"
#include <QInputDialog>

Book::Book(QWidget *parent) : QWidget(parent)
{
    m_pFileListW = new QListWidget; // 文件列表，显示所有文件
    m_pReturnPrePB = new QPushButton("返回");
    m_pCreateDirPB = new QPushButton("创建文件夹");
    m_pDelFileOrDirPB = new QPushButton("删除");
    m_pFlushDirPB = new QPushButton("刷新文件夹");
    m_pRenameFilePB = new QPushButton("重命名");

    QHBoxLayout *pDirOpVBL = new QHBoxLayout;
    pDirOpVBL -> addWidget(m_pReturnPrePB);
    pDirOpVBL -> addWidget(m_pCreateDirPB);
    pDirOpVBL -> addWidget(m_pFlushDirPB);
    pDirOpVBL -> addWidget(m_pDelFileOrDirPB);
    pDirOpVBL -> addWidget(m_pRenameFilePB);

    m_pUploadFilePB = new QPushButton("上传文件");
    m_pDownloadFilePB = new QPushButton("下载文件");
    m_pShareFilePB = new QPushButton("分享文件");
    m_pMoveFilePB = new QPushButton("移动文件");
    m_pMoveDesDirDB = new QPushButton("目标目录");
    m_pMoveDesDirDB->setEnabled(false); // 设置目标文件不可点击1

    QHBoxLayout *pFileOpVBL = new QHBoxLayout;
    pFileOpVBL -> addWidget(m_pUploadFilePB);
    pFileOpVBL -> addWidget(m_pDownloadFilePB);
    pFileOpVBL -> addWidget(m_pShareFilePB);
    pFileOpVBL -> addWidget(m_pMoveFilePB);
    pFileOpVBL -> addWidget(m_pMoveDesDirDB);

    QVBoxLayout *pMainVBL = new QVBoxLayout;
    pMainVBL -> addLayout(pDirOpVBL);
    pMainVBL -> addLayout(pFileOpVBL);
    pMainVBL -> addWidget(m_pFileListW);

    setLayout(pMainVBL);

    //连接按钮及其创建文件夹函数
    connect(m_pCreateDirPB, SIGNAL(clicked(bool)), this, SLOT(createDir()));

}

void Book::createDir()
{
    qDebug() << "Enter void Book::createDir()";

    QString strDirName = QInputDialog::getText(this, "新建文件夹", "文件夹名："); // 获得文件夹名
    QString strCurPath = TcpClient::getInstance().getStrCurPath();

    if(strDirName.isEmpty())
    {
        QMessageBox::warning(this, "新建文件夹", "文件夹名字不能为空！");
        return ;
    }
    PDU *pdu = mkPDU(strCurPath.size() + 1);
    pdu -> uiMsgType = ENUM_MSG_TYPE_CREATE_DIR_REQUEST;
    strncpy(pdu -> caData, strDirName.toStdString().c_str(), strDirName.size());
    memcpy((char*)pdu ->caMsg, strCurPath.toStdString().c_str(), strCurPath.size());
    TcpClient::getInstance().getTcpSocket().write((char*)pdu, pdu -> uiPDULen);

    free(pdu);
    pdu = NULL;
}
