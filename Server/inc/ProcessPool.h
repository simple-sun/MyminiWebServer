#ifndef PROCESSPOLL_H
#define PROCESSPOLL_H

#include<sys/socket.h>
#include<sys/epoll.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/unistd.h>
#include<errno.h>
#include<iostream>
#include<assert.h>
#include<sys/wait.h>

#include"LogThread.h"
#include"EpollTools.h"

#define MAXEVENTNUM 65536
#define USERNUM     10000
static int sigPipefd[2];

class Process
{
public:
    Process():pid_(-1){}

    pid_t pid_;
    int pipefd_[2];
};

template<typename T>
class ProcessPool
{
public:
    ProcessPool(int listenfd);
    ~ProcessPool();

    void run();
private:
    void runChild();
    void runParent();
    void setPiped();

private:
    int epollfd_;
    int index_; //父进程是-1,子进程是大于等于0
    int listenfd_;
    bool stop_;
    int ChildNum;
    Process* subProcess;
    

};


template<typename T>
ProcessPool< T>::ProcessPool(int listenfd)
            :listenfd_(listenfd),
            index_(-1),
            ChildNum(8),
            stop_(false)
{
    subProcess = new Process[ChildNum];
    for(int i = 0; i < ChildNum; i++)
    {
        int ret = socketpair(PF_INET,SOCK_STREAM,0,subProcess[i].pipefd_);
        assert(ret == 0);
        subProcess[i].pid_ = fork();
        if(subProcess->pid_ > 0)
        {
            close(subProcess[i].pipefd_[1]);
            continue;
        }
        else 
        {
            close(subProcess[i].pipefd_[0]);
            index_ = i;
            break;
        }
    }
}
template<typename T>
ProcessPool<T>::~ProcessPool()
{
    delete [] subProcess;
    close(epollfd_);
}

//private
static void sigHandler(int sig)
{
    int saveErrno = errno;
    int msg = sig;
    send(sigPipefd[1],(char*) &msg,1,0);
    errno = saveErrno;
}
template<typename T>
void ProcessPool<T>::setPiped()
{
    epollfd_ = epoll_create(5);

    int ret = socketpair(PF_INET,SOCK_STREAM,0,sigPipefd);

    setNonblocking(sigPipefd[1]);
    addfd(epollfd_,sigPipefd[0],false);

    addsig(SIGCHLD,sigHandler);
    addsig(SIGTERM,sigHandler);
    addsig(SIGINT,sigHandler);
    addsig(SIGPIPE,SIG_IGN);
    
}

template<typename T>
void ProcessPool<T>::run()
{
    if(index_ >= 0)
    {
        runChild();
    }
    runParent();
}
template<typename T>
void ProcessPool<T>::runParent()
{
    setPiped();

    epollfd_ = epoll_create(5);
    addfd(epollfd_,listenfd_,false);

    epoll_event events[MAXEVENTNUM];
    int subNum = 0;
    int pipeSig = 1;

    while(!stop_)
    {
        int num = epoll_wait(epollfd_,events,sizeof(events),-1);
        if( num < 0 )
        {
            std::cout << "runParent() epoll_wait failed." << std::endl;
            LOG_FATAL << "epoll_wait failed." << log::end;
        }
        else if( num == 0)
        {
            std::cout << "no events waited." << std::endl;
            LOG_FATAL << "no events waited." << log::end;
        }
        else
        {
            for(int i = 0; i < sizeof(events); i++)
            {
                int sockfd = events[i].data.fd;
                if(sockfd == listenfd_)
                {
                    //有新的连接，寻找空闲的子进程，传递
                    int j = subNum;
                    do
                    {
                        //说明这个子进程被创建了，没有被取消
                        if(subProcess[j].pid_ != -1)
                        {
                            break;
                        }
                        j = (j++)%ChildNum;                        
                    } while (j != subNum); 
                    subNum = (j+1)%ChildNum;   
                    send(subProcess[j].pipefd_[1],(char*)pipeSig,sizeof(pipeSig),0);   
                    LOG_INFO << "send " << sockfd << " to subProcess " << j << log::end;            
                }
                //处理父进程的信号
                else if((sockfd == sigPipefd[0]) && (events[i].events == EPOLLIN))
                {
                    int sigarr[1536];
                    int ret = recv(sigPipefd[0],sigarr,sizeof(sigarr),0);
                    assert( ret > 0);
                    for(int j = 0; j < sizeof(sigarr); j++)
                    {
                        //子进程状态发生变化
                        if(sigarr[j] == SIGCHLD)
                        {
                            pid_t tpid;
                            int stat;
                            while((tpid = waitpid(-1,&stat,WNOHANG)) > 0)
                            {
                                for(int k = 0; k < ChildNum; k++)
                                {
                                    if(subProcess[i].pid_ == tpid)
                                    {
                                        LOG_INFO << "subprocess[" << i << "] close" << log::end;
                                        subProcess[i].pid_ = -1;
                                        close(subProcess[i].pipefd_[0]);                                        
                                    }
                                }
                                for(int k = 0; k < ChildNum; k++)
                                {
                                    if(subProcess[i].pid_ != -1)
                                    {
                                        break;
                                    }
                                    stop_ = true;
                                } 
                            }                                                                                     
                        }
                        // 中止进程
                        else if(sigarr[j] == SIGTERM)
                        {

                        }
                        //键盘输入，中断进程
                        else if(sigarr[j] == SIGINT)
                        {
                            LOG_INFO << " kill all subProcess" << log::end;
                            for( int k = 0; k < ChildNum; k++)
                            {
                                int tpid = subProcess[k].pid_;
                                if(tpid != -1)
                                {
                                    kill(tpid,SIGTERM);
                                }
                            }
                        }
                    }
                }
                else
                {
                    continue;
                }
            }
        }
    }    
    close(epollfd_); 
}


template<typename T> 
void ProcessPool<T>::runChild()
{
    setPiped();

    int pipefd = subProcess[index_].pipefd_[1];
    add(epollfd_,pipefd);

    epoll_event events[MAXEVENTNUM];
    T* users = new T[USERNUM];
    int ret = 0;

    while(!stop_)
    {
        int num = epoll_wait(pipefd,events,sizeof(events),-1);
        if(num < 0)
        {
            LOG_FATAL << "epoll_wait num = " << num << log::end;
        }
        else if(num == 0)
        {
            LOG_INFO << num << "event happened" << log::end;
        }
        else
        {
            for(int i = 0; i < sizeof(events);i++)
            {
                int sockfd = events[i];
                if(sockfd == pipefd && events[i].events == EPOLLIN)
                {
                    int buf = 0;
                    ret = recv(sockfd,buf,sizeof(buf),0);
                    if(ret < 0)
                    {
                        //说明父进程没有传进来信号
                        continue;
                    }    
                    else
                    {
                        struct sockaddr_in address;
                        socklen_t addlen = sizeof(address);
                        int connfd = accept(listenfd_,&address,addlen);
                        if(connfd < 0)
                        {
                            LOG_FATAL << "accept() : connfd = " << connfd << log::end;
                        }
                        addfd(epollfd_,connfd);
                        users[sockfd].init(connfd,address)
                    }
                }
                else if(sockfd == sigPipefd[0] && events[i].events == EPOLLIN)
                {
                    int sigarr[1536];
                    ret = recv(sigPipefd[0],sigarr, sizeof(sigarr),0);
                    if(ret < 0)
                    {
                        //说明父进程没有传进来信号
                        continue;
                    } 
                    for(int j = 0; j < sizeof(sigarr); j++)
                    {
                        if(sigarr[j] == SIGCHLD)
                        {
                            pid_t tpid;
                            int stat;
                            while((tpid = waitpid(-1,&stat,WNOHANG)) > 0)
                            {
                                continue;
                            }
                        }
                        if(sigarr[j] == SIGTERM)
                        {

                        }
                        if(sigarr[j] == SIGINT)
                        {
                            LOG_INFO << "subProcess [" << j <<"] killed." << log::end;
                            stop_ = true;
                        }
                    }
                }
                else if(events[i].events == EPOLLIN)
                {
                    users[sockfd].process();
                }
            }
        }
    }
}

#endif