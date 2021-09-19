#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "response.h"
#include "request.h"
#include "router.h"
#include "mimetypes.h"

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

int extend_router_capacity(router_t *router)
{
    router->paths_capacity += 100;
    if (router->paths == NULL)
    {
        router->paths = malloc(sizeof(registered_view_t) * router->paths_capacity);
    }
    else
    {
        router->paths = realloc(router->paths, sizeof(registered_view_t) * router->paths_capacity);
    }
}

int init_router(router_t *router)
{
    router->paths_count = 0;
    router->paths_capacity = 0;
    extend_router_capacity(router);

    register_view(router, "/index.html", &serve_static);
    register_view(router, "/form.html", &serve_static);
    register_view(router, "/second_page.html", &serve_static);
}

int register_view(router_t *router, char *path, view_t view)
{
    if (router->paths_capacity <= router->paths_count)
        extend_router_capacity(router);

    router->paths[router->paths_count].path = path;
    router->paths[router->paths_count].view = view;
    router->paths_count++;
}
