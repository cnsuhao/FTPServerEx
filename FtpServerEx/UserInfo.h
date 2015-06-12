#ifndef __H_USERINFO__
#define __H_USERINFO__

#include "CommonFtp.h"
#include "IniReader.h"
#include "Utilities.h"

const int MAXPATHLEN = 2048;
const int MAXUSERNAMELEN = 128;

class FtpServer;

class UserInfo
{
public:
    UserInfo();
    ~UserInfo();

public:
    void parseInfo(const char *username, const char *settingFile);

    const char *getUsername() const;
    const char *getWorkDirectory() const;
    int getPermission() const;

    static int isCreateDirable(int perm);
    static int isDeleteable(int perm);
    static int isRenameable(int perm);
    static int isUploadable(int perm);
    static int isDownloadable(int perm);
    static int isListable(int perm);
    int isCreateDirable() const;
    int isDeleteable() const;
    int isRenameable() const;
    int isUploadable() const;
    int isDownloadable() const;
    int isListable() const;

private:
    int _perm;
    char _username[MAXUSERNAMELEN];
    char _workDirectory[MAXPATHLEN];
};

#endif
