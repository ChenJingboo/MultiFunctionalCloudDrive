#include "openwidget.h"

OpenWidget::OpenWidget(QWidget *parent) : QWidget(parent)
{
    m_pListW = new QListWidget(this);

    //新添加的
    m_pUserLabel = new QLabel;

    m_pListW->addItem("好友");
    m_pListW->addItem("图书");

    m_pFriend = new Friend;
    m_pBook = new Book;

    m_pSw = new QStackedWidget;
    m_pSw->addWidget(m_pFriend);
    m_pSw->addWidget(m_pBook);

    //新添加的
    QHBoxLayout* pBodyHBL = new QHBoxLayout;
    pBodyHBL->addWidget(m_pListW);
    pBodyHBL->addWidget(m_pSw);

    QVBoxLayout *pMainVBL = new QVBoxLayout;
    pMainVBL -> addWidget(m_pUserLabel);
    pMainVBL -> addLayout(pBodyHBL);
    setLayout(pMainVBL);

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

void OpenWidget::setUserLabel(const char *name)
{
    m_pUserLabel->setText(name);
}

Book *OpenWidget::getPFileSystem() const
{
    return m_pFileSystem;
}
