#include"arpa/inet.h"
#include<thread>
#include<assert.h>
#include<errno.h>
#include<stdio.h>

#include"HttpServer.h"
#include"EpollTools.h"
#include"LogThread.h"
#include"ThreadPool.h"
#include"locker.h"

#define MAXFD 65536
#define MAXEVENTNUM 10000

typedef HttpServer HS;


int main()
{
  log::LogThread::init();
  printf("main(): pid = %d\n", getpid());
  
  const char* ip = "127.0.0.1";
  int port = 9527;

  addsig(SIGPIPE,SIG_IGN);

  ThreadPool< HttpServer>* tPool = new ThreadPool<HttpServer>;

  HS* users = new HS[MAXFD];
  int user_count = 0;

  int listenfd = socket(PF_INET, SOCK_STREAM , 0);
  assert(listenfd >= 0);
  //优雅的退出
  struct linger tmp = {1,0};
  setsockopt(listenfd,SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));

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

  epoll_event events[MAXEVENTNUM];
  int epollfd = epoll_create(5);
  assert(epollfd != -1);
  addfd(epollfd,listenfd,false);
  HS::epollfd_ = epollfd;

  while(1)
  {
    int num = epoll_wait(epollfd, events, MAXEVENTNUM,-1);
    std::cout << num << " events has been got" << std::endl;    
    LOG_INFO << num << " events has been got "<< log::end;
    if( (num < 0) && errno != EINTR)
    {
      LOG_FATAL << "epoll_wait failed.\n" << log::end;
      printf("epoll_wait failed.\n");
      break;
    }

    for(int i = 0; i < num; i++)
    {
      int sockfd = events[i].data.fd;
      std::cout << events[i].events << " "<< events->data.ptr << std::endl;
      if(sockfd == listenfd)
      {
        struct sockaddr_in cAddress;
        socklen_t len = sizeof(cAddress);
        int connfd = accept(listenfd,(struct sockaddr*)&cAddress,
                        &len);
        if(connfd < 0)
        {
          LOG_FATAL << "errno is " << errno << log::end;
          printf("errno is %d\n",errno);
          continue;
        }
        if(HS::userConn_cnt >= MAXFD)
        {
          showError(connfd,"INTERNAL SERVER BUSY");
          continue;
        }
        //初始化连接
        addfd(epollfd,connfd,false); 
        users[connfd].init( connfd,cAddress);
      }
      else if( events[i].events & ( EPOLLRDHUP | EPOLLHUP | EPOLLERR))
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
          users[sockfd].close();
        }
      }      
    }        
  }

  close(epollfd);
  close(listenfd);
  delete [] users;
  delete tPool;
  return 0;  
}