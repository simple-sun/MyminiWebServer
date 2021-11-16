//#include<boost/scoped_ptr.hpp>

//#include "thread/Thread.h"
#include<thread>
#include <sys/syscall.h>
#include "Channel.h"
//#include<system.Class.hpp>


namespace SUNSQ{

class EventLoop : boost :: noncopyable
{
public:
    EventLoop();        //使用默认构造函数
    ~EventLoop();

    void loop();

    void assertInLoopThread(){
        if( ! isInLoopThread())
        {
            abortNotInLoopThread();
        }

    }

    void updateChannel(Channel *Channel){

    }

    bool isInLoopThread() const { 
        //std::thread idThisthread;
        //std::thread 0Id = 0;
        //auto tid = idThisthread.get_id();
        //const pid_t tid = std::thread::get_id();    
        int tid = syscall(SYS_gettid);

        return  tid; 
    }
    
private:

    void abortNotInLoopThread();

    bool looping_;              //原子操作
    const pid_t threadId_;
    //pid_t tid() = std :: thread :: get_id();
    

};
}
