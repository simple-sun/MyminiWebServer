#include "Channel.h"
#include "EventLoop.h"
#include "sys/epoll.h"
#include"LogThread.h"


#define MAX_EVENT_NUM_VAL 1024;

using namespace SUNSQ;

 const int Channel::kReadEvents_ = 0;
 const int Channel::kWriteEvents_ = EPOLLIN | EPOLLPRI;
 const int Channel::kNoEvents_ = EPOLLOUT;



Channel::Channel( EventLoop* loop, int fdA):loop_(loop),
        fd_(fdA),events_(0),revents_(0),idx_(-1),
        MAX_EVENT_NUM(1024),
        eventHandling(true) {}
Channel::~Channel()
{
    assert(!eventHandling);
}

void Channel :: update()
{
    loop_->updateChannel(this);
}

void Channel::handleEvent(system_clock::time_point recvTime)
{
    //close事件回调
    if(!(revents_ & EPOLLIN) && (revents_ & EPOLLHUP)){
        LOG_FATAL<<"Channel::handle_event() EPOLLHUP"<<log::end;
        if(closeCallback) closeCallback();
    }
    //error事件回调
    if(revents_ & EPOLLERR){
        if(errorCallback)    errorCallback();
    }
    //read事件回调
    if(revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)){
        if(readCallback)   readCallback(recvTime);
    }
    //write事件回调
    if(revents_ & EPOLLOUT)
    {
        if(writeCallback) writeCallback();
    }
    eventHandling = false;
}

