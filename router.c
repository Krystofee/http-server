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

    // // determine file open mode based on extension
    // char *open_mode = 0;
    // int full_path_length = strlen(full_path);
    // if (strcmp(&full_path[full_path_length - 4], ".jpg") == 0 || strcmp(&full_path[full_path_length - 4], ".png") == 0)
    // {
    //     open_mode = "rb";
    //     add_http_response_header(response, "Content-Type", "image/png");
    // }
    // else
    // {
    //     open_mode = "r";
    // }

    printf("loading static file %s\n", full_path);
    // add_http_response_header(response, "Content-Type", "image/png");

    char *buffer = NULL;
    long length;
    FILE *f = fopen(full_path, "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        fread(buffer, 1, length, f);
        fclose(f);

        printf("file: %s (%ld)\n", buffer, length);

        set_http_response_body(response, buffer, length);

        free(buffer);
        // add_http_response_header(response, "Content-Type", "html");
    }

    return 0;
}
