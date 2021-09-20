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

    // Pattern matching in path

    int max_groups = 16;
    regmatch_t group_array[max_groups];

    char *current_path = "/static/address.html";

    int match = regexec(path_regex, current_path, max_groups, group_array, 0);

    printf("match %d (%d)\n", match, REG_NOMATCH);

    if (match == 0)
    {
        for (int i = 1; i < max_groups; ++i)
        {
            if (group_array[i].rm_so == -1)
                break;

            char match_str[256];
            memset(match_str, 0, 256);
            strncpy(match_str, current_path + group_array[i].rm_so, group_array[i].rm_eo - group_array[i].rm_so);

            printf("- %s\n", match_str);
        }
    }

    // ^^^^^^^ Pattern matching in path

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

    register_view(router, "^/static/(.*)$", &view_serve_static);

    return 0;
}

int register_view(router_t *router, char *path, view_t view)
{
    int err = 0;

    if (router->paths_capacity <= router->paths_count)
        extend_router_capacity(router);

    regex_t *path_regex = malloc(sizeof(regex_t));
    err = regcomp(path_regex, path, REG_EXTENDED);
    if (err != 0)
        perror("regexp");

    router->paths[router->paths_count].path = path_regex;
    router->paths[router->paths_count].view = view;
    router->paths_count++;

    return 0;
}

int destroy_router(router_t *router)
{
    for (int i = 0; i < router->paths_count; ++i)
    {
        registered_view_t *path_regex = router->paths->path;
        regfree(path_regex);
    }

    free(router->paths);
}
