#include "CommonSocket.h"

SocketDescriptor CommonSocket::createServerSocket(const char *addr, int port, int sockType, int queue)
{
    struct sockaddr_in sa;
    SocketDescriptor socketFd, reuse = 1;
    
    if((socketFd = socket(AF_INET, sockType, 0)) < 0)
        return CREATESOCKETERR;
    
    if(-1 == setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)))
        return SETSOCKOPTERR;
    
    bzero(&sa, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    inet_aton(addr, &sa.sin_addr);
    
    if(bind(socketFd, (struct sockaddr *)&sa, sizeof(sa)) < 0)
        return BINDADDRERR;
    
    if(sockType == SOCK_STREAM && -1 == listen(socketFd, queue))
        return LISTENADDRERR;
    return socketFd;
}

int CommonSocket::setTimeout(SocketDescriptor socketFd, int timeout)
{
	struct timeval tv;
	tv.tv_sec = timeout;
	tv.tv_usec = 0;
	if(socketFd > 0 && timeout > 0)
		return setsockopt(socketFd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	return 0;
}

int CommonSocket::setSocketFlag(SocketDescriptor fd, int flag)
{
    int oflag;
    if((oflag = fcntl(fd, F_GETFL, 0)) < 0)
        return SETFLAGFAIL;
    if(fcntl(fd, F_SETFL, oflag | flag) < 0)
        return SETFLAGFAIL;
    return 0;
}

int CommonSocket::clearSocketFlag(SocketDescriptor fd, int flag)
{
    int oflag;
    if((oflag = fcntl(fd, F_GETFL, 0)) < 0)
        return SETFLAGFAIL;
    if(fcntl(fd, F_SETFL, oflag & ~flag) < 0)
        return SETFLAGFAIL;
    return 0;
}

int CommonSocket::setNonBlocking(SocketDescriptor socketFd)
{
    return setSocketFlag(socketFd, O_NONBLOCK);
}

int CommonSocket::readLine(SocketDescriptor socketFd, char *buf, int size, char endLineStr)
{
	char ch;
	int nRead, count = 0;
	while(count < size - 1)
	{
		if((nRead = ::read(socketFd, &ch, 1) == 1))
		{
			buf[count++] = ch;
            if(ch == endLineStr)
				break;
		}
		else if(nRead == -1)
		{
			if(errno == EINTR)
				continue;
			return -1;
		}
		else
			break;
	}
	buf[count] = '\0';
	return count;
}

int CommonSocket::read(SocketDescriptor socketFd, char *buf, unsigned long size, int timeout)
{
	struct timeval tv;
	fd_set rSet;
	int retVal;

	if(timeout > 0)
	{
		tv.tv_sec = timeout;
		tv.tv_usec = 0;

		FD_ZERO(&rSet);
		FD_SET(socketFd, &rSet);

		retVal = select(socketFd + 1, &rSet, NULL, NULL, &tv);
		if(retVal == -1)
			return -1;
		else if(retVal == 0)
			return 0;
	}
	return ::read(socketFd, buf, size);
}

