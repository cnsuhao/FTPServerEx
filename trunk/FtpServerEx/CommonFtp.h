#ifndef __H_COMMONFTP__
#define __H_COMMONFTP__

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <map>
#include <set>
#include <string>
#include "Lock.h"
#include "IniReader.h"
#include "CommonSocket.h"
#include "FtpRespString.h"

const int CMDBUFLEN = 10;

class CmdInfo;
class CommonFtp;

class CommonFtp
{
public:
    enum CmdType {
         CMD_ABOR, CMD_ACCT, CMD_ADAT, CMD_ALLO, CMD_APPE, CMD_AUTH, CMD_CCC, CMD_CDUP, CMD_CONF,
         CMD_CWD, CMD_DELE, CMD_ENC, CMD_EPRT, CMD_EPSV, CMD_FEAT, CMD_HELP, CMD_LANG, CMD_LIST, CMD_LPRT,
         CMD_LPSV, CMD_MDTM, CMD_MIC, CMD_MKD, CMD_MLSD, CMD_MLST, CMD_MODE, CMD_NLST, CMD_NOOP, CMD_OPTS,
         CMD_PASS, CMD_PASV, CMD_PBSZ, CMD_PORT, CMD_PROT, CMD_PWD, CMD_QUIT, CMD_REIN, CMD_REST, CMD_RETR, CMD_RMD,
         CMD_RNFR, CMD_RNTO, CMD_SITE, CMD_SIZE, CMD_SMNT, CMD_STAT, CMD_STOR, CMD_STOU, CMD_STRU, CMD_SYST,
         CMD_TYPE, CMD_USER, CMD_XCUP, CMD_XMKD, CMD_XPWD, CMD_XRCP, CMD_XRMD, CMD_XRSQ, CMD_XSEM, CMD_XSEN, CMD_MCMD,
         CMD_UNKNOW
    };
    static int sendResp(int socketFd, const char *msg);
    static int sendResp(int socketFd, const char *msg, const char *param1);
    static int sendResp(int socketFd, const char *msg, const char *param1, const char *param2);
    static int sendResp(int socketFd, const char *msg, int param1, int param2, int param3, int param4, int param5, int param6);
    static const CmdInfo *parseCmd(const char *raw, char *param, int size);

private:
    static void _fillMap();
    static int _getCmdValue(const char *raw, char *valueBuf, int bufSize);
    static int _getCmdType(const char *raw, char *cmdBuf, int bufSize);

private:
    static std::map<std::string, const CmdInfo *> _cmdMap;
    static mu::Mutex _mutex;
};

class CmdInfo
{
public:
    CmdInfo(const char *str, CommonFtp::CmdType code, bool isNeedParam = true, bool isNeedLogin = true);
    ~CmdInfo();

public:
    const char *getStrCmd() const;
    CommonFtp::CmdType getCmdCode() const;
    bool isNeedParam() const;
    bool isNeedLogin() const;

private:
    const char *_str;
    CommonFtp::CmdType _cmdCode;
    bool _isNeedParam;
    bool _isNeedLogin;
};

const CmdInfo cmdUnknow = {"UNKNOW", CommonFtp::CMD_UNKNOW};
const CmdInfo cmdList[] = {
    {"ABOR", CommonFtp::CMD_ABOR},
    {"ACCT", CommonFtp::CMD_ACCT},
    {"ADAT", CommonFtp::CMD_ADAT},
    {"ALLO", CommonFtp::CMD_ALLO},
    {"APPE", CommonFtp::CMD_APPE},
    {"AUTH", CommonFtp::CMD_AUTH},
    {"CCC", CommonFtp::CMD_CCC},
    {"CDUP", CommonFtp::CMD_CDUP},
    {"CONF", CommonFtp::CMD_CONF},
    {"CWD", CommonFtp::CMD_CWD},
    {"DELE", CommonFtp::CMD_DELE},
    {"ENC", CommonFtp::CMD_ENC},
    {"EPRT", CommonFtp::CMD_EPRT},
    {"EPSV", CommonFtp::CMD_EPSV},
    {"FEAT", CommonFtp::CMD_FEAT},
    {"HELP", CommonFtp::CMD_HELP, false, false},
    {"LANG", CommonFtp::CMD_LANG},
    {"LIST", CommonFtp::CMD_LIST, false},
    {"LPRT", CommonFtp::CMD_LPRT},
    {"LPSV", CommonFtp::CMD_LPSV},
    {"MDTM", CommonFtp::CMD_MDTM},
    {"MIC", CommonFtp::CMD_MIC},
    {"MKD", CommonFtp::CMD_MKD},
    {"MLSD", CommonFtp::CMD_MLSD},
    {"MLST", CommonFtp::CMD_MLST},
    {"MODE", CommonFtp::CMD_MODE},
    {"NLST", CommonFtp::CMD_NLST},
    {"NOOP", CommonFtp::CMD_NOOP},
    {"OPTS", CommonFtp::CMD_OPTS},
    {"PASS", CommonFtp::CMD_PASS, true, false},
    {"PASV", CommonFtp::CMD_PASV},
    {"PBSZ", CommonFtp::CMD_PBSZ},
    {"PORT", CommonFtp::CMD_PORT},
    {"PROT", CommonFtp::CMD_PROT},
    {"PWD", CommonFtp::CMD_PWD},
    {"QUIT", CommonFtp::CMD_QUIT},
    {"REIN", CommonFtp::CMD_REIN},
    {"REST", CommonFtp::CMD_REST},
    {"RETR", CommonFtp::CMD_RETR},
    {"RMD", CommonFtp::CMD_RMD},
    {"RNFR", CommonFtp::CMD_RNFR},
    {"RNTO", CommonFtp::CMD_RNTO},
    {"SITE", CommonFtp::CMD_SITE},
    {"SIZE", CommonFtp::CMD_SIZE},
    {"SMNT", CommonFtp::CMD_SMNT},
    {"STAT", CommonFtp::CMD_STAT},
    {"STOR", CommonFtp::CMD_STOR},
    {"STOU", CommonFtp::CMD_STOU},
    {"STRU", CommonFtp::CMD_STRU},
    {"SYST", CommonFtp::CMD_SYST},
    {"TYPE", CommonFtp::CMD_TYPE},
    {"USER", CommonFtp::CMD_USER, true, false},
    {"XCUP", CommonFtp::CMD_XCUP},
    {"XMKD", CommonFtp::CMD_XMKD},
    {"XPWD", CommonFtp::CMD_XPWD},
    {"XRCP", CommonFtp::CMD_XRCP},
    {"XRMD", CommonFtp::CMD_XRMD},
    {"XRSQ", CommonFtp::CMD_XRSQ},
    {"XSEM", CommonFtp::CMD_XSEM},
    {"XSEN", CommonFtp::CMD_XSEN},
    {"MCMD", CommonFtp::CMD_MCMD,true, false}
};

#endif
