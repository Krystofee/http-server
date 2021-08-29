#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <memory.h>

#include <netinet/in.h>
#include <sys/socket.h>

#include "macros.h"
#include "request.h"
#include "response.h"

#define PORT (8080)

#define BACKLOG (10)
#define REQUEST_DATA_BUFFER_SIZE (1024)

struct server_t
{
    int socket_fd;
};

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

int server_listen(struct server_t *server)
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

int server_accept(struct server_t *server)
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
    // prepare response

    struct http_response_t response;
    init_http_response(&response, &request);

    char *rendered_response;
    int rendered_response_length;
    render_http_response(&response, &rendered_response, &rendered_response_length);

    // const char *reply =
    //     "HTTP/1.1 200 OK\n"
    //     "Server: TyKokos/0.0.1\n"
    //     "Content-Type: text/html\n"
    //     "Content-Length: 78\n"
    //     "Connection: close\n"
    //     "\n"
    //     "<html><head><title>test</title></head><body>Hello world of HTML!</body></html>";

    err = send(conn_fd, rendered_response, rendered_response_length, 0);
    return_error(err, "send");

    err = close(conn_fd);
    return_error(err, "close");

    destroy_http_request(&request);
    free(rendered_response);

    return err;
}

int server_serve(struct server_t *server)
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

int server_destroy(struct server_t *server)
{
    int err = 0;

    err = shutdown(server->socket_fd, SHUT_RDWR);

    return 0;
}

int main(int argc, char const *argv[])
{
    printf("Server on 0.0.0.0:%d\n", PORT);

    int err = 0;
    struct server_t server = {0};

    prepare_env();
    err = server_listen(&server);
    if (err)
    {
        printf("Server could not be started\n");
        return 1;
    }

    int process_id = 0;
    err = (process_id = fork());
    return_error(err, "fork");

    if (process_id == 0)
    {
        // Parent process

        for (;;)
        {
            // check signals
            if (sig_flag)
            {
                printf("Shutting down (interrupt)\n");
                kill(process_id, SIGTERM);
                break;
            }
        }
    }
    else
    {
        // Child process
        server_serve(&server);
    }

    server_destroy(&server);

    return 0;
}
