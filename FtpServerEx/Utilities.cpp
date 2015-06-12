#include "Utilities.h"

char *Utilities::strCopy(const char *str)
{
    char *retStr = NULL;
    unsigned int len = strlen(str);

    retStr = new char[len + 1];
    ::strcpy(retStr, str);
    return retStr;
}
