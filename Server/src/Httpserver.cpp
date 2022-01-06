#include"HttpServer.h"
#include"HttpProcessRead.h"
#include"EpollTools.h"
#include"LogThread.h"

#include"string.h"
#include"sys/socket.h"
#include"unistd.h"

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
        memset(readBuffer,'\0',2048);
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
    return 1;
}

void HttpServer::process()
{
    processRead = std::make_shared<HttpProcessRead>(readBuffer,readIndex_);
    if(( processRead->processRead()) == 0)
    {
        modfd(epollfd_,sockfd_,EPOLLIN);
        return;
    }           
}
