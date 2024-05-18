#include "book.h"

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
    m_pMoveDesDirDB->setEnabled(false); // 设置目标文件不可点击

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
}
