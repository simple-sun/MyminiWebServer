#include"Connector.h"


using namespace SUNSQ;

Connector::Connector(EventLoop* loop, const sockaddr_in& serverAddr)
            :state_(false),
            loop_(loop)
            {}
Connector::~Connector(){}

void Connector::start()
{
    if(state_ == false) state_ = true;
    
}

void connect()
{
    
}