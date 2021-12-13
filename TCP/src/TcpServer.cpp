#include"TcpServer.h"
#include"arpa/inet.h"
#include"boost/bind.hpp"
#include"LogThread.h"
using namespace SUNSQ;

TcpServer::TcpServer(EventLoop* loop, const sockaddr_in &listenAddr)
            :loop_(loop),
            acceptor_(new Acceptor(loop,listenAddr)),
            name_( inet_ntoa(listenAddr.sin_addr)),
            started_(false),
            nextConnId(1)
{
    acceptor_->setNewConnectionCallback(
        //还没看懂bind这里为啥有this
        boost::bind(&TcpServer::newConnection, this,_1, _2)
    );
}

TcpServer::~TcpServer()
{}

void TcpServer::start()
{
    if(!started_)
    {
        // int sockfd = socket(AF_INET,SOCK_STREAM | SOCK_NONBLOCK, 0);
        // struct sockaddr_in peerAddr;
        // bzero(&peerAddr,sizeof(peerAddr));
        // newConnection(sockfd, peerAddr);
        started_ = true; 
    }
    if(!acceptor_->listening())
    {
        //loop_->runLoop(boost::bind(&newConnection,_1,_2));
        loop_->runLoop(
            boost::bind(&Acceptor::listenAcceptor, boost::get_pointer( acceptor_))
                    );
    }       
}

void TcpServer::removeConnnection(const TcpConnection::TcpConnectionPtr& conn)
{
    LOG_INFO << " TcpServer::removeConnection [" << name_   
                << "]" << "] - connection " << conn->name() << log::end;
    connections_.erase(conn->name());

    //此处，书上说要用queueLoop，否则会出现生命期管理问题，
    //后期可以试一下，
    loop_->queueLoop(
        boost::bind(&TcpConnection::connectDestroyed,conn)
    );
}

void TcpServer::newConnection(int sockfd, const sockaddr_in& peerAddr )
{
    char buf[32];
    snprintf(buf,sizeof(buf),"%d",nextConnId);
    nextConnId++;
    std::string connName = name_ + buf;

    LOG_INFO <<  "TcpServer::newConnection [" << name_
           << "] - new connection [" << connName
           << "] from " << inet_ntoa(peerAddr.sin_addr) << log::end;
    //新建一个localAddr
    sockaddr_in localAddr_;
    bzero(&localAddr_,sizeof(localAddr_));
    socklen_t addrlen = sizeof(localAddr_);
    localAddr_.sin_family = AF_INET;
    
    //::bind(sockfd,(sockaddr*)localAddr_,addrlen);
    //::getsockname(sockfd,(sockaddr*)localAddr_,&addrlen);

    TcpConnection::TcpConnectionPtr conn(new TcpConnection(loop_,sockfd,connName));
    connections_[connName] = conn;
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->connectionEstablished();
    conn->setCloseCallback(  
        boost::bind(&TcpServer::removeConnnection,this,_1)
      );
}


void TcpServer::trySend()
{
    //随手瞎写的
    int sockfd = loop_->isInLoopThread()   ; 
    //TcpServer::newConnection(int sockfd, const sockaddr_in& peerAddr );
}