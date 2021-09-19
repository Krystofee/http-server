#ifndef HEADER__VIEWS__TYPES
#define HEADER__VIEWS__TYPES

#include "../request.h"
#include "../response.h"

typedef int (*view_t)(struct http_response_t *response, struct http_request_t *request);

#define declare_view(name) int name(struct http_response_t *response, struct http_request_t *request)

#endif
