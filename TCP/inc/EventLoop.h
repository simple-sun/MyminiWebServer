#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "boost/noncopyable.hpp"
#include"boost/scoped_ptr.hpp"
#include"boost/function.hpp"
#include<sched.h>
#include<thread>
#include<vector>
#include<queue>
#include"sys/eventfd.h"
#include<mutex>



namespace SUNSQ{

class Channel;
class Epoller;

class EventLoop : boost :: noncopyable{
    public:
        EventLoop();
        ~EventLoop();

        void loop();

        void assertInLoopThread();
        
        bool isInLoopThread();

        

        EventLoop* getEventLoopOfCurrentThread();

        void updateChannel(Channel* channel);
        void removeChannel(Channel* channel);
        void runLoop(const boost::function<void()> &cb);
        void queueLoop(const boost::function<void()> &cb);
        boost::function<void()> queueLoop();
        void handleWakeupEventfd(const int &wakeupEventfd_);
        int epollfd(); 
        

        void quit();


    private:
        void abortNotInLoopThread();
        void handleRead();  //读出wakeupFd_
        void doPendingFunctors();
        void wakeup();      //写入wakeupFd_

        typedef std::vector<Channel*> ChannelList;
        typedef boost::function<void()> Functors;
        bool callingPendingFunctors_;
        bool quit_;
        bool looping_;
        const pid_t threadId_;
        std::queue<Functors> queueLoop_;
        int wakeupEventfd_;

        std::mutex mutex_;     

        //Epoller epoller_;
        boost::scoped_ptr<Epoller> epoller_;
        ChannelList activeChannels_;
};
}


#endif // SUNSQ_EVENTLOOP_H




