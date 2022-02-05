#include<assert.h>
#include<mysql/mysql.h>

#include"MysqlConn.h"
#include"LogThread.h"


SqlPool::SqlPool()
{
    maxConn = 10;
    currConn = 0;
    freeConn = maxConn - currConn;
}

SqlPool::~SqlPool(){
    destoryAll();
}

void SqlPool::init(std::string url, std::string user, std::string password, std::string databaseName,
                    int port)
{
    url_ = url;
    port_ = port;
    user_ = user;
    password_ = password;
    databaseName_ = databaseName;

    for(int i = 0; i < maxConn; i++)
    {
        MYSQL* conn = nullptr;
        conn = mysql_init(conn);
        if(conn == NULL)
        {
            LOG_FATAL << "Mysql Error" << log::end;
            exit(1);
        } 
        conn = mysql_real_connect(conn, url_.c_str(),user_.c_str(),password_.c_str(),
                    databaseName_.c_str(),port,NULL,0);
        // AfxMessageBox(mysql_error(conn));
        //assert(conn != NULL);
        connList.push(conn);
        freeConn++;        
    }

    sig = Sem();
    maxConn = freeConn;
}

//当有请求时，从数据库连接池中返回一个可用连接，更新使用和空闲连接数
MYSQL* SqlPool::getDatebaseConn()
{
    MYSQL* conn = nullptr;
    if(connList.size() == 0)
    {
        return NULL;
    }

    //sig.wait();

    lock.lock();

    conn = connList.front();
    connList.pop();
    freeConn--;
    currConn++;
    
    lock.unlock();
    return conn;
}

//释放传入的链接
bool SqlPool::releaseConn(MYSQL* conn)
{
    if(conn == NULL)
    {
        return false;
    }

    lock.lock();

    connList.push(conn);
    freeConn++;
    currConn--;

    lock.unlock();

    sig.post();
    return true;
}

//查询当前的空间链接
int SqlPool::freeConnection()
{
    return this->freeConn;
}

void SqlPool::destoryAll()
{
    lock.lock();

    while(!connList.empty())
    {
        MYSQL* conn = connList.front();
        mysql_close(conn);
        connList.pop();
    }

    currConn = 0;
    freeConn = 0;

    lock.unlock();    
}


ConnectMysql::ConnectMysql(MYSQL** conn, SqlPool* sqlPool)
            :sqlPool_(sqlPool)
{
    *conn = sqlPool->getDatebaseConn();
    conn_ = *conn;
}

ConnectMysql::~ConnectMysql()
{
    sqlPool_->releaseConn(conn_);
}