#include "EventLoop.h"
#include <stdio.h>
#include "syscall.h"
#include "unistd.h"
#include"Channel.h"
#include"Epoller.h"
#include"EventLoopThread.h"
#include<sys/timerfd.h>
#include<thread>
#include"LogThread.h"

/*
int main()
{
  SUNSQ::EventLoop loop;
  
  printf("到这就成功了");
  return 0;
}
*/

 
using namespace SUNSQ;
SUNSQ::EventLoop* g_loop;
int g_flag = 0;

void run4()
{
  printf("run4(): pid = %d, flag = %d\n", getpid(), g_flag);
  g_loop->quit();
}

void run3()
{
  printf("run3(): pid = %d, flag = %d\n", getpid(), g_flag);
  //g_loop->runAfter(3, run4);
  g_flag = 3;
}

void run2()
{
  printf("run2(): pid = %d, flag = %d\n", getpid(), g_flag);
  g_loop->queueLoop(run4);
}

void run1()
{
  g_flag = 1;
  printf("run1(): pid = %d, flag = %d\n", getpid(), g_flag);
  g_loop->runLoop(run2);
  g_flag = 2;
}


void timeout()
{
  
  printf("Timeout!\n");
  g_loop->quit();
}

void runInThread()
{
  printf("runInThread(): pid = %d, tid = %d\n",
         getpid(),std::this_thread::get_id());
}

int main()
{
  log::LogThread::init();
  SUNSQ::EventLoop loop_;
  g_loop = &loop_;

  printf("runInThread(): pid = %d, tid = %d\n",
         getpid(),std::this_thread::get_id());


  EventLoopThread loopThread;
  EventLoop* loop = loopThread.startLoop();
  loop->runLoop(runInThread);
  sleep(1);
  loop->quit();


  
  loop_.loop();

   printf("main(): pid = %d, flag = %d\n", getpid(), g_flag);


  printf("到这就成功了"); 
}



