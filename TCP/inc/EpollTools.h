#ifndef EPOLLTOOLS_H
#define EPOLLTOOLS_H

#include"sys/epoll.h"

//将文件描述符fd上的EPOLLIN注册到epollfd指示的epoll内核事件表中
void addfd(int epollfd, int fd, bool ifet);

//在epoll内核事件中删除文件描述符fd
void removefd(int epollfd, int fd);

//在epoll内核事件中修改文件描述符fd
void modfd(int epollfd, int fd, int op);

//将文件描述符设置为非阻塞的
int setNonblocking(int fd);

//添加信号
void addsig(int sig, void(handler)(int), bool restart = true);

void showError(int connfd, const char* info);


#endif