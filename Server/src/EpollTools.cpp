#include"EpollTools.h"
#include"fcntl.h"
#include"unistd.h"
#include<signal.h>
#include<string.h>
#include<assert.h>
#include<stdio.h>
#include<netinet/in.h>
#include<errno.h>

void addfd(int epollfd, int fd, bool one_shot)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLET | EPOLLIN | EPOLLRDHUP;
    if(one_shot)
    {
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    setNonblocking(fd);
}


void removefd(int epollfd, int fd)
{
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
    close(fd);
}


void modfd(int epollfd, int fd, int op)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = op | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event);
}

int setNonblocking(int fd)
{
    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return old_option;
}

void addsig(int sig, void(handler)(int), bool restart)
{
    struct sigaction sg;
    memset(&sg,'\0',sizeof(sg));
    sg.sa_handler = handler;
    if(restart)
    {
        sg.sa_flags |= SA_RESTART;
    }
    sigfillset(&sg.sa_mask);
    assert(sigaction(sig,&sg,NULL) != -1);      
}  


void showError(int connfd, const char* info)
{
    printf("%s", info);
    send(connfd,info,sizeof(info),0);
    close(connfd);
}