#ifndef __H_MICROTHREAD__
#define __H_MICROTHREAD__

#include <pthread.h>
#include "NoCopyable.h"
#include "Lock.h"

typedef pthread_t ThreadId;
typedef void *(*pThreadFunc)(void *);

class Thread : public NoCopyable
{
public:
    enum ThreadStatus {READY, WORK, SHUTDOWN};
    Thread();
    virtual ~Thread();
    friend void *_threadFunc(void *arg);

public:
    bool start();
    void join();
    bool isJoinable();
    bool setDetach(bool isDetach = true);
    virtual void run() = 0;
    ThreadId getId() const;
    ThreadStatus getThreadStatus() const;

    static ThreadId easyThread(pThreadFunc func, void *arg);

private:
    ThreadId _tid;
    ThreadStatus _status;
    bool _isDetach;
};

void *_threadFunc(void *arg);

#endif
