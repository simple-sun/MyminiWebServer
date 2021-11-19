#include "EventLoop.h"
#include "thread/Thread.h"
#include <stdio.h>
#include "syscall.h"
#include "unistd.h"


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

void threadFunc()
{
  printf("threadFunc(): pid = %d, tid = %d\n",
         getpid(), getpid());

  SUNSQ::EventLoop loop;
  loop.loop();
}

int main()
{
  printf("main(): pid = %d, tid = %d\n",
         getpid(), getpid());

  SUNSQ::EventLoop loop;

  //muduo::Thread thread(threadFunc);
  //thread.start();

  loop.loop();
   printf("到这就成功了");
  pthread_exit(NULL);

 
}



