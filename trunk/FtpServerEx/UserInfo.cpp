#include "UserInfo.h"
#include "FtpServer.h"

UserInfo::UserInfo()
{
    //parseInfo(username, settingFile);
}

UserInfo::~UserInfo()
{
    ;
}

void UserInfo::parseInfo(const char *username, const char *settingFile)
{
    char buf[128];
    int ret;

    strcpy(_username, username);
    ret = IniReader::read("User", username, buf, sizeof(buf), "", settingFile);
    if(ret < 0 || strcmp(buf, "") == 0 || strcmp(buf, "Enable") != 0)
    {
        _perm = 0;
        return;
    }
    IniReader::read(username, "WorkDirectory", buf, sizeof(buf), "/tmp", settingFile);
    strcpy(_workDirectory, buf);
    if((ret = IniReader::readInt(username, "Permission", _perm, settingFile)) != 0)
        _perm = 1;
}

const char *UserInfo::getUsername() const
{
    return _username;
}

const char *UserInfo::getWorkDirectory() const
{
    return _workDirectory;
}

int UserInfo::getPermission() const
{
    return _perm;
}

int UserInfo::isCreateDirable(int perm)
{
    return perm & 2;
}

int UserInfo::isDeleteable(int perm)
{
    return perm & 32;
}

int UserInfo::isRenameable(int perm)
{
    return perm & 16;
}

int UserInfo::isUploadable(int perm)
{
    return perm & 8;
}

int UserInfo::isDownloadable(int perm)
{
    return perm & 4;
}

int UserInfo::isListable(int perm)
{
    return perm & 1;
}

int UserInfo::isCreateDirable() const
{
    return isCreateDirable(getPermission());
}

int UserInfo::isDeleteable() const
{
    return isDeleteable(getPermission());
}

int UserInfo::isRenameable() const
{
    return isRenameable(getPermission());
}

int UserInfo::isUploadable() const
{
    return isUploadable(getPermission());
}

int UserInfo::isDownloadable() const
{
    return isDownloadable(getPermission());
}

int UserInfo::isListable() const
{
    return isListable(getPermission());
}
