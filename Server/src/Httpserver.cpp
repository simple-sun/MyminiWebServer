#include"HttpServer.h"
#include"HttpProcessRead.h"
#include"sys/socket.h"

HttpServer::HttpServer()
{
    
}


void HttpServer::process()
{
    
    HttpProcessRead::processRead();
}
