#include "CommonFile.h"

CommonFile::CommonFile()
{
    ;
}

CommonFile::~CommonFile()
{
    ;
}

char *CommonFile::getAbsoultPath(const char *path, char *buf)
{
    buf[0] = '\0';
    realpath(path, buf);
    return buf;
}

char *CommonFile::getName(const char *path, char *buf)
{
    int i, start;

    if(path == NULL)
        return NULL;
    for(start = 0, i = 0; path[i]; ++i)
        if(path[i] == SEPCHAR)
            start = i;
    if(path[start] != SEPCHAR)
        return strcpy(buf, path);
    return Utilities::rangeCopy(buf, start + 1, i, path);
}

char *CommonFile::getParent(const char *path, char *buf)
{
    const char *ptr = NULL;
    int end;

    if(path == NULL)
        return NULL;
    if((ptr = strrchr(path, SEPCHAR)) == NULL)
        return NULL;

    if((end = ptr - path) == 0)
        ++end;
    return Utilities::rangeCopy(buf, 0, end, path);
}

bool CommonFile::isDir(const char *path)
{
    struct stat st;

    if(path)
    {
        stat(path, &st);
        return S_ISDIR(st.st_mode);
    }
    return false;
}

bool CommonFile::isChildDir(const char *parent, const char *child)
{
    while(*parent && *child && *parent == *child)
    {
        ++parent;
        ++child;
    }
    return *parent == '\0';
}

bool CommonFile::isFile(const char *path)
{
    struct stat st;

    if(path)
    {
        stat(path, &st);
        return !S_ISDIR(st.st_mode);
    }
    return false;
}

bool CommonFile::isReadable(const char *path)
{
    return access(path, R_OK) == 0;
}

bool CommonFile::isWriteable(const char *path)
{
    return access(path, W_OK) == 0;
}

bool CommonFile::isExecuteable(const char *path)
{
    return access(path, X_OK) == 0;
}

bool CommonFile::isExists(const char *path)
{
    return access(path, F_OK) == 0;
}


long CommonFile::getLastModifyTime(const char *path)
{
    struct stat st;
    stat(path, &st);
    return st.st_atime;
}

unsigned long CommonFile::length(const char *path)
{
    if(path)
    {
        struct stat st;
        stat(path, &st);
        return st.st_size;
    }
    return 0;
}

bool CommonFile::dele(const char *path)
{
    return remove(path) == 0;
}

bool CommonFile::rmdir(const char *path)
{
    return remove(path) == 0;
}

bool CommonFile::mkdir(const char *path)
{
    return ::mkdir(path, 0777) == 0;
}

bool CommonFile::rename(const char *src, const char *tar)
{
    char buf[256];

    getName(tar, buf);
    return rename(src, buf) == 0;
}

char *CommonFile::toRealPath(const char *curPath, const char *path, char *buf)
{
    int preSep, curSep, delims, len;
    char *tmp;

    if(path == NULL || path[0] == '\0')
        return NULL;

    if(curPath != NULL)
        strcpy(buf, curPath);
    else
        sprintf(buf, "%c", SEPCHAR);

    if(*path == SEPCHAR) /* 首字母为分隔符, 证明是个绝对路径 */
    {
        buf[1] = '\0';
        while(*path && *path == SEPCHAR)  /* 过滤重复的分隔符 */
            ++path;
    }

    len = strlen(buf);   /* 取得已填充的buf大小 */
    tmp = buf + len;
    for(preSep = 0, curSep = 1;; ++curSep)
    {
        if(path[curSep] == SEPCHAR || path[curSep] == '\0')
        {
            delims = curSep - preSep;
            if(delims == 2 && path[preSep] == '.' && path[preSep + 1] == '.') /* ch back */
            {
                if(tmp > buf + len)
                {
                    --tmp;
                    while(*tmp != SEPCHAR)
                        --tmp;
                }
            }
            else if(!((delims == 1 && path[preSep] == '.') || delims == 0))
            {
                *tmp++ = SEPCHAR;
                Utilities::rangeCopy(tmp, preSep, curSep, path);
                tmp += delims;
            }
            preSep = curSep + 1;
        }
        if(path[curSep] == '\0')
            break;
    }
    *tmp = '\0';
    return buf;
}

char *CommonFile::virtualToReal(const char *basePath, const char *curPath, const char *virtPath, char *buf)
{
    char *tmp = NULL, tmpBuf[4096];

    tmp = toRealPath(curPath, virtPath, buf);
    if(tmp == NULL)
        return NULL;
    strcpy(tmpBuf, tmp);
    sprintf(buf, "%s%c%s", basePath, SEPCHAR, tmpBuf);
    return buf;
}
