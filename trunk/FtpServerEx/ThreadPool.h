#ifndef __H_THREADPOOL__
#define __H_THREADPOOL__

#include <pthread.h>
#include <set>
#include <unistd.h>
#include <stdio.h>
#include "Thread.h"
#include "Lock.h"

const int Scheduling = 20;

typedef unsigned long ULong;
typedef unsigned int UInt;
class ThreadTask;
class TaskQueue;
class WorkerThread;
class ManagerThread;
class ThreadPool;

class ThreadTask
{
public:
    ThreadTask();
    ThreadTask(pThreadFunc fc, void *args);
    ThreadTask(ThreadTask &task);
    pThreadFunc func;
    void *arg;
    ThreadTask *next;
};

class TaskQueue
{
public:
    enum QueueStatus {WORK, CLOSE};
    TaskQueue();
    ~TaskQueue();

public:
    void close();
    void open();
    UInt size() const;
    bool isEmpty();
    bool isClose();
    bool putTask(ThreadTask &task);
    bool getTask(ThreadTask &task);
    void clear();

private:
    UInt _size;
    QueueStatus _status;
    ThreadTask *_head;
    ThreadTask *_tail;
};

class WorkerThread : public Thread
{
public:
    WorkerThread(ThreadPool *pool);
    ~WorkerThread();

public:
    void run();

private:
    ThreadPool *_pool;
};

class ManagerThread : public Thread
{
public:
    ManagerThread(ThreadPool *pool);
    ~ManagerThread();

public:
    void run();

private:
    ThreadPool *_pool;
};

class ThreadPool : public NoCopyable
{
public:
    enum PoolStatus {READY, WORK, SHUTDOWN};
    friend class WorkerThread;
    friend class ManagerThread;
    ThreadPool(int minThread, int maxThread, int maxQueue);
    ~ThreadPool();

public:
    bool addTask(ThreadTask &task);
    bool startThreadPool();
    void shutdown();
    int getBusyCount() const;

private:
    void _createWorker();

private:
    int _minThread;                    /* min thread number */
    int _maxThread;                    /* max thread number */
    int _maxQueue;                     /* max thread task can in the queue */
    int _busyThreadCount;              /* count of thread which is working */
    int _waitToExitCount;              /* count of thread to exit */
    Thread *_manager;                  /* save manager id */
    std::set<Thread *> _workerSet;     /* save worker tid */
    mu::Mutex _mutex;
    mu::Condition _notEmpty, _notFull;
    TaskQueue *_queue;
    PoolStatus _status;                /* pool status */
};

#endif
