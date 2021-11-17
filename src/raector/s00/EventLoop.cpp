#include "EventLoop.h"

#include "logging/Logging.h"

#include <assert.h>
#include <poll.h>

using namespace muduo;
using namespace SUNSQ;

__thread  EventLoop* t_loopInThisThread = 0 ;

EventLoop :: EventLoop() :looping_(false), threadId_(syscall(SYS_gettid))
{
    LOG_TRACE << "EventLoop created " << this << " in thread " << threadId_;
    if (t_loopInThisThread)
    {
        LOG_FATAL << " Another EventLoop " << t_loopInThisThread
                            << " exists in this thread " << threadId_;
    }
    else{
        t_loopInThisThread = this;
    }
}

EventLoop :: ~EventLoop()
{
    assert( ! looping_ ) ;
    t_loopInThisThread = NULL ;
}

/*
EventLoop* EventLoop :: getEventLoopOfCurrentThread()
{
    return t_loopInThisThread ;
}
*/

void EventLoop :: loop()
{
    assert( ! looping_) ;
    assertInLoopThread();
    looping_ = true ;

    ::poll ( NULL, 0, 5*1000 );

    LOG_TRACE << "EventLoop " << this << " stop looping " ;
    looping_ = false ;
}

void EventLoop :: abortNotInLoopThread()
{
    LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop " << this
            << " was created in threadid_ = " << threadId_
            << " , current thread id = "<< syscall(SYS_gettid) ;
}