#ifndef LOCKER_H
#define LOCKER_H

#include<exception>
#include<pthread.h>
#include<semaphore.h>


//封装信号
class Sem
{
public:
    Sem();
    ~Sem();

    bool wait() { return sem_wait(&m_sem) == 0;}
    bool post() { return sem_post(&m_sem) == 0;}
private:
    sem_t m_sem;
};

//封装互斥锁
class Locker
{
public:
    Locker();
    ~Locker();

    bool lock() { return pthread_mutex_lock(&m_mutex) == 0; }
    bool unlock() { return pthread_mutex_unlock(&m_mutex) == 0;}

private:
    pthread_mutex_t m_mutex;
};

//封装条件变量
class Cond
{
public:
    Cond();
    ~Cond();
    bool wait();
    bool signal();
private:
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
};

#endif