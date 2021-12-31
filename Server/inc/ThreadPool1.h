#ifndef THREADPOOL_H
#define THREADPOOL_H

#include<list>
#include<cstdio>
#include<exception>
#include<pthread.h>
#include"locker.h"
#include"LogThread.h"

template<typename T>
class ThreadPool
{
public:
    ThreadPool(int thread_number = 8, int max_request = 10000);
    ~ThreadPool();

    bool append(T* request);

private:
    static void* workPool(void* arg);
    void run();

    int thread_number_;    //线程池中的线程数
    int max_requests_;     //请求队列中允许的最大请求数
    pthread_t* threads_;    //描述线程池的数组
    std::list<T*> workqueue_;  //请求队列
    Locker queuelocker_;   //保护请求队列
    Sem queuestat_;        //是否有待处理任务
    bool stop_;            //
};
template<typename T>
ThreadPool< T >::ThreadPool(int thread_number, int max_request)
                :thread_number_(thread_number),
                max_requests_(max_request),
                stop_(false),
                threads_(NULL)
{
    if(thread_number <= 0 || max_requests_ <= 0)
    {
        LOG_FATAL << "ThreadPool exception:: thread_number <= 0 || max_requests <= 0." << log::end;
        throw std::exception();
    }
    threads_ = new pthread_t[thread_number];
    if(!threads_)
    {
        LOG_FATAL << "ThreadPool exception:: thread is not exist." << log::end;
        throw std::exception();
    }
    //创建thread_number_个线程
    for(int i = 0; i < thread_number; i++)
    {
        printf("the %d thread is created\n", i);
        if(pthread_create(threads_+i, NULL, workPool, this) != 0)
        {
            delete [] threads_;            
            throw std::exception();
        }
        if(pthread_detach(threads_[i]))
        {
            delete [] threads_;
            LOG_FATAL << "ThreadPool exception:: pthread_detach fail." << log::end;
            throw std::exception();
        }
    }
}

template<typename T>
ThreadPool< T >::~ThreadPool()
{
    delete [] threads_;
    stop_ = true;
}

template<typename T>
bool ThreadPool< T >::append(T* request)
{
    queuelocker_.lock();
    if( workqueue_.size() > max_requests_)
    {
        queuelocker_.unlock();
        return false;
    }
    workqueue_.push_back(request);
    queuelocker_.unlock();
    queuestat_.post();
    return true;
}

template<typename T>
void* ThreadPool< T >::workPool(void* arg)
{
    ThreadPool* pool = (ThreadPool*)arg;
    pool->run();
    return pool;
}

template<typename T>
void ThreadPool< T >::run()
{
    while(!stop_)
    {
        queuestat_.wait();
        queuelocker_.lock();
        if(workqueue_.empty())
        {
            queuelocker_.unlock();
            continue;
        }
        T* request = workqueue_.front();
        workqueue_.pop_front();
        queuelocker_.unlock();
        if( !request)
        {
            continue;
        }
        request->process();
    }
}
#endif