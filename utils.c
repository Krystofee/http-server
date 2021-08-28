#include <memory.h>

#include "utils.h"

int get_method_code(char *str)
{
    if (strcmp(str, "GET") == 0)
    {
        return HTTP_METHOD_GET;
    }

    if (strcmp(str, "POST") == 0)
    {
        return HTTP_METHOD_POST;
    }

    if (strcmp(str, "PUT") == 0)
    {
        return HTTP_METHOD_PUT;
    }

    if (strcmp(str, "PATCH") == 0)
    {
        return HTTP_METHOD_PATCH;
    }

    if (strcmp(str, "OPTIONS") == 0)
    {
        return HTTP_METHOD_OPTIONS;
    }

    if (strcmp(str, "HEAD") == 0)
    {
        return HTTP_METHOD_HEAD;
    }

    return -1;
}

int get_protocol_code(char *str)
{
    if (strcmp(str, "HTTP/1.1") == 0)
    {
        return HTTP_PROTOCOL_HTTP_1;
    }

    return -1;
}
