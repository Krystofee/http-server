#ifndef HEADER__SERVER
#define HEADER__SERVER

#include <regex.h>

#include "views/types.h"

#define PORT (8080)

#define BACKLOG (10)
#define REQUEST_DATA_BUFFER_SIZE (1024)

typedef struct registered_view
{
    regex_t *path;
    view_t view;
} registered_view_t;

typedef struct router
{
    struct registered_view *paths;
    int paths_count;
    int paths_capacity;
} router_t;

typedef struct server
{
    int socket_fd;

    router_t router;
} server_t;

#endif
