#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

#include <sys/socket.h>

#include "macros.h"
#include "request.h"

int init_http_request(int conn_fd, struct http_request_t *request)
{
    int err = 0;

    memset(request, 0, sizeof(struct http_request_t));

    char buffer[MAX_HTTP_REQUEST_LENGTH];
    memset(&buffer, 0, MAX_HTTP_REQUEST_LENGTH);

    err = recv(conn_fd, buffer, MAX_HTTP_REQUEST_LENGTH, 0);
    return_error(err, "recv");

    printf("\n%s\n", buffer);

    char line_buffer[MAX_HTTP_HEADER_LENGTH];

    int line = 0;
    int header_start = 0, header_end = 0;
    while (header_end < MAX_HTTP_REQUEST_LENGTH)
    {
        if (buffer[header_end] == '\n')
        {
            memset(line_buffer, 0, MAX_HTTP_HEADER_LENGTH);
            memcpy(&line_buffer, &buffer[header_start], header_end - header_start);

            if (line == 0)
            {
                read_http_request_starting_line(request, line_buffer, header_end - header_start);
            }
            else
            {
                add_http_request_header(request, line_buffer, header_end - header_start);
            }

            if (header_end == header_start + 1)
            {
                int body_len = get_content_length(request);

                if (body_len > 0)
                {
                    request->body_length = body_len;
                    char *body_ptr = malloc(sizeof(char) * request->body_length);
                    memcpy(body_ptr, &buffer[header_end + 1], request->body_length);
                    request->body = body_ptr;
                    printf("body: %s (%d)\n", request->body, request->body_length);
                }
            }

            line++;
            header_start = header_end + 1;
        }

        header_end++;
    }

    return err;
}

int destroy_http_request(struct http_request_t *request)
{
    free(request->body);

    return 0;
}

int read_http_request_starting_line(struct http_request_t *request, char *line_buffer, int line_length)
{
    int err = 0;

    // read line data
    int i_start = 0,
        i_end = 0;
    int word_c = 0;

    char word_buffer[MAX_HTTP_HEADER_LENGTH];

    while (i_end < line_length)
    {
        if (line_buffer[i_end] == ' ')
        {
            memset(&word_buffer, 0, MAX_HTTP_HEADER_LENGTH);
            memcpy(&word_buffer, &line_buffer[i_start], i_end - i_start);

            switch (word_c)
            {
            case 0:
                request->method = get_method_code(word_buffer);
                break;
            case 1:
                memcpy(&request->address, &line_buffer[i_start], i_end - i_start);
            case 2:
                request->protocol = get_protocol_code(word_buffer);
                break;
            }

            i_start = i_end + 1;
            i_end++;
            word_c++;
        }
        i_end++;
    }

    printf("request: %d %s %d\n", request->method, request->address, request->protocol);

    return err;
}

int add_http_request_header(struct http_request_t *request, char *line_buffer, int line_length)
{
    int err = 0;
    const struct http_header_t *header_ptr = &request->headers[request->header_count];

    // read header data
    int i_start = 0,
        i_end = 0;

    while (i_end < line_length)
    {
        if (line_buffer[i_end] == ':')
        {
            memcpy((char *)header_ptr->key, &line_buffer[0], i_end);
            memcpy((char *)header_ptr->value, &line_buffer[i_end + 2], line_length - i_end + 1);
            printf("header: %s=%s\n", header_ptr->key, header_ptr->value);
            request->header_count++;
            break;
        }
        i_end++;
    }

    return err;
}

int get_content_length(struct http_request_t *request)
{
    for (int i = 0; i < request->header_count; ++i)
    {
        struct http_header_t *h = &request->headers[i];
        if (strcmp(h->key, "Content-Length") == 0)
        {
            int result = atoi(h->value);
            return result;
        }
    }
    return -1;
}
