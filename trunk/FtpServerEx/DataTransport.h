#ifndef DATATRANSPORT_H
#define DATATRANSPORT_H

#include <stdio.h>
#include "CommonFtp.h"

typedef int SocketDescriptor;

const int LINELEN = 80;

class DataTransport
{
public:
    enum DataType {TYPE_NONE, TYPE_ASCII, TYPE_BINARY};
    enum DataMode {M_NONE, M_PASV, M_PORT};
    enum CreateRet {CREATE_OK, CREATE_FAIL, CREATE_TIMEOUT};
    enum TransRet {TRAN_OK, TRAN_FAIL, TRAN_ERROR, TRAN_CLOSE, TRAN_TIMEOUT};
    DataTransport(SocketDescriptor cmdSocket);
    ~DataTransport();

public:
    CreateRet createDataSocket(DataMode mode, const char *param = NULL);
    bool isDataSocketOpen();
    void setDataType(DataType type);
    void setDataMode(DataMode mode);
    void setRestPoint(unsigned long point);
    void clearRestPoint();
    DataType getDataType() const;
    DataMode getDataMode() const;
    SocketDescriptor getCmdSocket() const;
    SocketDescriptor getDataSocket() const;
    unsigned long getRestPoint() const;
    TransRet transport(SocketDescriptor src);
    TransRet getFile(SocketDescriptor src);
    void closeDataSocket();

private:
    CreateRet _createPasvDataSocket();
    CreateRet _createPortDataSocket(const char *param);
    TransRet _sendAscii(SocketDescriptor src);
    TransRet _sendBinary(SocketDescriptor src);

private:
    SocketDescriptor _cmdSocket;
    SocketDescriptor _dataSocket;
    DataMode _mode;
    DataType _type;
    unsigned long _restPoint;
};

#endif // DATATRANSPORT_H
