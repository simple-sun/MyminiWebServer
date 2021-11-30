//    #include"BufferUnion.h"
#include"Log.h"


using namespace SUNSQ;

Log::Log():running_(false),
            CurrentBuffer_(new Buffer),
            nextBuffer_(new Buffer)            
            {
                CurrentBuffer_->bzero();
                nextBuffer_->bzero();
                buffers_.reserve(16);
            }
Log::~Log(){}

void Log::append(const char* logline, int len)
{
    mutex_.lock();
    if(CurrentBuffer_->avail() > 0)
    {
        CurrentBuffer_->append(logline,len);
    }
    else{
        //需要开一个新的缓冲区
        buffers_.push_back(std::move(CurrentBuffer_));
        if(nextBuffer_)//一般情况
        {
            CurrentBuffer_ = std::move(nextBuffer_);
        }
        else{//没有next的情况，使用reset  
            CurrentBuffer_.reset( new Buffer);
            
        }
        CurrentBuffer_->append(logline,len);
        cond_.notify_all();
    }
    mutex_.unlock();
}


void Log::threadFunc()
{
    assert(running_ == false);
    
    //创建2块空闲缓存
    BufferPtr bufferFree1_ (new Buffer);
    BufferPtr bufferFree2_ (new Buffer);
    BufferVector bufferToWrite;
    bufferToWrite.reserve(16);

    //将bufferFree1_,2_填充为0
    bufferFree1_->bzero();
    bufferFree2_->bzero();    

    //处理事件,将running_设置为true



    while(running_)
    {
        assert( bufferFree1_ && bufferFree1_->length() == 0);
        assert( bufferFree2_ && bufferFree2_->length() == 0);
        assert( bufferToWrite.empty());

        mutex_.lock();
        buffers_.push_back(std::move(CurrentBuffer_));
        CurrentBuffer_ = std::move(bufferFree1_);
        buffers_.swap(bufferToWrite);
        if(!nextBuffer_)
        {
            nextBuffer_ = std::move( bufferFree2_) ;
        }        
        mutex_.unlock();
    }

    if(bufferToWrite.size() > 2){
        bufferToWrite.back().release();
        bufferToWrite.resize(2);
        //bufferToWrite.pop_back();
    }


    if(!bufferFree1_) 
    {
        assert(!bufferToWrite.size());
        bufferFree1_ = std::move( bufferToWrite.back() );
    }
    if( !bufferFree2_)
    { 
        assert(!bufferToWrite.size());
        bufferFree2_ = std::move( bufferToWrite.back());
    }
}