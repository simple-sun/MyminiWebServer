#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include"netinet/in.h"
#include"errno.h"
#include"Channel.h"
#include"EventLoop.h"
#include"TcpBuffer.h"
#include<boost/enable_shared_from_this.hpp>
#include"boost/function.hpp"


namespace SUNSQ
{

class TcpConnection: public boost::enable_shared_from_this<TcpConnection>
{
public:
    typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;
    typedef boost::function<void(const TcpConnectionPtr& )> ConnectionCallback;
    typedef boost::function<void(const TcpConnectionPtr&,
                                TcpBuffer* bufferData_,
                                system_clock::time_point recvTime)> MessageCallback;
    typedef boost::function<void(const TcpConnectionPtr&)> CloseCallback;

public:
    TcpConnection(EventLoop* loop,int sockfd,std::string name);
    TcpConnection(const TcpConnection&) = delete;
    ~TcpConnection();

    bool connected() { return state_ == kConnected; };
    std::string name() {return name_;};
    sockaddr_in peerAddress() {return peerAddr_;};
    
    void setConnectionCallback(const ConnectionCallback& cb)
    { connectionCallback_ = cb;}
    void setMessageCallback(const TcpConnection::MessageCallback& cb)
    { messageCallback_ = cb; }

    //TcpServer and TcpClient use only
    void setCloseCallback(const CloseCallback& cb) { closeCallback_ = cb;};

    //收到一个新的连接请求时，调用
    void connectionEstablished();
    //当从TcpServer中rm当前连接时调用
    void connectDestroyed();

    void send(const std::string& message);
    void shutdown();

private:
    enum StateE{ kConnecting,kConnected,kDisConnecting, kDisConnected };

    void setStateE(StateE s) { state_ = s;};
    void handleRead(system_clock::time_point recvTime);
    void handleWrite();
    void handleClose();
    void handleError();
    void sendInLoop(const std::string& message);
    void shutdownInLoop();

    TcpBuffer inputBuffer_;
    TcpBuffer outputBuffer_;

    StateE state_;
    std::string name_;

    EventLoop* loop_;
    std::unique_ptr<SUNSQ::Channel> channel_;
    boost::scoped_ptr<int> sockfd_;
    sockaddr_in localAddr_;
    sockaddr_in peerAddr_;  
        
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    CloseCallback closeCallback_;    
};
}

#endif