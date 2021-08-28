#ifndef HEADER__ROUTER
#define HEADER__ROUTER

#include "request.h"
#include "response.h"

#define WEB_BASE_PATH ("./web")

int process_request(struct http_response_t *response, struct http_request_t *request);

int serve_static(struct http_response_t *response, char *path);

#endif
