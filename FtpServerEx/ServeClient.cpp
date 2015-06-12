#include "ServeClient.h"
#include "FtpServer.h"

ServeClient::ServeClient(SocketDescriptor sd)
    :__cmdSocket(sd),
     __isLoggedIn(false),
     __path(NULL),
     __data(new DataTransport(sd))
{
    __username[0] = '\0';
}

ServeClient::~ServeClient()
{
    delete __path;
}

SocketDescriptor ServeClient::getSocketDescriptor() const
{
    return __cmdSocket;
}

bool ServeClient::isLoggedIn()
{
    return __isLoggedIn;
}

int ServeClient::sendWelcomeMsg()
{
    char buf[MAXBUFLEN];

    IniReader::read("System", "WelcomeMsg", buf, sizeof(buf), "Welcome", SETTINGFILE);
    CommonFtp::sendResp(__cmdSocket, RESP_220EX, buf);
    return 0;
}

int ServeClient::sendByeMsg()
{
    char buf[MAXBUFLEN];

    IniReader::read("System", "ByeMsg", buf, sizeof(buf), "Bye~", SETTINGFILE);
    if(CommonFtp::sendResp(__cmdSocket, buf) < 0)
        return WRITEERR;
    return 0;
}

int ServeClient::serve()
{
    char buf[CLIENTMAXBUFLEN], param[CLIENTMAXBUFLEN];
    int nRead = 0;
    const CmdInfo *cmd = NULL;
    RetType retVal = SERVE_OK;

    sendWelcomeMsg();
    while(1)
    {
        if((nRead = CommonSocket::readLine(__cmdSocket, buf, sizeof(buf))) < 0)
            return READERR;
        else if(nRead == 0)
            return CLIENTCLOSE;
        cmd = CommonFtp::parseCmd(buf, param, sizeof(param));
        if(isLoggedIn() == false && cmd->isNeedLogin())
        {
            CommonFtp::sendResp(__cmdSocket, RESP_530);
            continue;
        }
        switch(cmd->getCmdCode())
        {
        case CommonFtp::CMD_ABOR:
            retVal = __doABOR(__cmdSocket, param);
            break;
        case CommonFtp::CMD_ADAT:
            retVal = __doADAT(__cmdSocket, param);
            break;
        case CommonFtp::CMD_ALLO:
            retVal = __doALLO(__cmdSocket, param);
            break;
        case CommonFtp::CMD_APPE:
            retVal = __doAPPE(__cmdSocket, param);
            break;
        case CommonFtp::CMD_AUTH:
            retVal = __doAUTH(__cmdSocket, param);
            break;
        case CommonFtp::CMD_CCC:
            retVal = __doCCC(__cmdSocket, param);
            break;
        case CommonFtp::CMD_CDUP:
            retVal = __doCDUP(__cmdSocket, param);
            break;
        case CommonFtp::CMD_CONF:
            retVal = __doCONF(__cmdSocket, param);
            break;
        case CommonFtp::CMD_CWD:
            retVal = __doCWD(__cmdSocket, param);
            break;
        case CommonFtp::CMD_ACCT:
            retVal = __doACCT(__cmdSocket, param);
            break;
        case CommonFtp::CMD_DELE:
            retVal = __doDELE(__cmdSocket, param);
            break;
        case CommonFtp::CMD_ENC:
            retVal = __doENC(__cmdSocket, param);
            break;
        case CommonFtp::CMD_EPRT:
            retVal = __doEPRT(__cmdSocket, param);
            break;
        case CommonFtp::CMD_EPSV:
            retVal = __doEPSV(__cmdSocket, param);
            break;
        case CommonFtp::CMD_FEAT:
            retVal = __doFEAT(__cmdSocket, param);
            break;
        case CommonFtp::CMD_HELP:
            retVal = __doHELP(__cmdSocket, param);
            break;
        case CommonFtp::CMD_LANG:
            retVal = __doLANG(__cmdSocket, param);
            break;
        case CommonFtp::CMD_LIST:
            retVal = __doLIST(__cmdSocket, param);
            break;
        case CommonFtp::CMD_LPRT:
            retVal = __doLPRT(__cmdSocket, param);
            break;
        case CommonFtp::CMD_LPSV:
            retVal = __doLPSV(__cmdSocket, param);
            break;
        case CommonFtp::CMD_MDTM:
            retVal = __doMDTM(__cmdSocket, param);
            break;
        case CommonFtp::CMD_MIC:
            retVal = __doMIC(__cmdSocket, param);
            break;
        case CommonFtp::CMD_MKD:
            retVal = __doMKD(__cmdSocket, param);
            break;
        case CommonFtp::CMD_MLSD:
            retVal = __doMLSD(__cmdSocket, param);
            break;
        case CommonFtp::CMD_MLST:
            retVal = __doMLST(__cmdSocket, param);
            break;
        case CommonFtp::CMD_MODE:
            retVal = __doMODE(__cmdSocket, param);
            break;
        case CommonFtp::CMD_NLST:
            retVal = __doNLST(__cmdSocket, param);
            break;
        case CommonFtp::CMD_NOOP:
            retVal = __doNOOP(__cmdSocket, param);
            break;
        case CommonFtp::CMD_OPTS:
            retVal = __doOPTS(__cmdSocket, param);
            break;
        case CommonFtp::CMD_PASV:
            retVal = __doPASV(__cmdSocket, param);
            break;
        case CommonFtp::CMD_PBSZ:
            retVal = __doPBSZ(__cmdSocket, param);
            break;
        case CommonFtp::CMD_PORT:
            retVal = __doPORT(__cmdSocket, param);
            break;
        case CommonFtp::CMD_PROT:
            retVal = __doPROT(__cmdSocket, param);
            break;
        case CommonFtp::CMD_PWD:
            retVal = __doPWD(__cmdSocket, param);
            break;
        case CommonFtp::CMD_QUIT:
            retVal = __doQUIT(__cmdSocket, param);
            break;
        case CommonFtp::CMD_REIN:
            retVal = __doREIN(__cmdSocket, param);
            break;
        case CommonFtp::CMD_REST:
            retVal = __doREST(__cmdSocket, param);
            break;
        case CommonFtp::CMD_RETR:
            retVal = __doRETR(__cmdSocket, param);
            break;
        case CommonFtp::CMD_RMD:
            retVal = __doRMD(__cmdSocket, param);
            break;
        case CommonFtp::CMD_RNFR:
            retVal = __doRNFR(__cmdSocket, param);
            break;
        case CommonFtp::CMD_RNTO:
            retVal = __doRNTO(__cmdSocket, param);
            break;
        case CommonFtp::CMD_SITE:
            retVal = __doSITE(__cmdSocket, param);
            break;
        case CommonFtp::CMD_SIZE:
            retVal = __doSIZE(__cmdSocket, param);
            break;
        case CommonFtp::CMD_SMNT:
            retVal = __doSMNT(__cmdSocket, param);
            break;
        case CommonFtp::CMD_STAT:
            retVal = __doSTAT(__cmdSocket, param);
            break;
        case CommonFtp::CMD_STOR:
            retVal = __doSTOR(__cmdSocket, param);
            break;
        case CommonFtp::CMD_STOU:
            retVal = __doSTOU(__cmdSocket, param);
            break;
        case CommonFtp::CMD_STRU:
            retVal = __doSTRU(__cmdSocket, param);
            break;
        case CommonFtp::CMD_SYST:
            retVal = __doSYST(__cmdSocket, param);
            break;
        case CommonFtp::CMD_TYPE:
            retVal = __doTYPE(__cmdSocket, param);
            break;
        case CommonFtp::CMD_USER:
            retVal = __doUSER(__cmdSocket, param);
            break;
        case CommonFtp::CMD_PASS:
            retVal = __doPASS(__cmdSocket, param);
            break;
        case CommonFtp::CMD_XCUP:
            retVal = __doXCUP(__cmdSocket, param);
            break;
        case CommonFtp::CMD_XMKD:
            retVal = __doXMKD(__cmdSocket, param);
            break;
        case CommonFtp::CMD_XPWD:
            retVal = __doXPWD(__cmdSocket, param);
            break;
        case CommonFtp::CMD_XRCP:
            retVal = __doXRCP(__cmdSocket, param);
            break;
        case CommonFtp::CMD_XRMD:
            retVal = __doXRMD(__cmdSocket, param);
            break;
        case CommonFtp::CMD_XRSQ:
            retVal = __doXRSQ(__cmdSocket, param);
            break;
        case CommonFtp::CMD_XSEM:
            retVal = __doXSEM(__cmdSocket, param);
            break;
        case CommonFtp::CMD_XSEN:
            retVal = __doXSEN(__cmdSocket, param);
            break;
        case CommonFtp::CMD_PORTFILE:
            retVal = __doPortFile(__cmdSocket, param);
            break;
        default:
            retVal = __doUNKNOW(__cmdSocket, param);
            break;
        }
        switch(retVal)
        {
        case SERVE_OK:
            break;  /* do log */
        case SERVE_FAIL:
            break;  /* do log */
        case SERVE_ABQUIT:
            return -1;
        case SERVE_QUIT:
            return 0;
            break;
        default:
            return -1;
        }
    }
    return 0;
}

ServeClient::RetType ServeClient::__doABOR(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doACCT(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doADAT(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doALLO(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doAPPE(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doAUTH(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doCCC(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doCDUP(SocketDescriptor fd, const char *param)
{
    return __doCWD(fd, "..");
}

ServeClient::RetType ServeClient::__doCONF(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doCWD(SocketDescriptor fd, const char *param)
{
    __path->cd(param);
    CommonFtp::sendResp(fd, RESP_250);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doDELE(SocketDescriptor fd, const char *param)
{
    char buf[MAXPATHLEN];
    CommonFile::virtualToReal(__path->getBasePath(), __path->getVirtualPath(), param, buf);
    if(!CommonFile::dele(buf))
    {
        CommonFtp::sendResp(fd, RESP_550);
        return SERVE_FAIL;
    }
    CommonFtp::sendResp(fd, RESP_200);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doENC(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doEPRT(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doEPSV(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doFEAT(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doHELP(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_214EX, "All these commands are avaliable!");
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doLANG(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doLIST(SocketDescriptor fd, const char *param)
{
    return __doNLST(fd, "./");
}

ServeClient::RetType ServeClient::__doLPRT(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doLPSV(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doMDTM(SocketDescriptor fd, const char *param)
{
    char buf[MAXPATHLEN];
    long modTime;
    struct tm mdt;

    if(CommonFile::virtualToReal(__path->getBasePath(), __path->getVirtualPath(), param, buf) == NULL || !CommonFile::isExists(buf))
    {
        CommonFtp::sendResp(fd, RESP_501);
        return SERVE_FAIL;
    }
    modTime = CommonFile::getLastModifyTime(buf);
    gmtime_r(&modTime, &mdt);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S %A", &mdt);
    CommonFtp::sendResp(fd, RESP_213TIME, buf);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doMIC(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doMKD(SocketDescriptor fd, const char *param)
{
    char buf[MAXPATHLEN];

    if(CommonFile::virtualToReal(__path->getBasePath(), __path->getVirtualPath(), param, buf) == NULL || CommonFile::mkdir(buf) == false)
    {
        CommonFtp::sendResp(fd, RESP_501);
        return SERVE_FAIL;
    }
    CommonFtp::sendResp(fd, RESP_200EX, "Dir created!");
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doMLSD(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doMLST(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doMODE(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doNLST(SocketDescriptor fd, const char *param)
{
    FILE *file = NULL;
    char buf[MAXPATHLEN], buf2[MAXPATHLEN];

    if(__ui.isListable() != true)
    {
        CommonFtp::sendResp(fd, RESP_550EX, "You have not permission to list file");
        return SERVE_DENY;
    }
    else if(!__data->isDataSocketOpen())
    {
        CommonFtp::sendResp(fd, RESP_425);
        return SERVE_FAIL;
    }
    CommonFile::virtualToReal(__path->getBasePath(), __path->getVirtualPath(), param, buf2);
    sprintf(buf, "/bin/ls %s -al", buf2);
    if((file = popen(buf, "r")) == NULL)
    {
        CommonFtp::sendResp(fd, RESP_501);
        return SERVE_FAIL;
    }
    CommonFtp::sendResp(fd, RESP_150);
    __data->transport(fileno(file));
    CommonFtp::sendResp(fd, RESP_226);
    __data->closeDataSocket();
    fclose(file);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doNOOP(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_200EX, "NOOP");
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doOPTS(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doPASS(SocketDescriptor fd, const char *param)
{
    static int tryTimes = 5;
    char buf[MAXPASSWORDLEN];

    IniReader::read(__username, "Password", buf, sizeof(buf), "", SETTINGFILE);
    if(strcmp(param, buf) != 0)
    {
        if(--tryTimes == 0)
            return SERVE_ABQUIT;
        return SERVE_FAIL;
    }
    CommonFtp::sendResp(fd, RESP_230EX, __username);

    /* read user setting */
    __ui.parseInfo(__username, SETTINGFILE);
    delete __path;
    __path = NULL;
    __path = new VirtualPath(__ui.getWorkDirectory());
    tryTimes = 5;
    __isLoggedIn = true;
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doPASV(SocketDescriptor fd, const char *param)
{
    switch(__data->createDataSocket(DataTransport::M_PASV, param))
    {
    case DataTransport::CREATE_OK:
        break;
    case DataTransport::CREATE_FAIL:
        CommonFtp::sendResp(fd, RESP_425);
        return SERVE_FAIL;
    default:
        return SERVE_TIMEOUT;
    }
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doPBSZ(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doPORT(SocketDescriptor fd, const char *param)
{
    if(__data->createDataSocket(DataTransport::M_PORT, param) != DataTransport::CREATE_OK)
    {
        CommonFtp::sendResp(fd, RESP_425);
        return SERVE_FAIL;
    }
    CommonFtp::sendResp(fd, RESP_200);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doPROT(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doPWD(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_257EX, __path->getVirtualPath());
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doQUIT(SocketDescriptor fd, const char *param)
{
    sendByeMsg();
    return SERVE_QUIT;
}

ServeClient::RetType ServeClient::__doREIN(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doREST(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doRMD(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doRNFR(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doRETR(SocketDescriptor fd, const char *param)
{
    FileDescriptor file;
    char buf[MAXPATHLEN];

    CommonFile::virtualToReal(__path->getBasePath(), __path->getVirtualPath(), param, buf);
    if(!CommonFile::isExists(buf))
    {
        CommonFtp::sendResp(fd, RESP_501);
        return SERVE_FAIL;
    }
    if((file = open(buf, O_RDONLY)) < 0)
    {
        CommonFtp::sendResp(fd, RESP_550);
        return SERVE_FAIL;
    }
    //new
    __data->transport(file);
    close(file);
    CommonFtp::sendResp(fd, RESP_150);
    CommonFtp::sendResp(fd, RESP_226);
    __data->closeDataSocket();
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doRNTO(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doSITE(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doSIZE(SocketDescriptor fd, const char *param)
{
    char buf[MAXBUFLEN];
    unsigned long size = 0;

    if(CommonFile::virtualToReal(__path->getBasePath(), __path->getVirtualPath(), param, buf) == NULL || !CommonFile::isFile(buf))
    {
        CommonFtp::sendResp(fd, RESP_533);
        return SERVE_FAIL;
    }
    size = CommonFile::length(buf);
    sprintf(buf, "%lu", size);
    CommonFtp::sendResp(fd, RESP_213EX, buf);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doSMNT(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doSTAT(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doSTOR(SocketDescriptor fd, const char *param)
{
    FileDescriptor file;
    char buf[MAXPATHLEN];
    CommonFtp::sendResp(fd, RESP_150);
    CommonFile::virtualToReal(__path->getBasePath(), __path->getVirtualPath(), param, buf);
    if((file = open(buf, O_RDWR | O_CREAT |O_TRUNC|S_IRUSR|S_IWUSR|S_IXUSR
                    |S_IRGRP|S_IWGRP)) < 0)
    {
        CommonFtp::sendResp(fd, RESP_550);
        return SERVE_FAIL;
    }
    fchmod(file,0755);
    __data->getFile(file);
    CommonFtp::sendResp(fd, RESP_200);
     close(file);
    __data->closeDataSocket();
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doSTOU(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doSTRU(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doSYST(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_215);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doTYPE(SocketDescriptor fd, const char *param)
{
    switch(param[0])
    {
    case 'i':
    case 'I':
        __data->setDataType(DataTransport::TYPE_BINARY);
        break;
    case 'a':
    case 'A':
        __data->setDataType(DataTransport::TYPE_ASCII);
        break;
    default:
        CommonFtp::sendResp(fd, RESP_501);
        return SERVE_FAIL;
    }

    CommonFtp::sendResp(fd, RESP_200EX, "Set type ok!");
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doUSER(SocketDescriptor fd, const char *param)
{
    char buf[20];

    __isLoggedIn = false;
    IniReader::read("User", param, buf, sizeof(buf), "", SETTINGFILE);
    if(strcmp(buf, "Enable") != 0)
    {
        CommonFtp::sendResp(fd, RESP_332);
        return SERVE_FAIL;
    }
    strncpy(__username, param, sizeof(__username));
    CommonFtp::sendResp(fd, RESP_331EX, __username); /* need a password */
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doXCUP(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doXMKD(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doXPWD(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doXRCP(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doXRMD(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doXRSQ(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doXSEM(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doXSEN(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}

ServeClient::RetType ServeClient::__doUNKNOW(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_500);
    return SERVE_OK;
}
ServeClient::RetType ServeClient::__doPortFile(SocketDescriptor fd, const char *param)
{
    CommonFtp::sendResp(fd, RESP_502);
    return SERVE_OK;
}
