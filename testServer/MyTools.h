#ifndef EPOLLTOOLS_H
#define EPOLLTOOLS_H

#include"sys/epoll.h"
#include"sys/socket.h"
#include <netinet/in.h>
#include<arpa/inet.h>

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

//向服务器写入len字节数据
bool writeBytes(int sockfd, const char* buffer, int len);

//从服务器读取数据
bool readOnce(int sockfd, char* buffer, int len);

//向服务器发送tcp连接，
void startConn( int epollfd, int num ,const char* ip, int port);

void closeConn(int epollfd, int sockfd);

#endif