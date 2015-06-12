#include "FtpServer.h"

FtpServer::FtpServer(const char *address, int port)
    :_port(port), _socketFd(-1), _pool(NULL)
{
    strncpy(_serverAddr, address, sizeof(address));
    __initFtpServer();
}

FtpServer::~FtpServer()
{
    delete _pool;
}

int FtpServer::run()
{
    int maxQueue, client;
    struct sockaddr_in sa;
    socklen_t len;
    ThreadTask task;

    if(IniReader::readInt("System", "MaxQueue", maxQueue, SETTINGFILE) != 0)
        maxQueue = 100;
    if((_socketFd = CommonSocket::createServerSocket(getAddress(), getPort(), SOCK_STREAM, maxQueue)) < 0)
        return _socketFd;
    task.func = _serveClient;
    _pool->startThreadPool();
    while(1)
    {
        len = sizeof(struct sockaddr_in);
        client = accept(_socketFd, (struct sockaddr *)&sa, &len);
        printf("Got connection from ip:%s, port:%d\n", inet_ntoa(sa.sin_addr), ntohs(sa.sin_port));
        task.arg = (void *)client;
        _pool->addTask(task);
    }
}

const char *FtpServer::getAddress() const
{
    return _serverAddr;
}

int FtpServer::getPort() const
{
    return _port;
}

int FtpServer::getUserCount() const
{
    return _pool->getBusyCount();
}

void FtpServer::__initFtpServer()
{
    int minThreadCount = 10, maxThreadCount = 100, maxQueue = 40;
    IniReader::readInt("System", "MinThread", minThreadCount, SETTINGFILE);
    IniReader::readInt("System", "MaxThread", maxThreadCount, SETTINGFILE);
    IniReader::readInt("System", "MaxQueue", maxQueue, SETTINGFILE);
    _pool = new ThreadPool(minThreadCount, maxThreadCount, maxQueue);
}

void *_serveClient(void *arg)
{
    int timeout = -1;
    long client = long (arg);

    IniReader::readInt("System", "Timeout", timeout, SETTINGFILE);
    if(timeout != -1)
        CommonSocket::setTimeout(client, timeout);
    ServeClient sc(client);
    sc.serve();
    close(client);
    return (char *)0;
}
