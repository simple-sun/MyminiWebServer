#include"arpa/inet.h"
#include<thread>
#include<assert.h>
#include<errno.h>
#include<stdio.h>
#include<ctime>
#include<sys/types.h>

#include"HttpServer.h"
#include"EpollTools.h"
#include"LogThread.h"
//#include"ThreadPool.h"
#include"locker.h"
#include"TcpConnect.h"
#include"ProcessPool.h"

#define MAXFD 65536


typedef HttpServer HS;


//int main(int argc, char* argv[])
int main6()
{
  //log::LogThread::init();
  printf("main: pid = %d\n", getpid());

  // const char* ip = argv[1];
  // int port = atoi(argv[2]);

  const char* ip = "127.0.0.1";
  int port = 9527;

  //addsig(SIGPIPE,SIG_IGN);

  int listenfd = socket(PF_INET, SOCK_STREAM,0);
  assert(listenfd >= 0);
  //优雅退出
  struct linger tmp = {1,0};
  setsockopt(listenfd,SOL_SOCKET,SO_LINGER,&tmp,sizeof(tmp));

  struct sockaddr_in address;
  bzero(&address,sizeof(address));
  address.sin_family = AF_INET;
  inet_pton(AF_INET,ip,&address.sin_addr);
  address.sin_port = htons(port);
  //close(listenfd);
  
  int ret = bind(listenfd,(struct sockaddr*)&address,sizeof(address));
  assert(ret >= 0);
  ret = listen(listenfd,5);
  //assert(ret >= 0);

  ProcessPool<HS>* pPool = new ProcessPool<HS>(listenfd);
  
  if(pPool)
  {
    
  pPool->run();
  delete pPool;
  }
  close(listenfd);
  return 0;

}