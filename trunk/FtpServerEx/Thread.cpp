#include "Thread.h"

Thread::Thread()
    :_tid(0), _status(READY), _isDetach(false)
{
}

Thread::~Thread()
{
	;
}

bool Thread::start()
{
    if(getThreadStatus() == WORK)
        return false;
    _status = WORK;
    if(_isDetach == true)
    {
        pthread_attr_t attr;

        pthread_attr_init(&attr);
        if(pthread_attr_init(&attr) != 0 || pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0)
            return false;
        return pthread_create(&_tid, &attr, _threadFunc, (void *)this) == 0 && pthread_attr_destroy(&attr) == 0;
    }
    return pthread_create(&_tid, NULL, _threadFunc, (void *)this) == 0;
}

ThreadId Thread::getId() const
{
    return _tid;
}

Thread::ThreadStatus Thread::getThreadStatus() const
{
    return _status;
}

bool Thread::setDetach(bool isDetach)
{
    /* can not set detach when thread is running */
    if(_status == WORK)
        return false;
    _isDetach = isDetach;
    return true;
}

void Thread::join()
{
    /* wait util thread exit */
    if(_isDetach)
        return;
    pthread_join(_tid, NULL);
    _status = SHUTDOWN;
}

bool Thread::isJoinable()
{
    return !_isDetach;
}

void *_threadFunc(void *arg)
{
    Thread *thread = static_cast<Thread *>(arg);
    thread->run();
    pthread_exit(0);
}

ThreadId Thread::easyThread(pThreadFunc func, void *arg)
{
    ThreadId id;

    if(pthread_create(&id, NULL, func, arg) < 0)
        return 0;
    return id;
}
