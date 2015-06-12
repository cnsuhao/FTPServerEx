#include "ThreadPool.h"

/*  -- ThreadTask --  */
ThreadTask::ThreadTask()
    :func(NULL), arg(NULL)
{
    ;
}

ThreadTask::ThreadTask(pThreadFunc fc, void *args)
    :func(NULL), arg(NULL)
{
    func = fc;
    arg = args;
}

ThreadTask::ThreadTask(ThreadTask &task)
    :func(NULL), arg(NULL)
{
    func = task.func;
    arg = task.arg;
}

/*  -- TaskQueue --  */
TaskQueue::TaskQueue()
    :_size(0), _status(WORK), _head(NULL), _tail(NULL)
{
    _head = new ThreadTask();
    _tail = new ThreadTask();
    _head->next = _tail;
    _tail->next = NULL;
}

TaskQueue::~TaskQueue()
{
    clear();
    delete _head;
    delete _tail;
}

void TaskQueue::close()
{
    _status = CLOSE;
}

void TaskQueue::open()
{
    _status = WORK;
}

void TaskQueue::clear()
{
    ThreadTask *tmp = NULL;

    while(_head->next != _tail)
    {
        tmp = _head->next;
        delete _head;
        _head = tmp;
        tmp = tmp->next;
    }
    _size = 0;
}

UInt TaskQueue::size() const
{
    return _size;
}

bool TaskQueue::isEmpty()
{
    return size() == 0;
}

bool TaskQueue::isClose()
{
    return _status == CLOSE;
}

bool TaskQueue::putTask(ThreadTask &task)
{
    if(_status == CLOSE)
        return false;
    _tail->func = task.func;
    _tail->arg = task.arg;
    _tail->next = new ThreadTask();
    _tail = _tail->next;
    _tail->next = NULL;
    ++_size;
    return true;
}

bool TaskQueue::getTask(ThreadTask &task)
{
    ThreadTask *tmp = NULL;

    if(size() <= 0)
        return false;
    task.func = _head->next->func;
    task.arg = _head->next->arg;
    task.next = NULL;
    tmp = _head;
    _head = _head->next;
    delete tmp;
    --_size;
    return true;
}

/*  ThreadPool  */
ThreadPool::ThreadPool(int minThread, int maxThread, int maxQueue)
    :_minThread(minThread), 
     _maxThread(maxThread),
     _maxQueue(maxQueue), 
     _busyThreadCount(0),
     _queue(NULL),
     _waitToExitCount(0)
{
    _queue = new TaskQueue();
    _status = READY;
}

ThreadPool::~ThreadPool()
{
    delete _queue;
}

bool ThreadPool::addTask(ThreadTask &task)
{
    mu::ScopeLock lock(_mutex);
    while((signed)_queue->size() >= _maxQueue && (_status == WORK))
    	   _notFull.wait(_mutex);
    if(_status != WORK)
        return false;
    _queue->putTask(task);
    _notEmpty.signal();
    return true;
}

bool ThreadPool::startThreadPool()
{

    mu::ScopeLock lock(_mutex);
    if(_status == WORK)
        return false;
    _status = WORK;                          /* change pool status to work */
    _queue->open();                          /* open task queue, thread task can be addin */
    for(int i = 0; i < _minThread; ++i)      /* create worker thread */
        _createWorker();
    _manager = new ManagerThread(this);      /* create manager thread */
    _manager->start();
    return true;
}

void ThreadPool::shutdown()
{
    _status = SHUTDOWN;                            /* change pool status to shutdown */
    _queue->close();                                /* close queue to stop adding task */
    _manager->join();               /* wait manager thread to exit */
    delete _manager;
    //puts("manager exit");
    _notEmpty.broadcast();           /* wakeup all worker thread to finish all task and exit */
    for(std::set<Thread *>::iterator it = _workerSet.begin(); it != _workerSet.end(); ++it)
    {
        (*it)->join();                  /* wait all worker thread to exit */
        //printf("thread:%lu exit\n", (*it)->getId());
    }
    for(std::set<Thread *>::iterator it = _workerSet.begin(); it != _workerSet.end(); ++it)
        delete *it;	
    _workerSet.clear();                             /* clear worker id */
    _busyThreadCount = _waitToExitCount = 0;      /* change busy count and wait to exit count to zero */
}

void ThreadPool::_createWorker()
{
    Thread *thr = new WorkerThread(this);
    thr->start();
    _workerSet.insert(thr);
}

int ThreadPool::getBusyCount() const
{
    return _busyThreadCount;
}

/*  ManagerThread  */
ManagerThread::ManagerThread(ThreadPool *pool)
{
    _pool = pool;
}

ManagerThread::~ManagerThread()
{
    ;
}

void ManagerThread::run()
{
    while(1)
    {
        {
            sleep(Scheduling);              /* sleep */
            //puts("manager start, begin to get lock");
            mu::ScopeLock lock(_pool->_mutex);
            if(_pool->_status == ThreadPool::SHUTDOWN)    /* judge if pool status is shutdown */
                pthread_exit(0);
            //puts("manager got lock");
            if(_pool->_busyThreadCount < (signed)_pool->_workerSet.size() / 2 && _pool->_busyThreadCount < _pool->_minThread)
            {
                /* too many thread */
                _pool->_waitToExitCount = _pool->_workerSet.size() - _pool->_minThread;
                //printf("manager begin to cut thread, count:%d\n", _pool->_waitToExitCount);
                for(int i = _pool->_waitToExitCount; i > 0; --i)
                    _pool->_notEmpty.signal();
                //puts("thread cut!");
            }
            else if((signed)_pool->_queue->size() > _pool->_maxQueue / 2)
            {
                /* too many task */
                //printf("manager begin to create thread, count:%d, current:%d busy:%d\n", _pool->_maxQueue / 2, _pool->_workerSet.size(), _pool->_busyThreadCount);
                for(int i = _pool->_maxQueue / 2; i > 0 && _pool->_maxThread > (signed)_pool->_workerSet.size(); --i)
                    _pool->_createWorker();
                //puts("thread created!");
            }
        }
    }
}

/*  WorkerThread  */
WorkerThread::WorkerThread(ThreadPool *pool)
{
    _pool = pool;
}

WorkerThread::~WorkerThread()
{
    ;
}

void WorkerThread::run()
{
    ThreadTask task;
    std::set<Thread *>::iterator it;

    while(1)
    {
        {
            mu::ScopeLock lock(_pool->_mutex);
            while(_pool->_queue->size() <=0 && (_pool->_status == ThreadPool::WORK))
            {
                /* wait for task */
                _pool->_notEmpty.wait(_pool->_mutex);
                if(_pool->_waitToExitCount > 0)
                {
                    /* if wait to exit count great than 0, then kill self */
                    __sync_fetch_and_sub(&_pool->_waitToExitCount, 1);
                    it = _pool->_workerSet.find(this);
                    delete *it;
                    _pool->_workerSet.erase(it);
                    //sleep(3);
                    //printf("thread:%lu exit\n", pthread_self());
                    pthread_exit(0);
                }
            }
            /* don't end util finish all task */
            if(_pool->_status != ThreadPool::WORK && _pool->_queue->size() <= 0)
                pthread_exit(0);
            //printf("getTask:%d\n", _pool->_queue->getTask(task));
            _pool->_queue->getTask(task);
            _pool->_notFull.signal();
        }
        __sync_fetch_and_add(&_pool->_busyThreadCount, 1);
        task.func(task.arg);
        __sync_fetch_and_sub(&_pool->_busyThreadCount, 1);
    }
}
