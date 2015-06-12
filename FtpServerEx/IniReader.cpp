#include "IniReader.h"

int IniReader::read(const char *section, const char *key, char *retBuf, int size, const char *defaultStr, const char *file)
{
    FILE *fp = NULL;
    char buf[1024], str[1024], *ptr = NULL;
    int keyLen, valueLen;

    if((fp = fopen(file, "rt")) == NULL)
        return FILENOTFOUND;
    
    retBuf[0] = '\0';
    keyLen = strlen(key);
    sprintf(str, "[%s]", section);
    while(fgets(buf, sizeof(buf), fp) != NULL)
    {
        if(buf[0] == ';')
            continue;
        if(strstr(buf, str) != NULL)
        {
            while(fgets(buf, sizeof(buf), fp) != NULL)
            {
                   if(buf[0] == ';')
                    continue;
                if((ptr = strchr(buf, '=')) != NULL)
                {
                    if(strncmp(key, buf, keyLen) == 0)
                    {
                        valueLen = strlen(ptr + 1) - 1;
                        if(valueLen >= size)
                            valueLen = size - 1;
                        strncpy(retBuf, ptr + 1, valueLen);
                        retBuf[valueLen] = '\0';
                        fclose(fp);
                        return strlen(retBuf);
                    }
                }
            }
        }
    }
    fclose(fp);
    strcpy(retBuf, defaultStr);
    return KEYVALUENOTFOUND;
}

int IniReader::readInt(const char *section, const char *key, int &value, const char *file)
{
    char buf[128];
    IniReader::read(section, key, buf, sizeof(buf), "", file);
    if(strcmp(buf, "") == 0)
        return -1;
    value = atoi(buf);
    return 0;
}

bool IniReader::write(const char *section, const char *key, const char *value, const char *file)
{
    return true;
}