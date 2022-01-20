#ifndef THREADPOOL_H
#define THREADPOOL_H

#include<list>
#include<pthread.h>
#include<stdio.h>

#include"locker.h"
#include"LogThread.h"



template<typename T>
class ThreadPool
{
public:
    ThreadPool(int threadnum = 4);
    ~ThreadPool();

    void append(T* req);

private:
    static void* workfunc(void* arg);
    void run();

    pthread_t* thread_;
    int threadNum;
    std::list<T*> workqueue;
    Locker queueLock_;
    Sem queueStat_;
    bool stop_;
};

template<typename T> 
ThreadPool<T>:: ThreadPool(int threadnum)
        :threadNum(threadnum),
        stop_(false),
        thread_(NULL)
{
    thread_ = new pthread_t[threadNum];

    //创建threadNum个线程，并设置为脱离状态
    for(int i = 0; i< threadnum; i++)
    {
        int ret = pthread_create(thread_+i,NULL,workfunc,this);
        if(ret != 0)
        {

            printf("%d thread creat falied\n", i);
        }
        printf("%d thread is created\n",i);
        LOG_INFO << i << " thread is created." << log::end;

        if(pthread_detach(thread_[i]))
        {
            delete [] thread_;
            printf("%d thread is detached\n", i);
            LOG_INFO << i << " thread is detached." << log::end;

        }
    }
}

template<typename T>
ThreadPool<T> :: ~ThreadPool()
{
    delete [] thread_;
    stop_ = true;
}
template<typename T>
void ThreadPool<T> :: append(T* req)
{
    queueLock_.lock();
    workqueue.push_back(req);
    queueLock_.unlock();
    queueStat_.post();
}
template<typename T>
void* ThreadPool<T> ::workfunc(void* arg)
{
    ThreadPool* pool = (ThreadPool*)arg;
    pool->run();
    return pool;
}

template<typename T>
void ThreadPool<T> :: run()
{
    while(!stop_)
    {
        queueStat_.wait();
        queueLock_.lock();
        if( ! workqueue.empty())
        {
            
            T* req = workqueue.front();
            workqueue.pop_front();
            queueLock_.unlock();
            req->process();          
        }
        else{
            queueLock_.unlock();
        }
    }
}


#endif