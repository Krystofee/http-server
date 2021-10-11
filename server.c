#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <memory.h>

#include <netinet/in.h>
#include <sys/socket.h>

#include "server.h"
#include "macros.h"
#include "request.h"
#include "response.h"
#include "router.h"

volatile sig_atomic_t sig_flag = 0;

void handle_sig_int(int sig)
{
    sig_flag = 2;
}

int prepare_env()
{
    // Register signals
    signal(SIGINT, handle_sig_int);

    return 0;
}

int server_init(server_t *server)
{
    init_router(&server->router);

    return 0;
}

int server_listen(server_t *server)
{
    int err = 0;

    err = (server->socket_fd = socket(AF_INET, SOCK_STREAM, 0));
    return_error(err, "socket");

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    err = bind(server->socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    return_error(err, "bind");

    listen(server->socket_fd, BACKLOG);
    return_error(err, "listen");

    return 0;
}

int server_accept(server_t *server)
{
    int err = 0;
    int conn_fd;
    socklen_t client_len;
    struct sockaddr_in client_addr;

    client_len = sizeof(client_addr);

    err = (conn_fd = accept(server->socket_fd, (struct sockaddr *)&client_addr, &client_len));
    return_error(err, "accept");

    struct http_request_t request = {0};
    init_http_request(conn_fd, &request);

    printf("\n");

    // Prepare response
    struct http_response_t response;
    init_http_response(&response, &request);

    // Process request by view
    process_request(server, &response, &request);

    char *rendered_response;
    int rendered_response_length;
    render_http_response(&response, &rendered_response, &rendered_response_length);

    err = send(conn_fd, rendered_response, rendered_response_length, 0);
    return_error(err, "send");

    err = close(conn_fd);
    return_error(err, "close");

    destroy_http_request(&request);
    free(rendered_response);

    printf("\n");

    return err;
}

int server_serve(server_t *server)
{
    int err = 0;

    for (;;)
    {
        err = server_accept(server);
        if (err == -1)
        {
            printf("Failed to accept connection\n");
            break;
        }
    }

    return err;
}

int server_destroy(server_t *server)
{
    int err = 0;

    err = shutdown(server->socket_fd, SHUT_RDWR);

    destroy_router(&server->router);

    return 0;
}

int main(int argc, char const *argv[])
{
    printf("Server on 0.0.0.0:%d\n", PORT);

    int err = 0;
    server_t server = {0};

    prepare_env();
    server_init(&server); // handle errors
    err = server_listen(&server);
    if (err)
    {
        printf("Server could not be started\n");
        return 1;
    }

    int listener_process_id = 0;
    err = (listener_process_id = fork());
    return_error(err, "fork");

    if (listener_process_id == 0)
    {
        // Parent process

        for (;;)
        {
            // check signals
            if (sig_flag)
            {
                printf("Shutting down (interrupt)\n");
                kill(listener_process_id, SIGKILL);
                server_destroy(&server);
                break;
            }
        }
    }
    else
    {
        // Child process
        server_serve(&server);
    }

    return 0;
}
