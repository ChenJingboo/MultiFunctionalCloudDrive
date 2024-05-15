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
    int handleAddFriend(const char *addedName, const char *sourceName); // 处理添加用户，0对方存在不在线，1对方存在在线，2不存在，3已是好友，4请求错误
    bool handleAddFriendAgree(const char *addedName, const char *sourceName); // 处理同意好友申请
    int getIdByUserName(const char *name);                   // 根据用户名获取用户id
    QStringList handleFlushFriendRequest(const char *name);  // 刷新好友列表请求
    bool handleDeleteFriend(const char *deletedName, const char *sourceName); //删除好友
    ~DBOperate(); // 析构函数，关闭数据库连接

signals:

public slots:
private:
    QSqlDatabase m_db; // 连接数据库

};

#endif // DBOPERATE_H
