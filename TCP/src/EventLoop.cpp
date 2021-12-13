#include "EventLoop.h"
//#include "logging/Logging.h"
//#include<syslog.h>
#include<assert.h>
#include<poll.h>
#include"Channel.h"
#include"Epoller.h"
#include<stdio.h>
#include<iostream>
#include"sys/eventfd.h"
#include<mutex>
#include<thread>
//#include<syslog.h>
#include"LogThread.h"
#include"sys/socket.h"

using namespace SUNSQ;

thread_local EventLoop* t_loopInThisThread = 0;
const int kEpollTimeMs = 1000;

EventLoop::EventLoop()
        :   looping_(false),
            threadId_(getpid()),
            epoller_(new Epoller(this)),
            wakeupEventfd_(eventfd(0,EFD_CLOEXEC | EFD_NONBLOCK))
{  
    LOG_INFO << "EventLoop created "<< "in thread "<< threadId_ <<log::end;
    if( t_loopInThisThread){
        LOG_FATAL << " Another EventLoop " //<< t_loopInThisThread
                        << " exists in this thread "<< threadId_;
    }
    else{
        t_loopInThisThread = this;
    }
}

EventLoop :: ~EventLoop()
{
    assert(!looping_);          
    t_loopInThisThread = NULL;
}

void EventLoop :: loop()
{
    assert(!looping_);
    assertInLoopThread();
    looping_ = true; 
    quit_ = false;

    while (!quit_)
    {
        activeChannels_.clear();
        system_clock::time_point recvTime = epoller_->epoll(kEpollTimeMs,&activeChannels_);
        for( ChannelList::const_iterator it = activeChannels_.begin();
            it != activeChannels_.end(); it++)
        {
                (*it)->handleEvent(recvTime);
        }
        //std::cout<<"Reading...."<< threadId_ <<std::endl;
        doPendingFunctors();
    }    
    ::poll(NULL,0,1000);
    printf("EventLoop %d stop looping", this);

    looping_ = false;
}

int EventLoop :: epollfd() { return epoller_->epollfd() ;}


//EventLoop的静态成员函数返回当前的EventLoop对象
EventLoop* EventLoop :: getEventLoopOfCurrentThread(){
    return t_loopInThisThread;
}

void EventLoop::abortNotInLoopThread()
{
  LOG_FATAL << " EventLoop::abortNotInLoopThread " << log::end;
}


void EventLoop::updateChannel(Channel* channel)
{
    assert(channel->belongLoop() == this);
    assertInLoopThread();
    epoller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel)
{
    assert(channel->belongLoop() == this);
    assertInLoopThread();
    epoller_->removeChannel(channel);

}

void EventLoop::quit()
{
    quit_=true;
    if(! isInLoopThread())
    {
        wakeup();
    }
}


bool EventLoop::isInLoopThread() 
{
            pid_t id = getpid();
            return threadId_ == id;
}

void EventLoop::assertInLoopThread()
{
    if(!isInLoopThread()){
        abortNotInLoopThread();
    }
}

void EventLoop::runLoop(const Functors &cb)
{
    //如果调进来的函数的是当前线程的，
    //返回callback回调，否则放在队列中排队。
    if(isInLoopThread())
    {
        cb();
    }
    else
    {
        queueLoop(cb);
    }
}

//使用重载，这样就能实现两个功能，调取和插入cb
void EventLoop::queueLoop(const Functors &cb)
{
    {
        //pthread_mutex_lock(mutex_);
        mutex_.lock();
        
        queueLoop_.push(cb);
        mutex_.unlock();
    }
    if( !isInLoopThread() || callingPendingFunctors_)
    {
        wakeup();
    }    
}
boost::function<void()> EventLoop::queueLoop()
{
    boost::function<void()> cb = queueLoop_.front();
    queueLoop_.pop();
    return cb;    
}

void EventLoop::wakeup()
{
    uint64_t __buf = 0;    
    if(::send(wakeupEventfd_,&__buf,sizeof(__buf),0)<0){
        //日志记录错误
        LOG_FATAL <<  "EventLoop::wakeup() writes wrong" << log::end;
    }     
}

//写一个处理wakeupEventfd的函数，这样就能处理不同的事件了
void EventLoop::handleWakeupEventfd(const int &wakeupEventfd_)
{
    //wakeupEfd 如果是-1,说明error，如果>0,说明readable，
    if(wakeupEventfd_<0){
        //日志记录错误
    }else {
        handleRead();
    }    
}


void EventLoop::handleRead()
{
    uint64_t __buf = 0;
    if(::write(wakeupEventfd_,&__buf,8) < 0)
    {
        //日志记录错误        
        LOG_FATAL<<"EventLoop::handleRead wrong" << log::end;
    }    
}


void EventLoop::doPendingFunctors()
{
    std::queue<Functors> functors;
    callingPendingFunctors_ = true;
    {
        //有bug
        //pthread_mutex_lock(mutex_);
        mutex_.lock();
        
        functors.swap(queueLoop_);
        while(functors.size())
        {
            functors.front()();
            functors.pop();
        }
        callingPendingFunctors_ = false;
        mutex_.unlock();
    }
}

