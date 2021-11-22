#include "EventLoop.h"
#include "thread/Thread.h"
#include <stdio.h>
#include "syscall.h"
#include "unistd.h"
#include"Channel.h"
#include"Epoller.h"
#include<sys/timerfd.h>

/*
int main()
{
  SUNSQ::EventLoop loop;
  
  printf("到这就成功了");
  return 0;
}
*/

using namespace muduo;
using namespace SUNSQ;
SUNSQ::EventLoop* g_loop;
void timeout()
{
  
  printf("Timeout!\n");
  g_loop->quit();
}

int main()
{
  SUNSQ::EventLoop loop_;
  g_loop = &loop_;

  int timerfd = ::timerfd_create(CLOCK_MONOTONIC, 
                          TFD_NONBLOCK | TFD_CLOEXEC);
  SUNSQ::Channel channel(&loop_,timerfd);
  channel.setReadCallback(timeout); 
  channel.enableReading();

  struct itimerspec howlong;
  bzero(&howlong,sizeof(howlong));
  howlong.it_value.tv_sec = 5;
  ::timerfd_settime(timerfd,0,&howlong,NULL);
  
  (loop_).loop();

  ::close(timerfd);


  printf("到这就成功了");
  pthread_exit(NULL);

 
}



