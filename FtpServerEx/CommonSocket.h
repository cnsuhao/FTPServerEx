#ifndef __H_COMMONSOCKET__
#define __H_COMMONSOCKET__

#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <fcntl.h>
#include <memory.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

typedef int SocketDescriptor;

class CommonSocket
{
public:
    enum SocketRetType {
        SUCCESS = 0, CREATESOCKETERR = -1, SETSOCKOPTERR = -2, BINDADDRERR = -3, LISTENADDRERR = -4,
        INVALIDADDR = -5, SETFLAGFAIL = -6, IPADDRERR = -7, CONNECTERR = -8
    };
    static SocketDescriptor createServerSocket(const char *addr, int port, int sockType = SOCK_STREAM, int queue = 100);
    static int setTimeout(SocketDescriptor socketFd, int timeout = 0);
    static int setSocketFlag(SocketDescriptor fd, int flag);
    static int clearSocketFlag(SocketDescriptor fd, int flag);
    static int setNonBlocking(SocketDescriptor socketFd);
    static int readLine(SocketDescriptor socketFd, char *buf, int size, char endLineStr = '\n');
    static int read(SocketDescriptor socketFd, char *buf, unsigned long size, int timeout);
};

#endif
