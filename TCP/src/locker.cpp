#include"locker.h"
#include"LogThread.h"

Sem::Sem()
{
    if(sem_init(&m_sem,0,0) != 0)
    {
        LOG_FATAL << "Sem::Sem() sem_init() exception." << log::end;
        throw std::exception();
    }
}
Sem::~Sem()
{
    sem_destroy(&m_sem);
}

Locker::Locker()
{
    if(pthread_mutex_init(&m_mutex, NULL) != 0)
    {
        LOG_FATAL << "Locker::Locker() pthread_mutex_init() exception." << log::end;
        throw std::exception();
    }
}
Locker::~Locker()
{
    pthread_mutex_destroy(&m_mutex);
}

Cond::Cond()
{
    if(pthread_mutex_init(&m_mutex,NULL) != 0)
    {
        LOG_FATAL << "Cond::Cond() pthread_mutex_init() exception." << log::end;
        throw std::exception();
    }   
    if(pthread_cond_init(&m_cond,NULL) != 0)
    {
        pthread_mutex_destroy(&m_mutex);
        LOG_FATAL << "Cond::Cond() pthread_cond_init() exception." << log::end;
        throw std::exception();
    }
}
Cond::~Cond()
{
    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_cond);
}

bool Cond::wait()
{
    int ret =0;
    pthread_mutex_lock(&m_mutex);
    ret = pthread_cond_wait(&m_cond,&m_mutex);
    pthread_mutex_unlock(&m_mutex);
    return ret == 0;
}

bool Cond::signal()
{
    return pthread_cond_signal(&m_cond);
}