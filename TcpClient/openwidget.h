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

class Friend;
class Book;

#include <QStackedWidget>

class OpenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OpenWidget(QWidget *parent = nullptr);
    Friend* getFriend() const;
    static OpenWidget& getInstance();
    void setUserLabel(const char* name); // 设置登录用户信息的Label值

signals:

public slots:

private:
    QLabel *m_pUserLabel;       // 标识客户端登录用户信息
    QListWidget* m_pListW;
    Friend *m_pFriend;
    Book* m_pBook;
    QStackedWidget* m_pSw;

};

#endif // OPENWIDGET_H
