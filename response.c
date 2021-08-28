#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <sys/time.h>

#include "response.h"
#include "router.h"

int init_http_response(struct http_response_t *response, struct http_request_t *request)
{
    int err = 0;

    memset(response, 0, sizeof(struct http_response_t));

    response->protocol = HTTP_PROTOCOL_HTTP_1;
    response->status_code = 200;

    struct timeval tv;
    gettimeofday(&tv, NULL);
    response->timestamp = tv.tv_usec;

    process_request(response, request);

    return err;
}

int add_http_response_header(struct http_response_t *response, char *key, char *value)
{
    struct http_header_t *h = &response->headers[response->header_count];
    memcpy(h->key, key, strlen(key));
    memcpy(h->value, value, strlen(value));

    response->header_count++;

    return 0;
}

int set_http_response_body(struct http_response_t *response, char *body)
{
    response->body_length = strlen(body);
    response->body = malloc(sizeof(char) * response->body_length);
    memset(response->body, 0, sizeof(char) * response->body_length);

    char body_length_str[32];
    memset(body_length_str, 0, 32);
    sprintf(body_length_str, "%d", response->body_length);
    add_http_response_header(response, "Content-Length", body_length_str);

    memcpy(response->body, body, response->body_length);

    return 0;
}

int render_http_response(struct http_response_t *response, char **result, int *result_length)
{
    int err = 0;
    char *rendered = malloc(sizeof(char) * 2048);

    strcpy(rendered, "HTTP/1.1 200 OK\n");

    for (int i = 0; i < response->header_count; ++i)
    {
        struct http_header_t *h = &response->headers[i];
        strncat(rendered, h->key, strlen(h->key));
        strncat(rendered, ": ", 2);
        strncat(rendered, h->value, strlen(h->value));
        strncat(rendered, "\n", 1);
    }

    strncat(rendered, "\n", sizeof("\n"));
    strncat(rendered, response->body, response->body_length);

    printf("rendered: %s\n", rendered);

    *result = rendered;
    *result_length = strlen(rendered);

    return err;
}
