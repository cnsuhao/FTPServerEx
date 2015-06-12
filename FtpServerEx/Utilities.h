#ifndef __H_UTILITIES__
#define __H_UTILITIES__

#include <string.h>

class Utilities
{
public:
    static char *strcpy(const char *str);
    static char *strCopy(const char *str);
    template<class Object>
    static Object *rangeCopy(Object *dest, int start, int end, const Object *src, bool isStr = true);
};

template<class Object>
Object *Utilities::rangeCopy(Object *dest, int start, int end, const Object *src, bool isStr)
{
    int i;

    if(end <= start)
        return NULL;
    for(i = start; i < end; ++i)
        dest[i - start] = src[i];
    if(isStr)
        dest[i - start] = '\0';
    return dest;
}

#endif