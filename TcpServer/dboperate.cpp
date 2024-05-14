#include "dboperate.h"
#include <QMessageBox>
#include <QDebug>

DBOperate::DBOperate(QObject *parent) : QObject(parent)
{
    // 连接数据库
    m_db = QSqlDatabase::addDatabase("QMYSQL"); // 连接的数据库类型

}

DBOperate &DBOperate::getInstance()
{
    static DBOperate instance;
    return instance;
}

// 数据库连接
void DBOperate::init()
{
    m_db.setHostName("localhost");         // 数据库服务器IP
    m_db.setUserName("root");              // 数据库用户名
    m_db.setPassword("1234");              // 数据库密码
    m_db.setDatabaseName("networkdiskdb"); // 数据库名
    if(m_db.open()) // 数据库是否打开成功
    {
        qDebug() << "数据库打开了";
        QSqlQuery query;
        query.exec("select * from userInfo");
        while(query.next())
        {
            QString data = QString("%1, %2, %3, %4").arg(query.value(0).toString()).arg(query.value(1).toString())
                    .arg(query.value(2).toString()).arg(query.value(3).toString());
            // 只格式化查询结果中存在的三个字段：id, name, pwd
//            QString data = QString("%1, %2, %3").arg(query.value(0).toString())
//                .arg(query.value(1).toString())
//                .arg(query.value(2).toString());
            qDebug() << data;
        }
    }
    else
    {
        QMessageBox::critical(NULL, "数据库打开", "数据库打开失败");
    }
}

bool DBOperate::handleRegist(const char* name, const char* password){
    // 考虑极端情况
    if(NULL == name || NULL == password)
    {
        return false;
    }
    // 数据插入数据库
    QString strQuery = QString("insert into userInfo(name, pwd) values('%1', '%2')").arg(name).arg(password);
    QSqlQuery query;

    // qDebug() << strQuery;
    return query.exec(strQuery); // 数据库中name索引是unique，所以如果name重复会返回false，插入成功返回true
}

bool DBOperate::handleLogin(const char* name, const char* password){

    qDebug() << "Enter the main function." << endl;

    // 考虑极端情况
    if(NULL == name || NULL == password)
    {
        return false;
    }
    // 数据库查询
    QString strQuery = QString("select * from userInfo where name = \'%1\' and pwd = \'%2\' "
                               "and online = 0").arg(name).arg(password); // online = 0 可以判定用户是否未登录，不允许重复登陆
    QSqlQuery query;

    qDebug() << strQuery;
    query.exec(strQuery);

    if(query.next()) // 每次调用next都会读取一条数据，并将结果放入query中，返回值为true，无数据则返回false
    {
        qDebug() << "登录成功" << endl;
        // 如果登录成功，需要设置online = 1，并返回true
        strQuery = QString("update userInfo set online = 1 where name = \'%1\' and pwd = \'%2\' ").arg(name).arg(password);
        return query.exec(strQuery);
    }
    else
    {
        qDebug() << "登录失败" << endl;
        return false;
    }
}

bool DBOperate::handleOffline(const char *name)
{
    if(NULL == name)
    {
        qDebug() << "name is NULL";
        return false;
    }
    // 更新online状态为0
    QString strQuery = QString("update userInfo set online = 0 where name = \'%1\'").arg(name);
    QSqlQuery query;

    return query.exec(strQuery);
}

QStringList DBOperate::handleAllOnline()
{
    qDebug() << "Enter QStringList DBOperate::handleAllOnline()";

    QString data = QString("select name from userinfo where online = 1");

    QSqlQuery query;
    QStringList result;
    result.clear();

    query.exec(data); // 执行语句！  这里忘记一次导致未返回数据
    while(query.next()){
        result.append(query.value(0).toString());
        qDebug() << result;
    }

    return result;
}

int DBOperate::handleSearchUser(const char *name)
{
    qDebug() << "Enter int DBOperate::handleSearchUser(const char *name)";

    if(NULL == name)
    {
        return 2;
    }
    QString strQuery = QString("select online from userinfo where name = \'%1\' ").arg(name);
    QSqlQuery query;

    query.exec(strQuery);
    if(query.next())
    {
        return query.value(0).toInt(); // 存在并在线返回1，存在不在线返回0
    }
    else
    {
        return 2; // 不存在该用户
    }
}

int DBOperate::handleAddFriend(const char *addedName, const char *sourceName)
{
    qDebug() << "Enter DBOperate::handleAddFriend(const char *addedName, const char *sourceName)";

    if(NULL == addedName || NULL == sourceName)
    {
        return 4; // 请求错误
    }
    QString strQuery = QString("select * from friendinfo "
                               "where (id = (select id from userinfo where name = \'%1\') and "
                               "friendId = (select id from userinfo where name = \'%2\')) or "  // 好友是双向的，数据库只存了单向，注意是or关系
                               "(id = (select id from userinfo where name = \'%3\') and "
                               "friendId = (select id from userinfo where name = \'%4\'))")
            .arg(sourceName).arg(addedName).arg(addedName).arg(sourceName);
    qDebug() << strQuery;
    QSqlQuery query;
    query.exec(strQuery);
    if(query.next())
    {
        return 3; // 双方已经是好友
    }
    else // 不是好友
    {
        return handleSearchUser(addedName); // 查询对方，存在并在线返回1，存在不在线返回0，不存在该用户返回2
    }
}

bool DBOperate::handleAddFriendAgree(const char *addedName, const char *sourceName){
    if(NULL == addedName || NULL == sourceName)
    {
        qDebug() << "handleAddFriendAgree: name is NULL";
        return false;
    }

    int sourceUserId = -1, addedUserId = -1;
    // 查找用户对应id
    addedUserId = getIdByUserName(addedName);
    sourceUserId = getIdByUserName(sourceName);

    QString strQuery = QString("insert into friendinfo values(%1, %2) ").arg(sourceUserId).arg(addedUserId);
    QSqlQuery query;

    qDebug() << "handleAddFriendAgree " << strQuery;

    return query.exec(strQuery);
}

int DBOperate::getIdByUserName(const char *name)
{
    if(NULL == name)
    {
        return -1;
    }
    QString strQuery = QString("select id from userinfo where name = \'%1\' ").arg(name);
    QSqlQuery query;

    query.exec(strQuery);
    if(query.next())
    {
        return query.value(0).toInt();
    }
    else
    {
        return -1; // 不存在该用户
    }
}

QStringList DBOperate::handleFlushFriendRequest(const char *name)
{

    qDebug() << "Enter QStringList DBOperate::handleFlushFriendRequest(const char*)";

    QStringList strFriendList;
    strFriendList.clear(); // 清除内容

    if (NULL == name)
    {
        return strFriendList;
    }

    // 获取请求方name对应的id
    QString strQuery = QString("select id from userinfo where name = \'%1\' and online = 1 ").arg(name);
    QSqlQuery query;
    int iId = -1; // 请求方name对应的id
    query.exec(strQuery);
    if (query.next())
    {
        iId = query.value(0).toInt();
    }

    // 查询好友信息表与用户信息表获取好友列表
    strQuery = QString("select name, online from userinfo "
                       "where id in "
                       "((select friendId from friendinfo "
                       "where id = %1) "
                       "union "
                       "(select id from friendinfo "
                       "where friendId = %2))").arg(iId).arg(iId);
    query.exec(strQuery);
    while(query.next())
    {
        char friName[32];
        char friOnline[4];
        strncpy(friName, query.value(0).toString().toStdString().c_str(), 32);
        strncpy(friOnline, query.value(1).toString().toStdString().c_str(), 4);
        strFriendList.append(friName);
        strFriendList.append(friOnline);
         qDebug() << "好友信息 " << friName << " " << friOnline;
         qDebug() << strFriendList;
    }

    return strFriendList; // 返回查询到所有在线用户的姓名
}

DBOperate::~DBOperate()
{
    m_db.close(); // 关闭数据库连接
}
