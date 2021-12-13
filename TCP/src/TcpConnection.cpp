#include"TcpConnection.h"
#include"LogThread.h"
#include<boost/bind.hpp>
#include<sys/socket.h>

using namespace SUNSQ;

TcpConnection::TcpConnection(EventLoop* loop,int sockfd,
                            std::string name)
                :loop_(loop),
                name_(name),
                state_(kConnecting),
                channel_(new Channel(loop_,sockfd))
                {}
TcpConnection::~TcpConnection(){}

void TcpConnection::handleRead(system_clock::time_point recvTime)
{
    TcpBuffer* bufferData_;
    ssize_t len = ::recv(channel_->epollFd(), bufferData_,sizeof(bufferData_),0);
    //这里暂时代替一下，后面改      
    //system_clock::time_point recvTime = system_clock::now();
    if(len > 0)
    {
        messageCallback_( shared_from_this(),bufferData_,recvTime);
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
        //errno = savedErrno;
        handleError();
    }    
}

void TcpConnection::handleWrite()
{
    loop_->assertInLoopThread();
    if(channel_->isWriting())
    {
        size_t len = ::send(channel_->epollFd(),outputBuffer_.itAtRdIndex() ,
                                        outputBuffer_.readableBytes(),0);
        if(len > 0) 
        {   
            outputBuffer_.readOut(len);
            if(outputBuffer_.readableBytes() == 0)
            {
                channel_->disableWriting();
                if(state_ == kDisConnecting)    shutdownInLoop();
            }
            else
            {
                LOG_INFO << " More data need to be send ..." << log::end;
            }
        }
        else
        {
            LOG_FATAL << "TcpConnection::handleWrite():write... " << log::end;
        }
    }
    else
    {
        LOG_INFO << "TcpConnection::handleWrite is done. " << log::end;
    }    
}

void TcpConnection::handleClose()
{
    loop_->assertInLoopThread();
    LOG_INFO << "TcpConnection::handleClose state = "<< state_ << log::end;
    assert(state_ == kConnected);
    channel_->disableALl();    
    closeCallback_(shared_from_this());
}

void TcpConnection::handleError()
{
    LOG_INFO << "TcpConnection::handleError [" << name_ <<
                "]-SO_ERROR = "<<log::end;
}

void TcpConnection::connectionEstablished()
{
    loop_->assertInLoopThread();
    assert(state_ == kConnecting);
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

    loop_ ->removeChannel(boost::get_pointer(channel_));
}

void TcpConnection::send(const std::string& message)
{
    if(state_ == kConnected)
    {
        if(loop_->isInLoopThread())
        {
            sendInLoop(message);
        }
        else{
            loop_->runLoop(boost::bind(&TcpConnection::sendInLoop,this,message));
        }        
    }    
}

void TcpConnection::sendInLoop(const std::string& message)
{
    loop_->assertInLoopThread();
    ssize_t nwrite = 0;
    //如果outputBUffer_是空的，那么就直接往sockfd中写入
    if( (!channel_->isWriting()) && (outputBuffer_.readableBytes() == 0))
    {
        nwrite = ::send(channel_->epollFd(),
                message.data(),message.size(),0);
        if(nwrite >= 0)
        {
            if(nwrite < message.size())
            {
                LOG_INFO << " More data will be writing..."<< log::end;
            }
        }else
        {
            nwrite = 0;
            if(errno != EWOULDBLOCK)
            {
                LOG_FATAL << "TcpConnection::sendInLoop:nwrite < 0" << log::end;
            }
        }
    }   
    assert(nwrite >= 0);
    if( nwrite < message.size())
    {
        outputBuffer_.writeIn(*message.data());   
        if(!channel_->isWriting())
        {
            channel_->enableWriting();
        }
    }
}

void TcpConnection::shutdown()
{
    if(state_ == kConnected)
    {
        setStateE(kDisConnecting);
        loop_->runLoop(boost::bind(&TcpConnection::shutdownInLoop,this));
    }
}

void TcpConnection::shutdownInLoop()
{
    loop_->assertInLoopThread();
    if(!channel_->isWriting())
    {
        if( ::shutdown(*sockfd_,SHUT_WR) < 0)
        {
            LOG_FATAL << " TcpConnection::shutdownInLoop error : " << errno <<log::end;
        }
    }
}