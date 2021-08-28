#ifndef HEADER__HEADER
#define HEADER__HEADER

#define MAX_HTTP_HEADER_LENGTH (1024 * 8) // 8 kB

struct http_header_t
{
    char key[MAX_HTTP_HEADER_LENGTH];
    char value[MAX_HTTP_HEADER_LENGTH];
} http_header_t;

#endif
