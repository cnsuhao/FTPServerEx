#ifndef __H_FTPSERVER__
#define __H_FTPSERVER__

#include "ThreadPool.h"
#include "ServeClient.h"

const int IPV4LEN = 17;
extern const int MAXBUFLEN;
const char *const SETTINGFILE = "/home/wenjie/FtpServerEx/Setting.ini";

void *_serveClient(void *arg);

class FtpServer
{
public:
    FtpServer(const char *address = "0.0.0.0", int port = 8000);
    ~FtpServer();

public:
    int run();
    const char *getAddress() const;
    int getPort() const;
    int getUserCount() const;

private:
    char _serverAddr[IPV4LEN];
    int _port;
    int _socketFd;
    ThreadPool *_pool;

protected:
    void __initFtpServer();
};

#endif
