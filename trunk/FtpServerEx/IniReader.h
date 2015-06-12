#ifndef __H_INIREADER__
#define __H_INIREADER__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int FILENOTFOUND = -1;
const int KEYVALUENOTFOUND = -2;

class IniReader
{
public:
    static int read(const char *section, const char *key, char *buf, int size, const char *defaultStr, const char *file);
    static int readInt(const char *section, const char *key, int &value, const char *file);
    static bool write(const char *section, const char *key, const char *value, const char *file);
};

#endif
