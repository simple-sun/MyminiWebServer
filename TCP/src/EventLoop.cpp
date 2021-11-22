#include "EventLoop.h"
//#include "logging/Logging.h"
#include<syslog.h>
#include<assert.h>
#include<poll.h>
#include"Channel.h"
#include"Epoller.h"
#include<stdio.h>
#include<iostream>


using namespace SUNSQ;

__thread EventLoop* t_loopInThisThread = 0;
const int kEpollTimeMs = 1000;

EventLoop::EventLoop()
            : looping_(false),
            threadId_(getpid()),
            epoller_(new Epoller(this))
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
    assert(!looping_);          //looping_如果是0，就说明没有EL，
                                //就不需要下一步
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
        epoller_->epoll(kEpollTimeMs,&activeChannels_);
        for( ChannelList::const_iterator it = activeChannels_.begin();
            it != activeChannels_.end(); it++)
            {
                (*it)->handleEvent();
            }
        std::printf("Reading.....");
        std::cout<<"Reading...."<< std::endl;
    }
    
    ::poll(NULL,0,1000);
    printf("EventLoop %d stop looping", this);

    looping_ = false;
}




//EventLoop的静态成员函数返回当前的EventLoop对象
EventLoop* EventLoop :: getEventLoopOfCurrentThread(){
    return t_loopInThisThread;
}

void EventLoop::abortNotInLoopThread()
{
  //日志记录文件
}


void EventLoop::updateChannel(Channel* channel)
{
    assert(channel->belongLoop() == this);
    assertInLoopThread();
    epoller_->updateChannel(channel);
}

void EventLoop::quit()
{
    quit_=true;
}


bool isInLoopThread() {
            pid_t id = getpid();
            //pid_t thisId = std::pthread_np::pthread_getthreadid_np();
            return 1;//EventLoop::threadId_ == id;
            //std::thread::id tid = std::this_thread::get_id();
        }

void EventLoop::assertInLoopThread()
{
    if(!isInLoopThread){
        abortNotInLoopThread();
    }
}