#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "boost/noncopyable.hpp"
#include"boost/scoped_ptr.hpp"
#include<sched.h>
#include<thread>
#include<vector>
//#include <linux/uni

namespace SUNSQ{

class Channel;
class Epoller;

class EventLoop : boost :: noncopyable{
    public:
        EventLoop();
        ~EventLoop();

        void loop();

        void assertInLoopThread();
        /*{
            if(!isInLoopThread())
            {
                abortNotInLoopThread();
            }
        }
        */

        

        EventLoop* getEventLoopOfCurrentThread();

        void updateChannel(Channel* channel);
        void quit();

    private:
        void abortNotInLoopThread();

        typedef std::vector<Channel*> ChannelList;

        bool quit_;
        bool looping_;
        const pid_t threadId_;
        //Epoller epoller_;
        boost::scoped_ptr<Epoller> epoller_;
        ChannelList activeChannels_;

};
}


#endif // SUNSQ_EVENTLOOP_H




