#include"arpa/inet.h"
#include<thread>
#include<assert.h>
#include<errno.h>
#include<stdio.h>
#include<ctime>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>

#include"HttpServer.h"
#include"EpollTools.h"
#include"LogThread.h"
#include"ThreadPool.h" 
#include"locker.h"

#define MAXFD 65536
#define MAXEVENTNUM 10000

typedef HttpServer HS;

int main( int argc,char* argv[] )
{
  log::LogThread::init();
  //打印线程号
  //printf("main(): pid = %d\n", getpid());
  
  //打印输入的ip地址，端口号
  //printf("argc = %d, argv[0] = %s, argv[1] = %s\n", argc,argv[0],argv[1]);

  const char* ip = "172.22.30.196";
  int port = 9527;

  addsig(SIGPIPE,SIG_IGN);

  ThreadPool< HttpServer>* tPool = new ThreadPool<HttpServer>(2);

  HS* users = new HS[MAXFD];
  assert(users);
  int user_count = 0;  

  int listenfd = socket(PF_INET, SOCK_STREAM , 0);
  assert(listenfd >= 0);
  //设置退出机制
  struct linger tmp = {1,0};
  setsockopt(listenfd,SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
  //实现端口复用
  int temp = 1;
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof(temp));

  int ret = 0;
  struct sockaddr_in address;
  bzero(&address,sizeof(address));
  address.sin_family = AF_INET;
  inet_pton(AF_INET,ip,&address.sin_addr);
  address.sin_port = htons(port);

  ret = bind(listenfd,(struct sockaddr*)&address, sizeof(address));
  assert(ret >= 0);

  

  ret = listen(listenfd,5);
  assert(ret >= 0);
  
  int epollfd = epoll_create(5);
  assert(epollfd != -1);
  addfd(epollfd,listenfd,false);

  epoll_event events[MAXEVENTNUM];
  HS::epollfd_ = epollfd;  

  while(1)
  {
    int num = epoll_wait(epollfd, events, MAXEVENTNUM,-1);   
    //assert(num >= 0);
    //打印epoll事件数目
    //printf("%d events has been got.\n", num);
    LOG_INFO << num << " events has been got "<< log::end;

    for(int i = 0; i < num; i++)
    {      
      int sockfd = events[i].data.fd;
      if(sockfd == listenfd)
      {
        struct sockaddr_in cAddress;
        socklen_t len = sizeof(cAddress);
        int connfd = accept(listenfd,(struct sockaddr*)&cAddress,
                        &len);
        if(connfd < 0)
        {
          LOG_FATAL << "errno is " << errno << log::end;
          //发生errno，打印errno事件
          //printf("errno is %d\n",errno);
          continue;
        }
        if(HS::userConn_cnt >= MAXFD)
        {
          showError(connfd,"INTERNAL SERVER BUSY");
          continue;
        }
        //初始化连接
        users[connfd].init( connfd,cAddress);
      }
      else if( events[i].events & ( EPOLLRDHUP |  EPOLLERR | EPOLLHUP))
      {
        //遇到异常，关闭连接
        users[sockfd].close();
        LOG_FATAL << "events[" << sockfd << "]." << events[i].events << log::end;
      }
      else if (events[i].events & EPOLLIN)
      {
        //由读的结果，判断下一步
        if( users[sockfd].read())
        {          
          // users[sockfd].process();
          tPool->append(users+sockfd);
        }
        else
        {
          users[sockfd].close();
        }
      }
      else if(events[i].events & EPOLLOUT)
      {
        if( !users[sockfd].write())
        {
          sleep(1);
          users[sockfd].close();
        }  
      }      
      else
      {
        printf("something else happened.");
      }
    }        
  }

  close(epollfd);
  close(listenfd);
  delete [] users;
  delete tPool;
  return 0;  
}


