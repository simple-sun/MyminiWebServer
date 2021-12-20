#ifndef TCPCONNECT_H
#define TCPCONNECT_H

#include"HttpServer.h"

const int MAXFD = 65536;
const int MAXEVENTNUM = 10000;

typedef HttpServer HS;

class TcpConnect
{
public:
    TcpConnect(int port, const char* ip);
    ~TcpConnect();

    int epollfd()   { return epollfd_;}
    int listenfd()  { return listenfd_;}

    bool connect(HS* users);
    
    bool handleEvent();

private:

    void setListen();
    int creatEpoll();

    int listenfd_;
    int epollfd_;
    epoll_event events_[10000];

    int port_;
    const char* ip_;

    bool isListen;
    bool isCreatE;

    

};



#endif 