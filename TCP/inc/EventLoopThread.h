#ifndef EVENTLOOPTHREAD_H
#define EVENTLOOPTHREAD_H

#include"EventLoop.h"
#include<condition_variable>
#include<thread>
#include<mutex>

namespace SUNSQ{

class EventLoopThread
{
    public:
        EventLoopThread();
        ~EventLoopThread();

        SUNSQ::EventLoop* startLoop();

        void threadFunc();

    private:
   

    std::condition_variable cond_;
    std::thread thread_;    
    EventLoop* loop_;
    std::unique_lock<std::mutex> myLock_;
};

}
#endif