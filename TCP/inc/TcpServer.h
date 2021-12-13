#ifndef TCPSERVER_H
#define TCPSERVER_H

#include"EventLoop.h"
#include"Channel.h"
#include"Acceptor.h"
#include<algorithm>
#include<map>
#include"TcpConnection.h"

namespace SUNSQ
{
class TcpServer
{
    public:
        TcpServer(EventLoop* loop, const sockaddr_in &listenAddr);
        ~TcpServer();

        //如果没有listening，start（）
        void start();
        

        //设置连接回调callback
        void setConnectionCallback(const TcpConnection::ConnectionCallback& cb)
        {  connectionCallback_ = cb; }
        //设置消息回调callback
        void setMessageCallback(const TcpConnection::MessageCallback& cb)
        { messageCallback_ = cb; }

        void trySend();
    private:

        //不是线程安全的，但是in loop
        void newConnection(int sockfd, const sockaddr_in& peerAddr );
        void removeConnnection(const TcpConnection::TcpConnectionPtr& conn);

        typedef std::map<std::string, 
                    TcpConnection::TcpConnectionPtr> ConnnectionMap;
        //typedef boost::function<
        TcpConnection::ConnectionCallback connectionCallback_;
        TcpConnection::MessageCallback messageCallback_;
        TcpConnection::CloseCallback closeCallback_;

        EventLoop* loop_;
        const std::string name_;
        std::unique_ptr<Acceptor> acceptor_;

        bool started_;
        int nextConnId;
        ConnnectionMap connections_;         
};
}

#endif