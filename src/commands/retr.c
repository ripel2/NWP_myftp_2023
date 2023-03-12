/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** retr command
*/

#include "myftp.h"
#include "server.h"
#include "commands.h"
#include "client.h"
#include "logger.h"

#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

static void retr_transfer(client_t *client, int ns)
{
    char *path = client->buffer + 5;
    int fd = open(path, O_RDONLY);
    char buffer[1024] = {0};
    ssize_t ret = 0;

    if (fd < 0)
        file_open_error(client);
    ret = read(fd, buffer, 1024);
    while (ret > 0) {
        if (write(ns, buffer, ret))
            file_write_error(client);
        memset(buffer, 0, 1024);
        ret = read(fd, buffer, 1024);
    }
    close(ns);
    file_transfer_success(client);
}

static void retr_command_active(server_t *server, client_t *client)
{
    active_t *active = client->transfer_socket;
    pid_t pid = 0;

    (void)server;
    pid = fork();
    if (pid < 0) {
        LOG_ERROR("Fork failed");
        thread_send(client, "550 Fork failed.\r\n");
        return;
    }
    if (pid == 0) {
        chdir(client->path);
        retr_transfer(client, active->fd);
        exit(0);
    } else {
        destroy_active_socket(active);
        client->transfer_socket = NULL;
    }
}

static void retr_command_passive(server_t *server, client_t *client)
{
    passive_t *passive = client->transfer_socket;
    int new_socket = 0;
    pid_t pid = 0;

    (void)server;
    pid = fork();
    if (pid < 0) {
        thread_send(client, "550 Fork failed.\r\n");
        return;
    }
    if (pid == 0) {
        chdir(client->path);
        new_socket = accept(passive->fd, (struct sockaddr *)&passive->addr,
        (socklen_t *)&passive->addr_len);
        retr_transfer(client, new_socket);
        exit(0);
    } else {
        destroy_passive_socket(passive);
        client->transfer_socket = NULL;
    }
}

static void retr_check_file(server_t *server, client_t *client)
{
    char *path = client->buffer + 5;

    if (!path_exists(path)) {
        LOG_DEBUG("Client %d tried to RETR a non-existing file", client->fd);
        client_printf(client, "%d %s.\r\n", 550, "File not found");
        return;
    }
    if (!path_is_file(path)) {
        LOG_DEBUG("Client %d tried to RETR a directory", client->fd);
        client_printf(client, "%d %s.\r\n", 550, "Can't RETR a directory");
        return;
    }
    client_printf(client, "%d %s.\r\n", 150,
    "File status okay; about to open data connection");
    if (client->transfer_type == ACTIVE) {
        retr_command_active(server, client);
    } else if (client->transfer_type == PASSIVE) {
        retr_command_passive(server, client);
    }
}

void retr_command(server_t *server, client_t *client)
{
    FD_SET(client->fd, &server->write_fds);
    if (!client->logged_in) {
        LOG_DEBUG("Client %d tried to RETR without logged in", client->fd);
        client_printf(client, "%d %s.\r\n", 530, "Not logged in");
        return;
    }
    if (client->transfer_socket == NULL) {
        LOG_DEBUG("Client %d tried to RETR without using PORT or PASV first",
        client->fd);
        client_printf(client, "%d %s.\r\n", 425, "Use PORT or PASV first");
        return;
    }
    retr_check_file(server, client);
}
