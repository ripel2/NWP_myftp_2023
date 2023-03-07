/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** init server
*/

#include "server.h"
#include "logger.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>

static void init_server_default_values(server_t *server, char *port, char *path)
{
    server->fd = 0;
    server->max_fd = 0;
    server->addr.sin_family = AF_INET;
    server->addr.sin_port = htons(atoi(port));
    server->addr.sin_addr.s_addr = INADDR_ANY;
    server->addr_len = sizeof(server->addr);
    server->default_home = path;
    server->clients = NULL;
    FD_ZERO(&server->read_fds);
    FD_ZERO(&server->write_fds);
}

void init_server(server_t *server, char *port, char *path)
{
    struct sockaddr *addr = (struct sockaddr *)&server->addr;
    int optval = 1;

    init_server_default_values(server, port, path);
    server->fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server->fd < 0)
        exit_fatal("socket: %s", strerror(errno));
    if (setsockopt(server->fd, SOL_SOCKET, SO_REUSEADDR, &optval,
    sizeof(optval)) < 0)
        exit_fatal("setsockopt: %s", strerror(errno));
    if (bind(server->fd, addr, server->addr_len) < 0)
        exit_fatal("bind: %s", strerror(errno));
    if (listen(server->fd, LISTEN_BACKLOG) < 0)
        exit_fatal("listen: %s", strerror(errno));
    server->max_fd = server->fd;
    FD_SET(server->fd, &server->read_fds);
    FD_SET(server->fd, &server->write_fds);
    LOG_INFO("Server started on port %s", port);
}
