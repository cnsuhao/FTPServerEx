#include "DataTransport.h"

const int IPV4LEN = 17;

DataTransport::DataTransport(SocketDescriptor cmdSocket)
    :_type(TYPE_BINARY),
     _mode(M_PASV),
     _restPoint(0)
{
    _cmdSocket = cmdSocket;
}

DataTransport::~DataTransport()
{
    if(_dataSocket > 0)
        close(_dataSocket);
}

DataTransport::CreateRet DataTransport::createDataSocket(DataMode mode, const char *param)
{
    switch(mode)
    {
    case M_PASV:
        return _createPasvDataSocket();
        break;
    case M_PORT:
        return _createPortDataSocket(param);
        break;
    default:
        break;
    }
    return CREATE_FAIL;
}

bool DataTransport::isDataSocketOpen()
{
    return _dataSocket > 0;
}

void DataTransport::setDataType(DataType type)
{
    _type = type;
}

void DataTransport::setDataMode(DataMode mode)
{
    _mode = mode;
}

void DataTransport::setRestPoint(unsigned long point)
{
    _restPoint = point;
}

void DataTransport::clearRestPoint()
{
    _restPoint = 0;
}

DataTransport::DataType DataTransport::getDataType() const
{
    return _type;
}

DataTransport::DataMode DataTransport::getDataMode() const
{
    return _mode;
}

SocketDescriptor DataTransport::getCmdSocket() const
{
    return _cmdSocket;
}

SocketDescriptor DataTransport::getDataSocket() const
{
    return _dataSocket;
}

unsigned long DataTransport::getRestPoint() const
{
    return _restPoint;
}

DataTransport::TransRet DataTransport::transport(SocketDescriptor src)
{
    lseek(src, getRestPoint(), SEEK_CUR);
    switch(_type)
    {
    case TYPE_ASCII:
        return _sendAscii(src);
        break;
    case TYPE_BINARY:
        return _sendBinary(src);
        break;
    default:
        break;
    }
    return TRAN_FAIL;
}

DataTransport::TransRet DataTransport::getFile(SocketDescriptor src)
{
    int nRead;
    char buf[LINELEN];
    while(1)
    {
        nRead = recv(getDataSocket(), buf, sizeof(buf),0);
        if(nRead < 0)
            return TRAN_ERROR;
        else if(nRead == 0)
            return TRAN_OK;
        else
            write(src , buf, sizeof(buf));
    }
    return TRAN_OK;
}

DataTransport::TransRet DataTransport::_sendAscii(SocketDescriptor src)
{
    int nRead;
    char buf[LINELEN];
    return _sendBinary(src);
    return TRAN_OK;
}

DataTransport::TransRet DataTransport::_sendBinary(SocketDescriptor src)
{
    int nRead;
    char buf[LINELEN];
    while(1)
    {
        nRead = read(src, buf, sizeof(buf));
        if(nRead < 0)
            return TRAN_ERROR;
        else if(nRead == 0)
            return TRAN_CLOSE;
        send(getDataSocket(), buf, nRead, 0);
    }
    return TRAN_OK;
}

void DataTransport::closeDataSocket()
{
    if(_dataSocket < 0)
        return;
    close(_dataSocket);
    _dataSocket = -1;
}

DataTransport::CreateRet DataTransport::_createPasvDataSocket()
{
    SocketDescriptor server;
    struct sockaddr_in sa;
    socklen_t len;
    int port;
    char ip[IPV4LEN], buf[128];

    /* get main ftpserer ip address */
    len = sizeof(sa);
    getsockname(_cmdSocket, (struct sockaddr *)&sa, &len);
    inet_ntop(AF_INET, &sa.sin_addr, ip, sizeof(ip));
    for(char *p = ip; *p; ++p)
        if(*p == '.')
            *p = ',';

    if((server = CommonSocket::createServerSocket("0.0.0.0", 0, SOCK_STREAM, 1)) < 0)
        return CREATE_FAIL;
    CommonSocket::setTimeout(server, 20);

    /* get server port */
    len = sizeof(sa);
    getsockname(server, (struct sockaddr *)&sa, &len);
    port = ntohs(sa.sin_port);

    sprintf(buf, RESP_227EX, ip, port / 256, port % 256);
    CommonFtp::sendResp(_cmdSocket, buf);
    _dataSocket = accept(server, NULL, NULL);
    close(server);
    if(_dataSocket < 0)
        return CREATE_TIMEOUT;
    return CREATE_OK;
}

DataTransport::CreateRet DataTransport::_createPortDataSocket(const char *param)
{
    int tmp[6], port; /* for save ip and port */
    char ipBuf[64];
    SocketDescriptor client;
    struct sockaddr_in sa;

    /* make ip and port */
    if(sscanf(param, "%d,%d,%d,%d,%d,%d", &tmp[0], &tmp[1], &tmp[2], &tmp[3], &tmp[4], &tmp[5]) != 6)
        return CREATE_FAIL;
    if(sprintf(ipBuf, "%d.%d.%d.%d", tmp[0], tmp[1], tmp[2], tmp[3]) > 16)
        return CREATE_FAIL;
    if(!inet_pton(AF_INET, ipBuf, &sa.sin_addr))
        return CREATE_FAIL;
    port = tmp[4] * 256 + tmp[5];
    sa.sin_port = htons(port);
    sa.sin_family = AF_INET;

    if((client = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return CREATE_FAIL;
    if(connect(client, (struct sockaddr *)&sa, (socklen_t)sizeof(sa)) < 0)
    {
        close(client);
        return CREATE_FAIL;
    }
    _dataSocket = client;
    return CREATE_OK;
}
