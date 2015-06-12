#include "VirtualPath.h"

const int SEPCHAR = '/';

VirtualPath::VirtualPath(const char *basePath)
{
    strcpy(_basePath, basePath);
    _virtualPath[0] = SEPCHAR;
    _virtualPath[1] = '\0';
    _virtualPathLen = 1;
}

VirtualPath::~VirtualPath()
{
    ;
}

int VirtualPath::cd(const char *path)
{
    int preSep, curSep, delims;
    char *tmp;

    if(path == NULL || path[0] == '\0')
        return NULLPATHTOCD;
    if(*path == SEPCHAR)
    {
        _virtualPath[1] = '\0';
        _virtualPathLen = 1;
        while(*path && *path == SEPCHAR)
            ++path;
        if(*path == '\0')
            return 0;
    }
    tmp = _virtualPath + _virtualPathLen;
    for(preSep = 0, curSep = 1; ; ++curSep)
    {
        if(path[curSep] == SEPCHAR || path[curSep] == '\0')
        {
            delims = curSep - preSep;
            if(delims == 2 && path[preSep] == '.' && path[preSep + 1] == '.') /* ch back */
            {
                if(tmp > _virtualPath + 1)
                {
                    --tmp;
                    while(*tmp != SEPCHAR)
                        --tmp;
                }
            }
            else if(!(delims == 1 && path[preSep] == '.' || delims == 0))
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
    _virtualPathLen = tmp - _virtualPath;
    return 0;
}

const char *VirtualPath::getVirtualPath() const
{
    return _virtualPath;
}

const char *VirtualPath::getBasePath() const
{
    return _basePath;
}

const char *VirtualPath::getRealPath()
{
    const char *ptr = _virtualPath;

    while(*ptr && *ptr == SEPCHAR)
        ++ptr;
    if(*ptr == '\0')
        return _basePath;
    sprintf(_realPath, "%s%c%s", _basePath, SEPCHAR, ptr);
    return _realPath;
}
