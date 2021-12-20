#include<sys/socket.h>
#include<netinet/in.h>
#include"arpa/inet.h"
#include"string.h"
#include<assert.h>
#include<sys/epoll.h>
#include<error.h>

#include"EpollTools.h"
#include"TcpConnect.h"
#include"LogThread.h"





TcpConnect::TcpConnect(int port, const char* ip)
            : port_(port),
            ip_(ip),
            
            isListen(false),
            isCreatE(false)
{
    struct linger tmp = {1,0};
    setsockopt(listenfd_,SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
    setListen();
    creatEpoll();
}

TcpConnect::~TcpConnect(){} 

void TcpConnect::setListen()
{
    listenfd_ = socket(PF_INET, SOCK_STREAM , 0);
    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET,ip_,&address);
    address.sin_port = port_;
    int ret = 0;
    ret = ::bind(listenfd_,(struct sockaddr*)&address,sizeof(address));
    assert(ret >= 0);
    ret = listen(listenfd_,5);
    assert(ret >= 0);
}

int TcpConnect::creatEpoll()
{   
    epollfd_ = epoll_create(5);
    assert(epollfd_ != -1);
    addfd(epollfd_,listenfd_,false);
    return epollfd_;
}

bool TcpConnect::handleEvent()
{
    int num = 6;
    while(1)
    {
        for(int i = 0; i < num; i++)
        {
            int sockfd = events_[i].data.fd;
        }
    }
}


bool TcpConnect::connect(HS* users)
{
    int num = epoll_wait(epollfd_,events_,MAXEVENTNUM,-1);
    LOG_INFO << num << " events has been got "<< log::end;
    if( (num < 0) && errno != EINTR)
    {
      LOG_FATAL << "epoll_wait failed.\n" << log::end;
      printf("epoll_wait failed.\n");
      return false;
    }

    for(int i = 0; i < num; i++)
    {
        int sockfd = events_[i].data.fd;
        if(sockfd == listenfd_)
        {
            struct sockaddr_in cAddress;
            socklen_t len = sizeof(cAddress);
            int connfd = accept(listenfd_,(struct sockaddr*)&cAddress,
                        &len);
            if(connfd < 0)
            {
                LOG_FATAL << "errno is " << errno << log::end;
                printf("errno is %d\n",errno);
                continue;
            }
            if(HS::userConn_cnt >= 65536)
            {
                showError(connfd,"INTERNAL SERVER BUSY");
                continue;
            }
            users[connfd].init(connfd,cAddress);
        }
        else if( events_[i].events & ( EPOLLRDHUP | EPOLLHUP | EPOLLERR))
        {
            //遇到异常，关闭连接
            users[sockfd].close();
            LOG_FATAL << "events[" << sockfd << "]." << events_[i].events << log::end;
        }

    }    
    return 1;
}