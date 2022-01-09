#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include<netinet/in.h>
#include<memory>

#include"HttpProcessRead.h"
#include"HttpProcessWrite.h"




class HttpServer
{
public:       

    HttpServer();
    ~HttpServer();

    void init(int sockfd, sockaddr_in& address);
    bool read();
    bool write();
    void close();
    void process();
public:
    int sockfd_;
    sockaddr_in sockaddr_;
    static int epollfd_;
private:
    std::shared_ptr<HttpProcessRead> processRead;
    std::shared_ptr<HttpProcessWrite> processWrite;
public:
    //连接用户数量
    static int userConn_cnt;

private:
    char readBuffer[READBUFFERSIZE];
    int readIndex_;
};



#endif