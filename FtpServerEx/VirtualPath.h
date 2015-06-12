#ifndef __H_VIRTUALCD__
#define __H_VIRTUALCD__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Utilities.h"

const int MAXBUFLEN = 4096;
//const char SEPCHAR = '/';

const int NULLPATHTOCD = -1;

class VirtualPath
{
public:
    VirtualPath(const char *basePath);
    ~VirtualPath();

public:
    int cd(const char *path);
    const char *getVirtualPath() const;
    const char *getBasePath() const;
    const char *getRealPath();

private:
    char _virtualPath[MAXBUFLEN];
    char _basePath[MAXBUFLEN];
    char _realPath[MAXBUFLEN];
    int _virtualPathLen;
};

#endif
