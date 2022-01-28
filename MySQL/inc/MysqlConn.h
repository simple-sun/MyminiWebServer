#ifndef MYSQLCONN_H
#define MYSQLCONN_H


#include<mysql/mysql.h>
#include<queue>
#include<string>

#include"locker.h"

class SqlPool
{
public:
    SqlPool();
    ~SqlPool();

    MYSQL* getDatebaseConn();   //获取数据库链接
    bool releaseConn(MYSQL* conn);  //释放链接
    int freeConnection();              //查询当前的空间链接
    void destoryAll();          //销毁所有链接

    void init(std::string url, std::string user, std::string password, std::string databaseName,
                    int port);


private:
    Locker lock;
    std::queue<MYSQL*> connList;   //连接池
    Sem sig;

    int maxConn;
    int freeConn;
    int currConn;

public:
    std::string url_;   //主机地址
    std::string port_;  //数据库端口号
    std::string user_;  //登录数据库的用户名
    std::string password_;   //登录数据库密码
    std::string databaseName_;   //使用数据库名
};


class ConnectMysql
{
public:
    ConnectMysql(MYSQL** conn, SqlPool* sqlPool);
    ~ConnectMysql();
private:
    MYSQL* conn_;
    SqlPool* sqlPool_;
};



#endif