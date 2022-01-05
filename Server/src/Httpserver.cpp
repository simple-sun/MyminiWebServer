#include"HttpServer.h"
#include"HttpProcessRead.h"
#include"sys/socket.h"

HttpServer::HttpServer()    
{
    
}

HttpServer::~HttpServer(){}

int HttpServer::epollfd_ = -1;
int HttpServer::userConn_cnt = 0;

void HttpServer::init(int sockfd, sockaddr_in address)
{

}

void HttpServer::close()
{

}

bool HttpServer::read()
{

}
bool HttpServer::write()
{

}

void HttpServer::process()
{
    std::shared_ptr<HttpProcessRead> processRead;
    processRead->processRead();
    
}
