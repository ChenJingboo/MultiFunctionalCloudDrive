#ifndef DBOPERATE_H
#define DBOPERATE_H

#include <QObject>
#include <QSqlDatabase> // 连接数据库
#include <QSqlQuery> // 数据库操作
#include <QDebug>
#include <QStringList>

class DBOperate : public QObject
{
    Q_OBJECT
public:
    explicit DBOperate(QObject *parent = nullptr);
    static DBOperate& getInstance(); // 公用获取引用，实现单例模式
    void init(); // 初始化函数，数据库连接
    bool handleRegist(const char* name, const char* password);
    bool handleLogin(const char* name, const char* password);
    bool handleOffline(const char* name);
    QStringList handleAllOnline();
    int handleSearchUser(const char *name); // 处理查找用户，0存在不在线，1存在并在线，2不存在
    ~DBOperate(); // 析构函数，关闭数据库连接

signals:

public slots:
private:
    QSqlDatabase m_db; // 连接数据库

};

#endif // DBOPERATE_H
