#include"MyTools.h"
#include"string.h"
#include"stdlib.h"
#include<iostream>

static const char* request = "GET http://localhost/index.html HTTP/1.1\r\nConnection: keep-alive\r\n\r\nxxxxxxxxxx";
//int argc, char* argv[]
int main()
{
    int epollfd = epoll_create(1000);
    //startConn(epollfd, atoi(argv[3]),argv[1],atoi(argv[2]));
    startConn(epollfd, 1000,"127.0.0.1",9527);
    epoll_event events[10000];
    char buffer[2048];
    std::cout << "connection finished" << std::endl;
    
    while(1)
    {
        int fds = epoll_wait(epollfd,events,10000,2000);
        for( int i = 0; i < fds; i++)
        {
            int sockfd = events[i].data.fd;
            if( events[i].events & EPOLLIN)
            {
                if(! readOnce(sockfd, buffer,sizeof(buffer) ))   
                {
                    closeConn(epollfd,sockfd);
                }
                struct epoll_event event;
                event.events = EPOLLET | EPOLLOUT | EPOLLRDHUP;
                event.data.fd = sockfd;
                epoll_ctl(epollfd,EPOLL_CTL_MOD,sockfd,&event);
            }
            else if( events[i].events & EPOLLOUT)
            {
                if( ! writeBytes(sockfd,request, strlen( request)))
                {
                    closeConn(epollfd, sockfd);
                }
                struct epoll_event event;
                event.events = EPOLLET | EPOLLIN | EPOLLRDHUP;
                event.data.fd = sockfd;
                epoll_ctl(epollfd,EPOLL_CTL_MOD,sockfd,&event);
            }
            else if( events[i].events & EPOLLERR)
            {
                closeConn(epollfd,sockfd);
            }
        }
    }

    return 0;
}