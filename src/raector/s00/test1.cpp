#include "EventLoop.h"
#include <thread>
#include "thread/Thread.h"
#include <iostream>

void threadFunc(){
    std::cout<< "threadFunc() : pid = "<< syscall(SYS_gettid) <<" , tid = "
    << muduo :: CurrentThread :: tid();

    SUNSQ :: EventLoop loop;
    loop.loop();
}

int main1()
{
    std::cout<< "main() : pid = "<< syscall(SYS_gettid) <<" , tid = "
    << muduo :: CurrentThread :: tid();

    SUNSQ :: EventLoop loop;

    muduo::Thread thread(threadFunc) ;
    thread.start();


    loop.loop();
    pthread_exit(NULL);
}//