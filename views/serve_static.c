#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../mimetypes.h"
#include "serve_static.h"
#include "types.h"

int view_serve_static(struct http_response_t *response, struct http_request_t *request)
{
    // TODO

    if (strcmp(request->address, "/") == 0)
    {
        serve_static(response, "/index.html");
    }
    else
    {
        serve_static(response, request->address);
    }

    return 0;
}

int serve_static(struct http_response_t *response, char *path)
{
    char full_path[256];
    memset(full_path, 0, 256);
    strcat(full_path, WEB_BASE_PATH);
    strcat(full_path, path);

    char *buffer = NULL;
    long length;
    FILE *f = fopen(full_path, "rb");

    if (f)
    {
        response->status_code = 200;

        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        fread(buffer, 1, length, f);
        fclose(f);

        add_http_response_header(response, "Content-Type", guess_mimetype(full_path));
        set_http_response_body(response, buffer, length);

        free(buffer);
    }
    else
    {
        response->status_code = 404;
    }

    return 0;
}
