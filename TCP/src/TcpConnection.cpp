#include"TcpConnection.h"
#include<boost/enable_shared_from_this.hpp>
#include"LogThread.h"


using namespace SUNSQ;

TcpConnection::TcpConnection()
                :loop_(new EventLoop)
                {};
TcpConnection::~TcpConnection(){};


void TcpConnection::handleRead()
{
    char buf[65536];
    ssize_t len = recv(channel_->epollFd(), buf,sizeof(buf),0);

    if(len > 0)
    {
        messageCallback_( shared_from_this(),buf,len);
        //handleWrite();
    }
    if( len == 0)
    {
        handleClose();
        //不使用：：close（），因为这样更容易后来寻找错误
        //::close(channel_->epollFd());
    }
    if(len < 0)
    {
        handleError();
    }
    //被迫使用了shared_from_this()指针，找机会看看能不能干掉
}

void TcpConnection::handleWrite()
{
    char buf[65536];
    ssize_t len = send(channel_->epollFd(),buf,sizeof(buf),0);
}

void TcpConnection::handleClose()
{
    loop_->assertInLoopThread();
    LOG_INFO << "TcpConnection::handleClose state = "<< state_ << log::end;
    assert(state_ == kConnected);
    channel_->disableALl();
    connectionCallback_(shared_from_this());
}

void TcpConnection::handleError()
{
    LOG_INFO << "TcpConnection::handleError [" << name_ <<
                "]-SO_ERROR = "<<log::end;
}

void TcpConnection::connectionEstablished()
{
    loop_->assertInLoopThread();
    assert(state_ == kDisConnected);
    setStateE(kConnected);
    channel_->enableReading();
    connectionCallback_(shared_from_this());    
}
void TcpConnection::connectDestroyed()
{
    loop_->assertInLoopThread();
    assert(state_ == kConnected);
    setStateE(kDisConnected);
    channel_->disableALl();
    connectionCallback_(shared_from_this());

    loop_ ->removeChannel(get_pointer(channel_));
}

