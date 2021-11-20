#ifndef CHANNEL_H
#define CHANNAL_H


//#include"EventLoop.h"
#include<boost/function.hpp>

//#define MAX_EVENT_NUM_VAL 1024;


namespace SUNSQ{

    class EventLoop;

    class Channel{
    public:
        Channel();
        Channel(const Channel&) = delete;
        ~Channel();

        typedef boost::function<void()> EventCallback;
        Channel(EventLoop* loop, int fd);           //作为处理fd的函数，获取一个loop，以及fd

        void handleEvent();
        void setReadCallback(const EventCallback* cb)
        {   readCallback = cb;}
        void setWriteCallback(const EventCallback* cb)
        {   writeCallback = cb;}
        void setErrorCallback(const EventCallback* cb)
        {   errorCallback = cb;}

        int epollFd() const{   return epollfd_;}
        int events() const {    return events_;}
        void set_revents(int revt){ revents_ = revt;}
        bool eventsNull() const { return events_ == kNoEvents_;}

        void enableReading() { events_ |= kReadEvents_; update(); }
        void enableWriting() { events_ |= kWriteEvents_; update(); }
        void disableReading() { events_ &= kReadEvents_; update();}
        void disableWriting() { events_ &= kReadEvents_; update();}
        void disableALl() { events_ = kNoEvents_; update();}

        EventLoop* belongLoop() { return loop_;}

        //for poller,但是可能会因为要使用epoll而改变
        int index() {   return idx_;}
        void set_index(int idx) { idx_ = idx;}

        bool isNoneEvent()
        {
            return events_ == kNoEvents_;
        }
        

    private:
        void update();

        EventLoop* loop_;

        static const int kReadEvents_;
        static const int kWriteEvents_;
        static const int kNoEvents_ ;

        //epoll的成员
        const int epollfd_;
        int events_;
        int revents_;
        int MAX_EVENT_NUM;
        int idx_ ;


        EventCallback readCallback;
        EventCallback writeCallback;
        EventCallback errorCallback;

    };



}

#endif

