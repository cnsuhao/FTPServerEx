#ifndef __H_SERVECLIENT__
#define __H_SERVECLIENT__

#include <time.h>
#include "CommonFtp.h"
#include "CommonFile.h"
#include "UserInfo.h"
//#include "FtpRespString.h"
#include "VirtualPath.h"
#include "DataTransport.h"

typedef int SocketDescriptor;
typedef int FileDescriptor;

const int CLIENTMAXBUFLEN = 2048;
const int MAXPASSWORDLEN = 64;
extern const int MAXUSERNAMELEN;

const int READERR = -1;
const int WRITEERR = -2;
const int LOGINERR = -3;
const int CLIENTCLOSE = -4;

class ServeClient;

class ServeClient
{
public:
    enum RetType {SERVE_OK, SERVE_FAIL, SERVE_DENY, SERVE_TIMEOUT, SERVE_ABQUIT, SERVE_QUIT};
    enum DataType {ASCII, BINARY};
    enum DataMode {MODE_PASV, MODE_PORT};
    ServeClient(SocketDescriptor sd);
    virtual ~ServeClient();

public:
    virtual int serve();
    SocketDescriptor getSocketDescriptor() const;
    bool isLoggedIn();
    bool isDataSocketOpen() const;
    int sendWelcomeMsg();
    int sendByeMsg();

protected:
    SocketDescriptor __cmdSocket;
    bool __isLoggedIn;
    char __username[MAXUSERNAMELEN];
    UserInfo __ui;
    VirtualPath *__path;
    DataTransport *__data;

protected:
    virtual RetType __doABOR(SocketDescriptor fd, const char *param);
    virtual RetType __doACCT(SocketDescriptor fd, const char *param);
    virtual RetType __doADAT(SocketDescriptor fd, const char *param);
    virtual RetType __doALLO(SocketDescriptor fd, const char *param);
    virtual RetType __doAPPE(SocketDescriptor fd, const char *param);
    virtual RetType __doAUTH(SocketDescriptor fd, const char *param);
    virtual RetType __doCCC(SocketDescriptor fd, const char *param);
    virtual RetType __doCDUP(SocketDescriptor fd, const char *param);
    virtual RetType __doCONF(SocketDescriptor fd, const char *param);
    virtual RetType __doCWD(SocketDescriptor fd, const char *param);
    virtual RetType __doDELE(SocketDescriptor fd, const char *param);
    virtual RetType __doENC(SocketDescriptor fd, const char *param);
    virtual RetType __doEPRT(SocketDescriptor fd, const char *param);
    virtual RetType __doEPSV(SocketDescriptor fd, const char *param);
    virtual RetType __doFEAT(SocketDescriptor fd, const char *param);
    virtual RetType __doHELP(SocketDescriptor fd, const char *param);
    virtual RetType __doLANG(SocketDescriptor fd, const char *param);
    virtual RetType __doLIST(SocketDescriptor fd, const char *param);
    virtual RetType __doLPRT(SocketDescriptor fd, const char *param);
    virtual RetType __doLPSV(SocketDescriptor fd, const char *param);
    virtual RetType __doMDTM(SocketDescriptor fd, const char *param);
    virtual RetType __doMIC(SocketDescriptor fd, const char *param);
    virtual RetType __doMKD(SocketDescriptor fd, const char *param);
    virtual RetType __doMLSD(SocketDescriptor fd, const char *param);
    virtual RetType __doMLST(SocketDescriptor fd, const char *param);
    virtual RetType __doMODE(SocketDescriptor fd, const char *param);
    virtual RetType __doNLST(SocketDescriptor fd, const char *param);
    virtual RetType __doNOOP(SocketDescriptor fd, const char *param);
    virtual RetType __doOPTS(SocketDescriptor fd, const char *param);
    virtual RetType __doPASS(SocketDescriptor fd, const char *param);
    virtual RetType __doPASV(SocketDescriptor fd, const char *param);
    virtual RetType __doPBSZ(SocketDescriptor fd, const char *param);
    virtual RetType __doPORT(SocketDescriptor fd, const char *param);
    virtual RetType __doPROT(SocketDescriptor fd, const char *param);
    virtual RetType __doPWD(SocketDescriptor fd, const char *param);
    virtual RetType __doQUIT(SocketDescriptor fd, const char *param);
    virtual RetType __doREIN(SocketDescriptor fd, const char *param);
    virtual RetType __doREST(SocketDescriptor fd, const char *param);
    virtual RetType __doRETR(SocketDescriptor fd, const char *param);
    virtual RetType __doRMD(SocketDescriptor fd, const char *param);
    virtual RetType __doRNFR(SocketDescriptor fd, const char *param);
    virtual RetType __doRNTO(SocketDescriptor fd, const char *param);
    virtual RetType __doSITE(SocketDescriptor fd, const char *param);
    virtual RetType __doSIZE(SocketDescriptor fd, const char *param);
    virtual RetType __doSMNT(SocketDescriptor fd, const char *param);
    virtual RetType __doSTAT(SocketDescriptor fd, const char *param);
    virtual RetType __doSTOR(SocketDescriptor fd, const char *param);
    virtual RetType __doSTOU(SocketDescriptor fd, const char *param);
    virtual RetType __doSTRU(SocketDescriptor fd, const char *param);
    virtual RetType __doSYST(SocketDescriptor fd, const char *param);
    virtual RetType __doTYPE(SocketDescriptor fd, const char *param);
    virtual RetType __doUSER(SocketDescriptor fd, const char *param);
    virtual RetType __doXCUP(SocketDescriptor fd, const char *param);
    virtual RetType __doXMKD(SocketDescriptor fd, const char *param);
    virtual RetType __doXPWD(SocketDescriptor fd, const char *param);
    virtual RetType __doXRCP(SocketDescriptor fd, const char *param);
    virtual RetType __doXRMD(SocketDescriptor fd, const char *param);
    virtual RetType __doXRSQ(SocketDescriptor fd, const char *param);
    virtual RetType __doXSEM(SocketDescriptor fd, const char *param);
    virtual RetType __doXSEN(SocketDescriptor fd, const char *param);
    virtual RetType __doUNKNOW(SocketDescriptor fd, const char *param);
    virtual RetType __doPortFile(SocketDescriptor fd, const char *param);
};

#endif // SERVECLIENT_H
