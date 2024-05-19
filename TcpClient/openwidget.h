/*
 *
 *
 *
*/

#ifndef OPENWIDGET_H
#define OPENWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QStackedWidget>

#include "friend.h"
#include "book.h"


class OpenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OpenWidget(QWidget *parent = nullptr);
    Friend* getFriend() const;
    static OpenWidget& getInstance();
    void setUserLabel(const char* name); // 设置登录用户信息的Label值
    Book *getPFileSystem() const;

signals:

public slots:

private:
    QLabel *m_pUserLabel;       // 标识客户端登录用户信息
    QListWidget* m_pListW;
    Friend *m_pFriend;
    Book* m_pBook;
    QStackedWidget* m_pSw;
    Book *m_pFileSystem;  // 文件页面

};

#endif // OPENWIDGET_H
