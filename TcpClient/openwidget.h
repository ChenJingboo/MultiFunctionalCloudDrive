/*
 *
 *
 *
*/

#ifndef OPENWIDGET_H
#define OPENWIDGET_H

#include <QWidget>
#include <QListWidget>

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

signals:

public slots:

private:
    QListWidget* m_pListW;
    Friend *m_pFriend;
    Book* m_pBook;
    QStackedWidget* m_pSw;

};

#endif // OPENWIDGET_H
