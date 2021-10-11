#include "index.h"
#include "serve_static.h"

int view_index(struct http_response_t *response, struct http_request_t *request)
{
    serve_static(response, "/static/index.html");

    return 0;
}
