#ifndef HEADER__ROUTER
#define HEADER__ROUTER

#include "request.h"
#include "response.h"
#include "views/types.h"

typedef struct registered_view
{
    char *path;
    view_t view;
} registered_view_t;

typedef struct router
{
    struct registered_view *paths;
    int paths_count;
    int paths_capacity;
} router_t;

int register_view(router_t *router, char *path, view_t view);
int init_router(router_t *router);

int process_request(struct http_response_t *response, struct http_request_t *request);

int serve_static(struct http_response_t *response, char *path);

#endif
