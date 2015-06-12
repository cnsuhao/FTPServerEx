#include "CommonFtp.h"

std::map<std::string, const CmdInfo *> CommonFtp::_cmdMap;
mu::Mutex CommonFtp::_mutex;

CmdInfo::CmdInfo(const char *str, CommonFtp::CmdType code, bool isNeedParam, bool isNeedLogin)
    :_str(str), _cmdCode(code), _isNeedParam(isNeedParam), _isNeedLogin(isNeedLogin)
{
    ;
}

CmdInfo::~CmdInfo()
{
    ;
}

const char *CmdInfo::getStrCmd() const
{
    return _str;
}

CommonFtp::CmdType CmdInfo::getCmdCode() const
{
    return _cmdCode;
}

bool CmdInfo::isNeedParam() const
{
    return _isNeedParam;
}

bool CmdInfo::isNeedLogin() const
{
    return _isNeedLogin;
}

int CommonFtp::sendResp(int socketFd, const char *msg)
{
    return write(socketFd, msg, strlen(msg));
}

int CommonFtp::sendResp(int socketFd, const char *msg, const char *param1)
{
    char buf[1024];
    sprintf(buf, msg, param1);
    return sendResp(socketFd, buf);
}

int CommonFtp::sendResp(int socketFd, const char *msg, const char *param1, const char *param2)
{
    char buf[1024];
    sprintf(buf, msg, param1, param2);
    return sendResp(socketFd, buf);
}

int CommonFtp::sendResp(int socketFd, const char *msg, int param1, int param2, int param3, int param4, int param5, int param6)
{
    char buf[1024];
    sprintf(buf, msg, param1, param2, param3, param4, param5, param6);
    return sendResp(socketFd, buf);
}

const CmdInfo *CommonFtp::parseCmd(const char *raw, char *param, int size)
{
    char cmdBuf[CMDBUFLEN];
    std::map<std::string, const CmdInfo *>::iterator it;

    if(_cmdMap.size() == 0)
    {
        mu::ScopeLock lock(_mutex);
        if(_cmdMap.size() == 0)
            _fillMap();
    }
    _getCmdType(raw, cmdBuf, sizeof(cmdBuf));
    _getCmdValue(raw, param, size);
    if((it = _cmdMap.find(cmdBuf)) == _cmdMap.end())
        return &cmdUnknow;
    return it->second;
}

void CommonFtp::_fillMap()
{
    printf("%d" ,(signed)(sizeof(cmdList) );
    for(int i = 0; i < (signed)(sizeof(cmdList) / sizeof(cmdList[0])); ++i)
        _cmdMap[cmdList[i].getStrCmd()] = &cmdList[i];
}

int CommonFtp::_getCmdValue(const char *raw, char *valueBuf, int bufSize)
{
    const char *ptr = NULL;
    char *tmptr = NULL;

    if((ptr = strchr(raw, ' ')) == NULL)
    {
        valueBuf[0] = '\0';  /* no space means that this cmd has no param */
        return 0;
    }
    while(*ptr && *ptr == ' ')
        ++ptr;
    strncpy(valueBuf, ptr, bufSize - 1);
    valueBuf[bufSize - 1] = '\0';
    tmptr = valueBuf;
    while(*tmptr && *tmptr != ' ' && *tmptr != '\r' && *tmptr != '\n')
        ++tmptr;
    *tmptr = '\0';
    return tmptr - valueBuf;
}

int CommonFtp::_getCmdType(const char *raw, char *cmdBuf, int bufSize)
{
    int len;

    --bufSize;
    for(len = 0; raw[len] != ' ' && raw[len] != '\r' && raw[len] != '\n' && len < bufSize; ++len)
        cmdBuf[len] = raw[len];
    cmdBuf[len] = '\0';
    return len;
}

