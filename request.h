#ifndef HEADER__REQUEST
#define HEADER__REQUEST

#include "header.h"
#include "utils.h"

#define MAX_HTTP_REQUEST_LENGTH (1024 * 2048) // 2048 kB
#define MAX_ADDRESS_LENGTH (1024 * 2)
#define MAX_HTTP_HEADER_COUNT (128)

struct http_request_t
{
    int method;
    char address[MAX_ADDRESS_LENGTH];
    int protocol;

    // Headers
    struct http_header_t headers[MAX_HTTP_HEADER_COUNT];
    int header_count;

    // Body
    char *body;
    int body_length;
} http_request_t;

int init_http_request(int conn_fd, struct http_request_t *request);
int destroy_http_request(struct http_request_t *request);

int read_http_request_starting_line(struct http_request_t *request, char *line_buffer, int line_length);
int add_http_request_header(struct http_request_t *request, char *line, int line_length);
int get_content_length(struct http_request_t *request);

#endif
