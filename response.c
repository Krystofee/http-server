#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <sys/time.h>

#include "response.h"
#include "router.h"
#include "status_codes.h"

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

int set_http_response_body(struct http_response_t *response, char *body, long body_length)
{
    response->body_length = body_length;
    response->body = malloc(sizeof(char) * response->body_length);
    memset(response->body, 0, sizeof(char) * response->body_length);

    char body_length_str[32];
    memset(body_length_str, 0, 32);
    int_to_string(response->body_length, body_length_str, 32);
    add_http_response_header(response, "Content-Length", body_length_str);

    memcpy(response->body, body, response->body_length);

    return 0;
}

int render_http_response(struct http_response_t *response, char **result, int *result_length)
{
    int err = 0;
    char *rendered = malloc(sizeof(char) * (MAX_HTTP_HEADER_COUNT * MAX_HTTP_HEADER_LENGTH + response->body_length));
    *result_length = 0;

    // Protocol
    strcat(rendered, "HTTP/1.1 ");
    // Status code
    char status_code_str[3];
    memset(status_code_str, 0, 3);
    int_to_string(response->status_code, status_code_str, 3);
    strcat(rendered, status_code_str);
    strcat(rendered, " ");
    // Description
    strcat(rendered, get_status_code_description(response->status_code));
    strcat(rendered, "\n");

    printf("< %s %s %s\n", "HTTP/1.1", status_code_str, get_status_code_description(response->status_code));

    for (int i = 0; i < response->header_count; ++i)
    {
        struct http_header_t *h = &response->headers[i];
        strncat(rendered, h->key, strlen(h->key));
        strncat(rendered, ": ", 2);
        strncat(rendered, h->value, strlen(h->value));
        strncat(rendered, "\r\n", 2);

        printf("< %s=%s\n", h->key, h->value);
    }

    strncat(rendered, "\r\n", sizeof("\r\n"));

    *result_length = strlen(rendered) + response->body_length;

    memcpy(&rendered[strlen(rendered)], response->body, response->body_length);

    if (response->body_length > 0)
    {
        printf("< Body:\n");
        printf("%s\n", response->body);
        printf("< BODY END\n");
    }

    *result = rendered;

    return err;
}
