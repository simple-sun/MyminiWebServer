#include "Channel.h"
#include "EventLoop.h"
#include "sys/epoll.h"
//#include"logging/Logging.h"

#define MAX_EVENT_NUM_VAL 1024;

using namespace SUNSQ;

static const int kReadEvents_ = 0;
static const int kWriteEvents_ = EPOLLIN | EPOLLPRI;
static const int kNoEvents_ = EPOLLOUT;



Channel::Channel( EventLoop* loop, int fdA):loop_(loop),
        epollfd_(fdA),events_(0),revents_(0),idx_(-1),
        MAX_EVENT_NUM(1024) {}

void Channel :: update()
{
    loop_->updateChannel(this);
}

void Channel::handleEvent()
{
    if(revents_ & EPOLLERR){
        if(errorCallback)    errorCallback();
    }
    if(revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)){
        if(readCallback)   readCallback();
    }
    if(revents_ & EPOLLOUT)
    {
        if(writeCallback) writeCallback();
    }
}

