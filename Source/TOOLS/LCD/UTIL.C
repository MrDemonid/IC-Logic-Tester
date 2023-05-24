#include "util.h"

char buff[4];

char *byte2dec(unsigned char num)
{
    char *t = &buff[3];
    buff[3] = '\0';
    while (num)
    {
        t--;
        *t = (num % 10) + '0';
        num /= 10;
    }
    return t;
}
