#ifndef __H_COMMONFILE__
#define __H_COMMONFILE__

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include "Utilities.h"

const char SEPCHAR = '/';

class CommonFile
{
public:
    CommonFile();
    ~CommonFile();

public:
    static char *getAbsoultPath(const char *path, char *buf);
    static char *getName(const char *path, char *buf);
    static char *getParent(const char *path, char *buf);

    static bool isDir(const char *path);
    static bool isChildDir(const char *parent, const char *child);
    static bool isFile(const char *path);
    static bool isReadable(const char *path);
    static bool isWriteable(const char *path);
    static bool isExecuteable(const char *path);
    static bool isExists(const char *path);

    static long getLastModifyTime(const char *path);
    static unsigned long length(const char *path);

    static bool dele(const char *path);
    static bool rmdir(const char *path);
    static bool mkdir(const char *path);
    static bool rename(const char *src, const char *tar);
    static char *toRealPath(const char *curPath, const char *path, char *buf);
    static char *virtualToReal(const char *basePath, const char *curPath, const char *virtPath, char *buf);
};

#endif // COMMONFILE_H
