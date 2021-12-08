#include "EventLoop.h"
#include <stdio.h>
// #include "syscall.h"
// #include "unistd.h"
//#include"Channel.h"
#include"Epoller.h"
// #include"EventLoopThread.h"
// #include<sys/timerfd.h>
// #include<thread>
#include"LogThread.h"
// #include<netinet/in.h>
#include"Acceptor.h"
#include<TcpConnection.h>


using namespace SUNSQ;

void onConnection(const TcpConnection::TcpConnectionPtr& conn)
{
  if(conn->connected())
  {
    printf("onConnection(): new connection [%s] from %s\n",
              conn->name().c_str(), conn->peerAddress().sin_port);
  }
  else
  {
    printf("onConnection(): connection [%s] is down\n",
            conn->name().c_str() );
  }
}

void onMessage(const TcpConnection::TcpConnectionPtr& conn,
                const char* data, ssize_t len)
{
  printf("onMessage(): received %zd bytes from connection [%s]\n",
          len, conn->name().c_str());
}



int main()
{
  log::LogThread::init();
  printf("main(): pid = %d\n", getpid());

  sockaddr_in listenAddr;
  listenAddr.sin_port = 9981;
  EventLoop loop;

  TcpConnection conn;


  loop.loop();

  return 0;
}
