#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <regex.h>

#include "response.h"
#include "request.h"
#include "router.h"
#include "mimetypes.h"

// Import views
#include "views/serve_static.h"
#include "views/index.h"

int process_request(struct http_response_t *response, struct http_request_t *request)
{
    view_serve_static(response, request);

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

    return 0;
}

int init_router(router_t *router)
{
    router->paths_count = 0;
    router->paths_capacity = 0;
    extend_router_capacity(router);

    register_view(router, "^/$", &view_index);
    register_view(router, "^/index.html$", &view_index);
    register_view(router, "^/form.html$", &view_serve_static);
    register_view(router, "^/second_page.html$", &view_serve_static);

    register_view(router, "^/static/.*$", &view_serve_static);

    return 0;
}

int register_view(router_t *router, char *path, view_t view)
{
    if (router->paths_capacity <= router->paths_count)
        extend_router_capacity(router);

    router->paths[router->paths_count].path = path;
    router->paths[router->paths_count].view = view;
    router->paths_count++;

    return 0;
}
