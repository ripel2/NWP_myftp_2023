/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** server loop
*/

#include "server.h"
#include "client.h"
#include "logger.h"

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

static void create_file_descriptor_set(server_t *server)
{
    client_t *client = (client_t *)server->clients;

    FD_ZERO(&server->read_fds);
    FD_ZERO(&server->write_fds);
    FD_SET(server->fd, &server->read_fds);
    FD_SET(server->fd, &server->write_fds);
    for (; client; client = client->next) {
        FD_SET(client->fd, &server->read_fds);
    }
}

static void select_and_accept(server_t *server)
{
    int activity = select(server->max_fd + 1, &server->read_fds,
        &server->write_fds, NULL, NULL);
    int new_socket = 0;

    if (activity < 0 && errno != EINTR)
        exit_fatal("select: %s", strerror(errno));
    if (!FD_ISSET(server->fd, &server->read_fds))
        return;
    new_socket = accept(server->fd, (struct sockaddr *)&server->addr,
        &server->addr_len);
    if (new_socket < 0)
        exit_fatal("accept: %s", strerror(errno));
    LOG_INFO("New connection, socket fd: %d, ip: %s, port: %d",
        new_socket, inet_ntoa(server->addr.sin_addr),
        ntohs(server->addr.sin_port));
    server->max_fd = new_socket;
    if (add_client(&server->clients, new_socket))
        welcome_client(server, server->clients);
}

static void remove_exit_clients(server_t *server)
{
    client_t *client = (client_t *)server->clients;
    int fd = 0;

    while (client != NULL) {
        fd = 0;
        if (client->needs_exit) {
            LOG_DEBUG("Removing client %d", client->fd);
            fd = client->fd;
            FD_CLR(fd, &server->read_fds);
            FD_CLR(fd, &server->write_fds);
            client = client->next;
            remove_client(&server->clients, fd);
        }
        if (client != NULL)
            client = client->next;
        if (fd > 0 && close(fd) < 0)
            LOG_WARNING("close: %s", strerror(errno));
    }
}

void server_loop(server_t *server)
{
    while (true) {
        create_file_descriptor_set(server);
        select_and_accept(server);
        handle_clients(server);
        remove_exit_clients(server);
    }
}
