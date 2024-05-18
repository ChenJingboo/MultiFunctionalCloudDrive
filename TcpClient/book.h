#ifndef BOOK_H
#define BOOK_H

#include <QWidget>
#include <QListWidget> // 显示文件的列表
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

class Book : public QWidget
{
    Q_OBJECT
public:
    explicit Book(QWidget *parent = nullptr);

signals:

private:
    QListWidget *m_pFileListW;      // 文件列表，显示所有文件

    QPushButton *m_pReturnPrePB;    // 返回上一路径
    QPushButton *m_pCreateDirPB;    // 创建文件夹
    QPushButton *m_pFlushDirPB;     // 刷新文件夹
    QPushButton *m_pDelFileOrDirPB; // 删除文件或文件夹

    QPushButton *m_pRenameFilePB;   // 重命名文件
    QPushButton *m_pUploadFilePB;   // 上传文件
    QPushButton *m_pDownloadFilePB; // 下载文件
    QPushButton *m_pShareFilePB;    // 分享文件

    QPushButton *m_pMoveFilePB;     // 移动文件
    QPushButton *m_pMoveDesDirDB;   // 移动目标目录
};

#endif // BOOK_H
