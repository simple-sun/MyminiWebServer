#include"EventLoop.h"
#include"Channel.h"
#include"Epoller.h"

using namespace SUNSQ;

const int kNew = -1;
const int kAdd = 1;
const int kDel = 2;


Epoller::Epoller(EventLoop* loop_) : ownerLoop_(loop_),
            epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
            events_(kInitEventListSize) {} ;
Epoller::~Epoller(){ ::close(epollfd_);}

void Epoller::epoll(int timeoutMs, ChannelList* activeChannels)
{
    int numEvents = epoll_wait(epollfd_,&*events_.data(),
    events_.size(),timeoutMs);
    if(numEvents > 0)
    {
        //此处应有日志写： <<numEvents << " events happened."
        fillActiveChannels(numEvents,activeChannels);
        if(events_.size() == numEvents){
            events_.resize(events_.size() * 2);
        }
    }
    else if ( numEvents == 0)
    {
        //无操作
        //此处应有日志写： << " nothing happened"
    }
    else
    {
        //此处应有日志写：<< "Epoller::poll() ";
    }
}
void Epoller::fillActiveChannels( int numEvents,
                                   ChannelList* acitveChannels) const
{
    assert(numEvents <= events_.size());
    for(int i = 0; i < numEvents; i++)
    {
        Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
        //检查无效变量
        #ifndef NDEBUG
            int epollfd = channel->epollFd();
            ChannelMap::const_iterator it = channels_.find(epollfd);
            assert(it != channels_.end());
            assert(it->second == channel);  //感觉这句话是废话阿
        #endif
        channel->set_revents(events_[i].events);
        acitveChannels->push_back(channel);
    }
}


void Epoller::update(int operation, Channel* channel)
{
    struct epoll_event event;
    bzero(&event,sizeof(epoll_event));
    event.data.ptr = channel;
    int epollfd = channel->epollFd();

    if(::epoll_ctl(epollfd_, operation, epollfd, &event) < 0)
    {
        if(operation == EPOLL_CTL_DEL)
        {
            //应该有日志记录， << " epoll_ctl op=" << operation
            //  <<" fd="<<epollfd;
        }else{
            //应该有日志记录， << " epoll_ctl op=" << operation
            //  <<" fd="<<epollfd;
        }
    }    
}


void Epoller::updateChannel(Channel* channel)
{
    assertInLoopThread();
    //日志记录，<< " fd= " << channel->fd() << " events="
    //          <<channel->events();
    const int index = channel->index();
    if( index == kNew || index == kDel )
    {
        int fd = channel->epollFd();
        if( index == kNew)
        {
            assert(channels_.find(fd) == channels_.end());
            channels_[fd] = channel;
            //muduo 写在if的最后了，但是我觉得应该写在这
            update(EPOLL_CTL_ADD,channel);  
        }
        else    //index == kDel
        {
            assert(channels_.find(fd) != channels_.end());
            assert(channels_[fd] == channel);
            //update(EPOLL_CTL_DEL, channel);     //muduo没写这个，但是觉得应该有
        }
        channel->set_index(kAdd);
    }
    else    //index = kAdd
    {
        int fd = channel->epollFd();
        assert(channels_.find(fd) == channels_.end() );
        assert(channels_[fd] == channel);
        if(channel->isNoneEvent())
        {
            //说明channel中event = 0.
            //删除fd上的注册事件
            update(EPOLL_CTL_DEL, channel);
            channel->set_index(kDel);
        }
        else{
            update(EPOLL_CTL_MOD,channel);            
        }
    }
}