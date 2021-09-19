#include <stdio.h>

#include "debug.h"

void debug(const char *prefix, const char *fmt, ...)
{
    printf("%s >>> ", prefix);

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf("\n");
}
