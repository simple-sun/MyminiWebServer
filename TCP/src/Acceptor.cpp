#include"Acceptor.h"
#include"boost/bind.hpp"
#include<sys/socket.h>
#include"LogThread.h"


using namespace SUNSQ;


Acceptor::Acceptor(EventLoop* loop, const sockaddr_in& listenAddr)
            :listening_(false),
            //使用socket（2,6版本以后），直接获取非阻塞socket的fd）
            //acceptSocket_( loop->epollfd()),
            acceptSocket_(socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)),
            acceptChannel_(loop, acceptSocket_),
            loop_(loop)
            {
                int ret = bind(acceptSocket_, 
                        (struct sockaddr*)&listenAddr,sizeof(listenAddr));
                acceptChannel_.setReadCallback(
                    boost::bind(&Acceptor::handleRead,this)
                );
            }
void Acceptor::listenAcceptor()
{
    loop_->assertInLoopThread();
    //将listening_设置为true，表示正在监听
    listening_ = true;
    ::listen(acceptSocket_,5);
    acceptChannel_.enableReading();
    //handleRead();
}

void Acceptor::handleRead()
{
    struct sockaddr_in peerAddr;
    bzero(&peerAddr,sizeof(peerAddr));
    int confd = ::accept4(acceptSocket_,(struct sockaddr*)(&peerAddr),
                (socklen_t*)sizeof( peerAddr), SOCK_NONBLOCK | SOCK_CLOEXEC);
    if(confd >= 0)
    {
        if(NewConnectionCallback_)
        {
            NewConnectionCallback_(confd,peerAddr);
        }
        else
        {
            //没有考虑文件描述符耗尽的情况，可以通过使用epoll
            //来观察，fd是否可写
            if(::close(confd)<0)
            {
                LOG_DEBUG<< " Acceptor::handleRead.close(fd)";
            }
        }
    }
}