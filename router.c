#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "response.h"
#include "request.h"
#include "router.h"

int process_request(struct http_response_t *response, struct http_request_t *request)
{
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

    char *buffer = 0;
    long length;

    char *open_mode = 0;

    // determine file open mode based on extension
    int full_path_length = strlen(full_path);
    if (strcmp(&full_path[full_path_length - 3], "jpg") == 0)
    {
        open_mode = "rb";
    }
    else
    {
        open_mode = "r";
    }

    printf("loading static file %s (%s)\n", full_path, open_mode);

    FILE *f = fopen(full_path, open_mode);

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer)
        {
            fread(buffer, 1, length, f);
        }
        fclose(f);

        set_http_response_body(response, buffer);
        // add_http_response_header(response, "Content-Type", "html");
    }

    return 0;
}
