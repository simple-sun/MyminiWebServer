#include"HttpServer.h"
#include"HttpProcessRead.h"
#include"EpollTools.h"
#include"LogThread.h"
#include"MysqlConn.h"


#include"string.h"
#include"sys/socket.h"
#include"unistd.h"
#include"sys/uio.h"

HttpServer::HttpServer()    
{
    
}

HttpServer::~HttpServer(){}

int HttpServer::epollfd_ = -1;
int HttpServer::userConn_cnt = 0;

void HttpServer::init(int sockfd, sockaddr_in& address)
{
    sockfd_ = sockfd;
    sockaddr_ = address;
    addfd(epollfd_,sockfd_,false);
    userConn_cnt++;
    {
        readIndex_ = 0;
        memset(readBuffer,'\0',READBUFFERSIZE);
    }
}

void HttpServer::close()
{
    if(sockfd_ != -1)
    {
        removefd(epollfd_,sockfd_);
        sockfd_ = -1;
        userConn_cnt--;
    }
}

bool HttpServer::read()
{
    if(readIndex_ > READBUFFERSIZE)
    {
        LOG_FATAL << " readIndex_ > READBUFFERSIZE" << log::end;
        return false;
    }
    int readByte = 0;

    //ET模式读取数据
    while(1)
    {
        readByte = recv(sockfd_,readBuffer+readIndex_,READBUFFERSIZE-readIndex_,0);
        if(readByte == -1)
        {
            if(errno == EAGAIN )
            {
                //sleep(10);
                break;
            }
            LOG_FATAL << "readByte = -1" << log::end;
            return false;
        }
        else if(readByte == 0)
        {
            LOG_FATAL << "readByte = 0" << log::end;
            return false;
        }
        readIndex_ += readByte;
    }
    return true;
}

bool HttpServer::write()
{
    int tmp = 0;
    int byteSend = 0;
    int byteToSend = processWrite->writeIndex_ + processRead->filestat_.st_size;
    if(byteToSend == 0)
    {
        modfd(epollfd_,sockfd_,EPOLLIN);
        reset();
        return true;
    }
    while (1)
    {
        //tmp = ::writev(sockfd_,processWrite->iv_,processWrite->ivCnt_);
        tmp = ::write(sockfd_,processWrite->writeBuffer,strlen(processWrite->writeBuffer));
        char buf[2048];
        int len = snprintf(buf,2048,processRead->fileAddr);
        tmp += ::write(sockfd_,buf,len);
        // printf("buf = %s\n", buf);
        // printf("HttpServer::write()::tmp = %d.\n",tmp);
        if( tmp <0)
        {
            if(errno == EAGAIN)
            {
                modfd(epollfd_,sockfd_,EPOLLOUT);
            }
            munmap(processRead->fileAddr,processRead->filestat_.st_size);
            processRead->fileAddr = 0;
            return false;
        }
        byteToSend -= tmp;
        byteSend += tmp;

        if(byteToSend <= 0)
        {
            //发送HTTP响应成功，判断是否关闭连接
            munmap(processRead->fileAddr,processRead->filestat_.st_size);
            processRead->fileAddr = 0;
            modfd(epollfd_,sockfd_,EPOLLIN);
            if(processRead->linger_)
            {
                reset();               
                return true;
            }
            else
            {            
                reset();   
                return false;
            }
        }
    }  
    return true;
}

void HttpServer::process()
{
    processRead = std::make_shared<HttpProcessRead>(readBuffer,readIndex_);
    
    //将数据从数据库转存到本地
    auto conn = SqlPool::getInstance();
    conn->init("127.0.0.1","root","Sun15045344359","userdata",3306);    
    processRead->initMysqlData(conn);

    auto ret = processRead->processRead();

    if(ret == HttpProcessRead::HTTPCODE::NO_REQUEST)
    {        
        modfd(epollfd_,sockfd_,EPOLLIN);
        return;
    }
    processWrite = std::make_shared<HttpProcessWrite>(processRead);
    if( ! processWrite->processWrite(ret))
    {
        close();
    }           
    modfd(epollfd_,sockfd_,EPOLLOUT);
}


void HttpServer::reset()
{
    readIndex_ = 0;
    processRead->reset();        
    processWrite->pRead = processRead;
    processWrite->reset();

    memset(readBuffer,'\0',READBUFFERSIZE);
}