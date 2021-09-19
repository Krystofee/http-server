#ifndef HEADER__VIEWS__SERVE_STATIC
#define HEADER__VIEWS__SERVE_STATIC

#include "types.h"

#define WEB_BASE_PATH "./web"

declare_view(view_serve_static);

int serve_static(struct http_response_t *response, char *path);

#endif
