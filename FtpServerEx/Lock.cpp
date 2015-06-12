#include "Lock.h"

/*  Mutex  */
mu::Mutex::Mutex()
{
    pthread_mutex_init(&_mutex, NULL);
}

mu::Mutex::~Mutex()
{
    pthread_mutex_destroy(&_mutex);
}

bool mu::Mutex::lock()
{
    return pthread_mutex_lock(&_mutex) == 0;
}

bool mu::Mutex::unlock()
{
    return pthread_mutex_unlock(&_mutex) == 0;
}

pthread_mutex_t *mu::Mutex::getMutex()
{
    return &_mutex;
}

/*  ScopeLock  */
mu::ScopeLock::ScopeLock(Mutex &mutex)
    :_mutex(mutex)
{
    _mutex.lock();
}

mu::ScopeLock::~ScopeLock()
{
    _mutex.unlock();
}

/*  RWLock  */
mu::RWLock::RWLock()
    :_readCount(0)
{
    ;
}

mu::RWLock::~RWLock()
{
    ;
}

bool mu::RWLock::rLock()
{
    bool ret = true;

    if(_readCount == 0)
    {
        ScopeLock lock(_rMutex);
        if(_readCount == 0)
            ret = _wMutex.lock();
    }
    __sync_fetch_and_add(&_readCount, 1);
    return ret;
}

bool mu::RWLock::rUnlock()
{
    bool ret = true;

    __sync_fetch_and_sub(&_readCount, 1);
    if(_readCount == 0)
    {
        ScopeLock lock(_rMutex);
        if(_readCount == 0)
            ret = _wMutex.unlock();
    }
    return ret;
}

bool mu::RWLock::wLock()
{
    return _wMutex.lock();
}

bool mu::RWLock::wUnlock()
{
    return _wMutex.unlock();
}

int mu::RWLock::getReaderCount() const
{
    return _readCount;
}

/*  Condition  */
mu::Condition::Condition()
{
    pthread_cond_init(&_cond, NULL);
}

mu::Condition::~Condition()
{
    pthread_cond_destroy(&_cond);
}

void mu::Condition::wait(Mutex &mutex)
{
    pthread_cond_wait(&_cond, mutex.getMutex());
}

bool mu::Condition::wait(Mutex &mutex, int second)
{
    struct timeval now;
    struct timespec ts;
    gettimeofday(&now, NULL);
    ts.tv_sec = now.tv_sec + second;
    ts.tv_nsec = now.tv_usec * 1000;
    return pthread_cond_timedwait(&_cond, mutex.getMutex(), &ts) == 0;
}

void mu::Condition::signal()
{
    pthread_cond_signal(&_cond);
}

void mu::Condition::broadcast()
{
    pthread_cond_broadcast(&_cond);
}