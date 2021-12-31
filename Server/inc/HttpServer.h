#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include<netinet/in.h>

#include"HttpProcessRead.h"

class HttpServer: HttpProcessRead
{
public:
    //状态
    enum METHOD {  GET,POST };
    enum CHECKSTATE { CHECK_REQUESTLINE,CHECK_HEAD, CHECK_CONNECT };    
    enum LINESTAUS { OK, WRONG, HALFBAKED };

    HttpServer();
    ~HttpServer();

    void init(int sockfd, sockaddr_in address);
    bool read();
    bool wirte();
    void close();
    void process();
private:
    int sockfd_;
    sockaddr_in sockaddr_;
    static int epollfd_;
};



#endif