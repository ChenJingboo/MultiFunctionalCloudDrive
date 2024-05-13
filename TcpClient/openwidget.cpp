#include "openwidget.h"
#include "friend.h"   // 包含 Friend 类的头文件
#include "book.h"     // 包含 Book 类的头文件

OpenWidget::OpenWidget(QWidget *parent) : QWidget(parent)
{
    m_pListW = new QListWidget(this);
    m_pListW->addItem("好友");
    m_pListW->addItem("图书");

    m_pFriend = new Friend;
    m_pBook = new Book;

    m_pSw = new QStackedWidget;
    m_pSw->addWidget(m_pFriend);
    m_pSw->addWidget(m_pBook);

    QHBoxLayout* pMain = new QHBoxLayout;
    pMain->addWidget(m_pListW);
    pMain->addWidget(m_pSw);

    setLayout(pMain);

    connect(m_pListW, SIGNAL(currentRowChanged(int))
            , m_pSw, SLOT(setCurrentIndex(int)));

}

Friend *OpenWidget::getFriend() const
{
    return m_pFriend;
}

OpenWidget &OpenWidget::getInstance()
{
    static OpenWidget instance;
    return instance;
}
