#ifndef CONNECTOR
#define CONNECTOR

#include"EventLoop.h"
#include"netinet/in.h"

namespace SUNSQ
{
    class Connector
    {
    public:
    typedef boost::function<void (int sockfd)> NewConnectionCallback;

    Connector(EventLoop* loop, const sockaddr_in& serverAddr );
    ~Connector();

    void setConnectionCallback(const NewConnectionCallback& cb)
    { newConnectionCallback_ = cb; }

    void start();
    void restart(); //只能在loop线程中调用
    void stop();

    private:

    void connect();

    NewConnectionCallback newConnectionCallback_;
    bool state_;
    EventLoop* loop_;

    };
}

#endif