#include"sys/socket.h"
#include"sys/uio.h"
#include<assert.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>

int mai()
{
    const char* ip = "172.22.30.196";
    int port = 9527;

    int listenfd = socket(PF_INET, SOCK_STREAM , 0);
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
    
    // int epollfd = epoll_create(5);
    // assert(epollfd != -1);
    // addfd(epollfd,listenfd,false);
    return 1;


}