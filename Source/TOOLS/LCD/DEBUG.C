#include <stdio.h>
#include <stdarg.h>

void dprintf(const char *fmt, ...)
{
    FILE *log;
    va_list argptr;

    va_start(argptr, fmt);
    log = fopen("debug.log","at");
    if (!log)
    {
        vfprintf(stderr, fmt, argptr);
    } else {
        vfprintf(log, fmt, argptr);
        fflush(log);
    }
    va_end(argptr);

    if (log)
        fclose(log);
}
