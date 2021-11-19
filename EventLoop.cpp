#include "EventLoop.h"
//#include "logging/Logging.h"
#include<syslog.h>
#include<assert.h>


using namespace SUNSQ;

__thread EventLoop* t_loopInThisThread = 0;

EventLoop::EventLoop()
            : looping_(false),
            threadId_(getpid())
{
/*
    LOG_TRACE << "EventLoop created " << this << " in thread " << threadId_;
    if( t_loopInThisThread){
        LOG_FATAL << " Another EventLoop " << t_loopInThisThread
                        << " exists in this thread "<< threadId_;
    }
    else{
        t_loopInThisThread = this;
    }
    */
}

EventLoop :: ~EventLoop()
{
    assert(!looping_);          //looping_如果是0，就说明没有EL，就不需要析构
    t_loopInThisThread = NULL;
}

void EventLoop :: loop()
{
    assert(!looping_);
    assertInLoopThread();
    looping_ = true; 
}




//EventLoop的静态成员函数返回当前的EventLoop对象
EventLoop* EventLoop :: getEventLoopOfCurrentThread(){
    return t_loopInThisThread;
}

