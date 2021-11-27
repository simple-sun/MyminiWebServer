#include"EventLoopThread.h"
#include"EventLoop.h"
#include<thread>


using namespace SUNSQ;

EventLoopThread::EventLoopThread(){ }
EventLoopThread::~EventLoopThread(){}

bool x()
{
    return 0;
}


EventLoop* EventLoopThread::startLoop()
{
    EventLoop loop;
    loop_ = & loop;
    thread_.hardware_concurrency();
    //此处应该是获得线程
    while(loop_ == NULL)
    {
        cond_.wait(myLock_);
        
    }

    return loop_;
}

void EventLoopThread::threadFunc()
{
    EventLoop loop;
    loop_ = &loop;
    cond_.notify_one();
    loop.loop();
}