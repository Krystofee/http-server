#ifndef HEADER__RESPONSE
#define HEADER__RESPONSE

#include "request.h"
#include "header.h"
#include "utils.h"

struct http_response_t
{
    int protocol;
    int status_code;
    int timestamp;

    // Headers
    struct http_header_t headers[MAX_HTTP_HEADER_COUNT];
    int header_count;

    // Body
    char *body;
    int body_length;
} http_response_t;

int init_http_response(struct http_response_t *response, struct http_request_t *request);
int render_http_response(struct http_response_t *response, char **rendered, int *rendered_length);

int add_http_response_header(struct http_response_t *response, char *key, char *value);
int set_http_response_body(struct http_response_t *response, char *body);

#endif
