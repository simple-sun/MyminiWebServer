#ifndef EPOLL_H
#define EPOLL_H


/*Poller 是EvnentLoop的间接成员，只供其owner EventLoop
在IO 线程调用，因此不用加锁。
*/
#include<vector>
#include"EventLoop.h"
#include"sys/epoll.h"
#include"chrono"
#include<map>

namespace SUNSQ
{
    class Channel;

    class Epoller{
    public:
        Epoller(EventLoop* loop_);
        Epoller(const Epoller&) = delete;
        ~Epoller();

        typedef std::vector<Channel*> ChannelList;

        //Epoll I/O 事件，在loop_的线程中唤起
        std::chrono::system_clock::time_point epoll(int timeoutMs, ChannelList* 
                    activeChannels);

        //改变I/O 事件，在loop_的线程中唤起
        void updateChannel(Channel* channel);

        //当通道损坏，移除通道，
        void removeChannel(Channel* chanenl);

        void assertInLoopThread() 
        {
            //ownerLoop_->assertInLoopThread();
        }
        int epollfd() {return epollfd_;}
        

    private:
        static const int kInitEventListSize = 16;
        void fillActiveChannels(int numEvents,
                                ChannelList* acitveChannels) const;

        void update(int operation, Channel* channel);

        typedef std::vector<epoll_event> EventList;
        typedef std::map <int, Channel*> ChannelMap;

        EventLoop* ownerLoop_;
        int epollfd_;
        EventList events_;
        ChannelMap channels_;
    };
}


#endif