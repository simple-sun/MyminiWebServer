#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include"EventLoop.h"
#include"Channel.h"
//#include"bits/socket.h"
#include"sys/un.h"
//#include"bits/in.h"
#include"netinet/in.h"  

namespace SUNSQ
{
    class Acceptor
    {
    public:
        typedef boost::function<void(int socktfd, 
                            const sockaddr_in &)> NewConnectionCallback;

        Acceptor(EventLoop* loop, const sockaddr_in& listenAddr);
        ~Acceptor(){};

        void handleRead();
        bool listening()    {return listening_;};
        void listenAcceptor();
        void setNewConnectionCallback(const NewConnectionCallback cb_)
        { NewConnectionCallback_ = cb_;};
    private:
        Channel acceptChannel_;
        bool listening_;
        NewConnectionCallback NewConnectionCallback_;
        EventLoop* loop_;
        int acceptSocket_;
    };
}
#endif