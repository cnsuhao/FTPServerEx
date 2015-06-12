#ifndef __H_MICROLOCK__
#define __H_MICROLOCK__

#include <pthread.h>
#include <sys/time.h>

namespace mu
{
    class Mutex;
    class ScopeLock;
    class RWLock;
    class Condition;

    class Mutex
    {
    public:
        explicit Mutex();
        ~Mutex();

    public:
        bool lock();
        bool unlock();
        pthread_mutex_t *getMutex();

    private:
        pthread_mutex_t _mutex;
    };


    class ScopeLock
    {
    public:
        explicit ScopeLock(Mutex &mutex);
        ~ScopeLock();

    private:
        Mutex &_mutex;
    };

    class RWLock
    {
    public:
        RWLock();
        ~RWLock();

    public:
        bool rLock();
        bool rUnlock();
        bool wLock();
        bool wUnlock();
        int getReaderCount() const;

    private:
        int _readCount;
        Mutex _rMutex, _wMutex;
    };

    class Condition
    {
    public:
        explicit Condition();
        ~Condition();

    public:
        void wait(Mutex &mutex);
        bool wait(Mutex &mutex, int second);
        void signal();
        void broadcast();

    private:
        pthread_cond_t _cond;
    };
}
#endif