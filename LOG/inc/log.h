#ifndef LOG_H
#define LOG_H


#include<syslog.h>
#include<string>
#include<boost/ptr_container/ptr_vector.hpp>
#include"BufferUnion.h"
#include<mutex>
#include<condition_variable>


//参考muduo编写多线程异步日志
using namespace SUNSQ;

namespace SUNSQ
{

class Log
{
public:

private:

    //自己写一个buffer，11-27
    typedef BufferUnion<SUNSQ::BUFFERSIZE> Buffer;
    typedef boost::ptr_vector<Buffer> BufferVector;
    //typedef BufferVector::auto_type BufferPtr;
    typedef std::unique_ptr<Buffer> BufferPtr;

    std::mutex mutex_;
    std::condition_variable cond_;

    BufferPtr CurrentBuffer_;   //当前缓冲
    BufferPtr nextBuffer_;      //预备缓冲
    std::vector<Buffer> buffers_;         //待写入文件的已填满的缓冲


};


}



#endif