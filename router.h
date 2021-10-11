#ifndef HEADER__ROUTER
#define HEADER__ROUTER

#include "router.h"
#include "server.h"
#include "request.h"
#include "response.h"
#include "views/types.h"

int register_view(router_t *router, char *path, view_t view);
int init_router(router_t *router);
int destroy_router(router_t *router);

int process_request(server_t *server, struct http_response_t *response, struct http_request_t *request);

int serve_static(struct http_response_t *response, char *path);

#endif
