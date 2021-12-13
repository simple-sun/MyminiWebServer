#include "EventLoop.h"
#include <stdio.h>
#include"Epoller.h"
#include"LogThread.h"
#include"Acceptor.h"
#include"TcpServer.h"
#include<TcpConnection.h>
#include"arpa/inet.h"

using namespace SUNSQ;

std::string message1;
std::string message2;

void onConnection(const TcpConnection::TcpConnectionPtr& conn)
{
  if(conn->connected())
  {
    printf("onConnection(): new connection [%s] from %s\n",
              conn->name().c_str(), 
              (inet_ntoa(conn->peerAddress().sin_addr)));
    conn->send(message1);
    conn->send(message2);
    conn->shutdown();
  }
  else
  {
    printf("onConnection(): connection [%s] is down\n",
            conn->name().c_str() );
  }
}

void onMessage(const TcpConnection::TcpConnectionPtr& conn,
                TcpBuffer* bufData, system_clock::time_point recvTime)
{
  printf("onMessage(): received %zd bytes from connection [%s] at %s\n",
          bufData->readableBytes(), conn->name().c_str(),recvTime );
  bufData->reset();
}



int main(int argc, char* argv[])
{
  log::LogThread::init();
  printf("main(): pid = %d\n", getpid());

  int len1 = 100, len2 = 200;

  if(argc > 2)
  {
    len1 = atoi(argv[1]);
    len2 = atoi(argv[2]);
  }

  message1.resize(len1);
  message2.resize(len2);

  std::fill(message1.begin(),message1.end(),'A');
  std::fill(message2.begin(),message2.end(),'B');

  sockaddr_in listenAddr;
  bzero(&listenAddr,sizeof(listenAddr));
  listenAddr.sin_family = AF_INET;
  // const char* ip = "1.2.3.4";
  // inet_pton(AF_INET,ip,&listenAddr.sin_addr);
  listenAddr.sin_port = htons(9527);
  //listenAddr.sin_addr.s_addr = htonl(INADDR_ANY) ;


  EventLoop loop;

  TcpServer server(&loop,listenAddr);
  server.setConnectionCallback(onConnection);
  server.setMessageCallback(onMessage);
  server.start();  
  // while(1)
  // {
  //   server.start();
  //   printf("server is connencting \n");
  // }
  

  loop.loop();

  return 0;
}
