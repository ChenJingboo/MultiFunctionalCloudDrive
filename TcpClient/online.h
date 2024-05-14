/*
 * 弹出框界面
*/

#ifndef ONLINE_H
#define ONLINE_H

#include <QWidget>
#include <QDebug>

#include "protocol.h"

namespace Ui {
class Online;
}

class Online : public QWidget
{
    Q_OBJECT

public:
    explicit Online(QWidget *parent = nullptr);
    void showUser(PDU* pdu);
    ~Online();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Online *ui;
};

#endif // ONLINE_H
