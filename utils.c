#include <memory.h>
#include <stdio.h>

#include "utils.h"

int method_to_code(char *str)
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

char *code_to_method(int code)
{
    if (code == HTTP_METHOD_GET)
    {
        return "GET";
    }

    if (code == HTTP_METHOD_POST)
    {
        return "POST";
    }

    if (code == HTTP_METHOD_PUT)
    {
        return "PUT";
    }

    if (code == HTTP_METHOD_PATCH)
    {
        return "PATCH";
    }

    if (code == HTTP_METHOD_OPTIONS)
    {
        return "OPTIONS";
    }

    if (code == HTTP_METHOD_HEAD)
    {
        return "HEAD";
    }

    return "[error method]";
}

int get_protocol_code(char *str)
{
    if (strcmp(str, "HTTP/1.1") == 0)
    {
        return HTTP_PROTOCOL_HTTP_1;
    }

    return -1;
}

char *int_to_string(long int n, char *result, int result_length)
{
    memset(result, 0, result_length);
    sprintf(result, "%ld", n);
    return result;
}
