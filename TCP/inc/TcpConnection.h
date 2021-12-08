#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include"netinet/in.h"
#include"boost/function.hpp"
#include"Channel.h"
#include"EventLoop.h"
#include<boost/enable_shared_from_this.hpp>


namespace SUNSQ
{

class TcpConnection: public boost::enable_shared_from_this<TcpConnection>
{
public:
    typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;
    typedef boost::function<void(const TcpConnectionPtr& )> ConnectionCallback;
    typedef boost::function<void(const TcpConnectionPtr&,
                                const char* data,
                                ssize_t len)> MessageCallback;
    typedef boost::function<void(TcpConnectionPtr&)> CloseCallback;

public:
    TcpConnection();
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

private:
    enum StateE     { kConnecting,kConnected, kDisConnected };

    void setStateE(StateE s) { state_ = s;};
    void handleRead();
    void handleWrite();
    void handleClose();
    void handleError();

    StateE state_;
    std::string name_;

    EventLoop* loop_;
    boost::scoped_ptr<SUNSQ::Channel> channel_;
    boost::scoped_ptr<int> sockfd_;
    sockaddr_in localAddr_;
    sockaddr_in peerAddr_;

        
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    CloseCallback closeCallback_;
};
}

#endif
