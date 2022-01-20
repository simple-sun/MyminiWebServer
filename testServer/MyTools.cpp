#include"MyTools.h"
#include"fcntl.h"
#include"unistd.h"
#include<signal.h>
#include<string.h>
#include<assert.h>
#include<stdio.h>
#include<netinet/in.h>

#include<chrono>
#include<ctime>
#include<iostream>
#include<time.h>

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

bool writeBytes(int sockfd, const char* buffer, int len)
{
    int bytesWrt = 0;
    printf("write out %d bytes to sockfd %d\n",len,sockfd);
    while (1)
    {
        bytesWrt = send(sockfd,buffer,len,0);
        if(bytesWrt == -1)
        {
            return false;
        }
        else if(bytesWrt == 0)
        {
            return false;
        }
        len -= bytesWrt;
        buffer = buffer + bytesWrt;
        if(len <= 0)
        {
            return true;
        }
    }  
}


bool readOnce(int sockfd, char* buffer, int len)
{
    int byteRd = 0;
    memset(buffer,'\0',len);
    byteRd = recv(sockfd,buffer,len,0);
    if(byteRd == -1)
    {
        return false;
    }
    else if(byteRd == 0)
    {
        return false;
    }
    printf("got %d bytes from socket %d with content: %s\n",byteRd,sockfd,buffer);
    return true;
}

void startConn( int epollfd, int num ,const char* ip, int port)
{
    clock_t start,finish;
    double totaltime;

    start = clock();
    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
    for(int i = 0; i < num; i++)
    {
        
        int ret = 0;
    
        //sleep(1);
        int sockfd = socket(PF_INET,SOCK_STREAM,0);
        
        printf("creat 1 sock\n");
        if( sockfd < 0)
        {
            continue;
        }
        
        if( connect(sockfd,(struct sockaddr*)&address,sizeof(address)) == 0)
        {
            finish = clock();
            totaltime=(double)(finish-start)/1000000;
            std::cout << "/n connect的运行时间为"<<totaltime<<"秒！" << std::endl;
            printf("build connection %d\n",i);
            addfd(epollfd,sockfd,false);
        }
    }
}

void closeConn(int epollfd, int sockfd)
{
    epoll_ctl(epollfd,EPOLL_CTL_DEL,sockfd,0);
    close(sockfd);
}