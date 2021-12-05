#include "EventLoop.h"
#include <stdio.h>
#include "syscall.h"
#include "unistd.h"
//#include"Channel.h"
#include"Epoller.h"
#include"EventLoopThread.h"
#include<sys/timerfd.h>
#include<thread>
#include"LogThread.h"
#include<netinet/in.h>
#include"Acceptor.h"


using namespace SUNSQ;

void newConnection(int sockfd, const sockaddr_in& peerAddr)
{
  int n = ::write(sockfd,"how are you",13);
  printf("%d alphabat has been writed." , n);
  ::close(sockfd);
}

int main()
{
  log::LogThread::init();
  printf("main(): pid = %d\n", getpid());

  sockaddr_in listenAddr;
  listenAddr.sin_port = uint16_t( 9981 );
  EventLoop loop;
  int sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

  Acceptor acceptor(&loop, listenAddr);
  acceptor.setNewConnectionCallback(newConnection);
  acceptor.listenAcceptor();

  loop.loop();

  return 0;
}
